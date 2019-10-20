#include <unordered_map>
#include <functional>
#include <set>
#include "Node.hpp"
#include "Xml.hpp"
#include "Exception.hpp"
#include "TextParser.hpp"
#include "TextPrinter.hpp"

static Doxydown::NodePtr findInCache(Doxydown::NodeCacheMap& cache, const std::string& refid) {
    const auto found = cache.find(refid);
    if (found != cache.end()) {
        return found->second;
    } else {
        return nullptr;
    }
}

static Doxydown::NodePtr findOrCreate(const std::string& inputDir, Doxydown::NodeCacheMap& cache, const std::string& refid, const bool isGroupOrFile) {
    auto found = findInCache(cache, refid);
    if (found) {
        if (found->isEmpty()) {
            return Doxydown::Node::parse(cache, inputDir, found, isGroupOrFile);
        } else {
            return found;
        }
    } else {
        return Doxydown::Node::parse(cache, inputDir, refid, isGroupOrFile);
    }
}

Doxydown::NodePtr Doxydown::Node::parse(NodeCacheMap& cache,
                                        const std::string& inputDir,
                                        const std::string& refid,
                                        const bool isGroupOrFile) {
    const auto ptr = std::make_shared<Node>(refid);
    return parse(cache, inputDir, ptr, isGroupOrFile);
}

Doxydown::NodePtr Doxydown::Node::parse(NodeCacheMap& cache,
                                        const std::string& inputDir,
                                        const NodePtr& ptr,
                                        const bool isGroupOrFile) {
    const auto refidPath = Path::join(inputDir, ptr->refid + ".xml");
    Xml xml(refidPath);

    auto root = assertChild(xml, "doxygen");
    auto compounddef = assertChild(root, "compounddef");
    auto briefdescription = assertChild(compounddef, "briefdescription");

    ptr->xmlPath = refidPath;
    ptr->name = assertChild(compounddef, "compoundname").getText();
    ptr->kind = strToKind(compounddef.getAttr("kind"));
    ptr->empty = false;
    cache.insert(std::make_pair(ptr->getRefid(), ptr));

    // Inner members such as functions
    auto sectiondef = compounddef.firstChildElement("sectiondef");
    while (sectiondef) {
        auto memberdef = sectiondef.firstChildElement("memberdef");
        while (memberdef) {
            const auto childKindStr = memberdef.getAttr("kind");
            const auto childRefid = memberdef.getAttr("id");
            const auto found = findInCache(cache, childRefid);
            const auto child = found ? found : Node::parse(memberdef, childRefid);
            ptr->addChild(child);
            
            if (isGroupOrFile) {
                // Only update child's parent if this is a group and the member has
                // just been created (not in cache)
                if (!found) child->setParent(ptr.get());
            } else {
                // Only update child's parent if we are not processing directories
                if (isKindLanguage(ptr->getKind()) && isKindLanguage(child->getKind())) {
                    child->setParent(ptr.get());
                }
            }
            memberdef = memberdef.nextSiblingElement("memberdef");
        }
        sectiondef = sectiondef.nextSiblingElement("sectiondef");
    }

    // A helper lambda to go through different sections and process children
    auto innerProcess = [&](Xml::Element& parent, const std::string& name) {
        parent.allChildElements(name, [&](Xml::Element& e) {
            const auto childRefid = e.getAttr("refid");
            auto child = findOrCreate(inputDir, cache, childRefid, isGroupOrFile);
            ptr->addChild(child);

            // Only update child's parent if we are not processing directories
            if (!isGroupOrFile || 
                (isGroupOrFile && child->getKind() == Kind::GROUP) || 
                (isGroupOrFile && child->getKind() == Kind::FILE) ||
                (isGroupOrFile && child->getKind() == Kind::DIR)) {
                child->setParent(ptr.get());
            }
        });
    };

    innerProcess(compounddef, "innerclass");
    innerProcess(compounddef, "innerstruct");
    innerProcess(compounddef, "innernamespace");
    if (isGroupOrFile) {
        innerProcess(compounddef, "innergroup");
        innerProcess(compounddef, "innerdir");
        innerProcess(compounddef, "innerfile");
    }

    if (!isGroupOrFile) {
        ptr->parseInheritanceInfo(inputDir, cache, compounddef);
    }
    ptr->parseBaseInfo(compounddef);
    return ptr;
}

Doxydown::NodePtr Doxydown::Node::parse(Xml::Element& memberdef, const std::string& refid) {
    auto ptr = std::make_shared<Node>(refid);
    ptr->name = assertChild(memberdef, "name").getText();
    ptr->kind = strToKind(memberdef.getAttr("kind"));
    ptr->empty = true;
    ptr->parseBaseInfo(memberdef);

    if (ptr->kind == Kind::ENUM) {
        auto enumvalue = memberdef.firstChildElement("enumvalue");
        while (enumvalue) {
            auto value = std::make_shared<Node>(enumvalue.getAttr("id"));
            value->name = enumvalue.firstChildElement("name").getText();
            value->kind = Kind::ENUMVALUE;
            value->empty = false;
            value->parent = ptr.get();
            value->parseBaseInfo(enumvalue);
            ptr->addChild(value);
            enumvalue = enumvalue.nextSiblingElement("enumvalue");
        }
    }
    return ptr;
}

Doxydown::Xml::Element Doxydown::Node::assertChild(const Xml& xml, const std::string& name) {
    auto child = xml.firstChildElement(name);
    if (!child) throw DOXYGEN_EXCEPTION("Unable to find <{}> element in root element file {}", name, xml.getPath());
    return child;
}

Doxydown::Xml::Element Doxydown::Node::assertChild(const Xml::Element& xml, const std::string& name) {
    auto child = xml.firstChildElement(name);
    if (!child)
        throw        DOXYGEN_EXCEPTION(
        "Unable to find <{}> element in element <{}> line {} file {}",
        name, xml.getName(), xml.getLine(), xml.getDocument().getPath()
    );
    return child;
}

Doxydown::Node::Node(const std::string& refid)
    : refid(refid) {

}

void Doxydown::Node::parseInheritanceInfo(const std::string& inputDir, NodeCacheMap& cache, const Xml::Element& element) {
    element.allChildElements("basecompoundref", [&](Xml::Element& e) {
        BaseClass base;
        base.ptr = findOrCreate(inputDir, cache, e.getAttr("refid"), false);
        base.virt = strToVirtual(e.getAttr("virt"));
        base.prot = strToVisibility(e.getAttr("prot"));
        data.baseClasses.push_back(base);
    });

    element.allChildElements("derivedcompoundref", [&](Xml::Element& e) {
        DerivedClass derived;
        derived.ptr = findOrCreate(inputDir, cache, e.getAttr("refid"), false);
        derived.virt = strToVirtual(e.getAttr("virt"));
        derived.prot = strToVisibility(e.getAttr("prot"));
        data.derivedClasses.push_back(derived);
    });
}

void Doxydown::Node::parseBaseInfo(const Xml::Element& element) {
    data.isAbstract = element.getAttr("abstract", "no") == "yes";
    data.isStatic = element.getAttr("static", "no") == "yes";
    data.prot = strToVisibility(element.getAttr("prot", "public"));
    data.virt = strToVirtual(element.getAttr("virt", "non-virtual"));
    data.prot = strToVisibility(element.getAttr("prot", "public"));

    auto locationElement = element.firstChildElement("location");
    if (locationElement) {
        Location location;
        location.file = locationElement.getAttr("file", "");
        location.line = std::stoi(locationElement.getAttr("line", "0"));
        location.column = std::stoi(locationElement.getAttr("column", "0"));
        location.bodyFile = locationElement.getAttr("bodyfile", "");
        location.bodyStart = std::stoi(locationElement.getAttr("bodystart", "0"));
        location.bodyEnd = std::stoi(locationElement.getAttr("bodyend", "0"));
        data.location = location;
    }

    auto briefdescription = assertChild(element, "briefdescription");
    const auto brief = TextParser::parse(briefdescription);
    data.brief = TextPrinter::printPlain(brief);
}

Doxydown::NodePtr Doxydown::Node::findChild(const std::string& refid) const {
    for (const auto& ptr : children) {
        if (ptr->getRefid() == refid) return ptr;
    }

    throw EXCEPTION("Refid {} not found in {}", refid, this->refid);
}

Doxydown::NodePtr Doxydown::Node::find(const std::string& refid) const {
    auto test = findRecursively(refid);
    if (!test) throw EXCEPTION("Refid {} not found in {}", refid, this->refid);
    return test;
}

Doxydown::NodePtr Doxydown::Node::findRecursively(const std::string& refid) const {
    for (auto it = children.begin(); it != children.end(); ++it) {
        if (it->get()->refid == refid) return *it;
        auto test = it->get()->findRecursively(refid);
        if (test) return test;
    }
    return nullptr;
}

Doxydown::Node::Kind Doxydown::Node::strToKind(const std::string& kind) {
    static std::unordered_map<std::string, Kind> kinds = {
        {"class", Kind::CLASS},
        {"namespace", Kind::NAMESPACE},
        {"struct", Kind::STRUCT},
        {"interface", Kind::INTERFACE},
        {"function", Kind::FUNCTION},
        {"variable", Kind::VARIABLE},
        {"typedef", Kind::TYPEDEF},
        {"enum", Kind::ENUM},
        {"dir", Kind::DIR},
        {"file", Kind::FILE},
        {"group", Kind::GROUP}
    };

    const auto it = kinds.find(kind);
    if (it == kinds.end()) {
        throw EXCEPTION("Kind \"{}\" not recognised, please contact the author", kind);
    }

    return it->second;
}

Doxydown::Node::Visibility Doxydown::Node::strToVisibility(const std::string& vis) {
    static std::unordered_map<std::string, Visibility> values = {
        {"public", Visibility::PUBLIC},
        {"protected", Visibility::PROTECTED},
        {"private", Visibility::PRIVATE},
    };

    const auto it = values.find(vis);
    if (it == values.end()) {
        throw EXCEPTION("Visibility \"{}\" not recognised, please contact the author", vis);
    }

    return it->second;
}

Doxydown::Node::Virtual Doxydown::Node::strToVirtual(const std::string& virt) {
    static std::unordered_map<std::string, Virtual> values = {
        {"non-virtual", Virtual::NON_VIRTUAL},
        {"virtual", Virtual::VIRTUAL},
        {"pure", Virtual::PURE_VIRTUAL},
        {"pure-virtual", Virtual::PURE_VIRTUAL},
    };

    const auto it = values.find(virt);
    if (it == values.end()) {
        throw EXCEPTION("Virtuality \"{}\" not recognised, please contact the author", virt);
    }

    return it->second;
}

bool Doxydown::Node::isKindLanguage(const Kind kind) {
    static std::set<Kind> kinds = {
        Kind::CLASS,
        Kind::NAMESPACE,
        Kind::STRUCT,
        Kind::INTERFACE,
        Kind::ENUM,
        Kind::FUNCTION,
        Kind::TYPEDEF,
        Kind::VARIABLE
    };

    return kinds.find(kind) != kinds.end();
}

bool Doxydown::Node::isKindFile(const Kind kind) {
    static std::set<Kind> kinds = {
        Kind::FILE,
        Kind::DIR
    };

    return kinds.find(kind) != kinds.end();
}
