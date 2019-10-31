#include <unordered_map>
#include <functional>
#include <cassert>
#include <fmt/format.h>
#include <Doxydown/Node.hpp>
#include <Doxydown/Exception.hpp>
#include <Doxydown/XmlTextParser.hpp>
#include <Doxydown/TextPrinter.hpp>
#include <Doxydown/Log.hpp>
#include <Doxydown/Utils.hpp>
#include "ExceptionUtils.hpp"

class Doxydown::Node::Temp {
public:
    XmlTextParser::Node brief;
};

static Doxydown::NodePtr findInCache(Doxydown::NodeCacheMap& cache, const std::string& refid) {
    const auto found = cache.find(refid);
    if (found != cache.end()) {
        return found->second;
    } else {
        return nullptr;
    }
}

static Doxydown::NodePtr findOrCreate(const std::string& inputDir,
                                      Doxydown::NodeCacheMap& cache,
                                      const std::string& refid,
                                      const bool isGroupOrFile) {
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
    assert(!refid.empty());
    const auto ptr = std::make_shared<Node>(refid);
    return parse(cache, inputDir, ptr, isGroupOrFile);
}

Doxydown::NodePtr Doxydown::Node::parse(NodeCacheMap& cache,
                                        const std::string& inputDir,
                                        const NodePtr& ptr,
                                        const bool isGroupOrFile) {
    const auto refidPath = Utils::join(inputDir, ptr->refid + ".xml");
    Log::i("Loading {}", refidPath);
    Xml xml(refidPath);

    auto root = assertChild(xml, "doxygen");
    auto compounddef = assertChild(root, "compounddef");

    ptr->xmlPath = refidPath;
    ptr->name = assertChild(compounddef, "compoundname").getText();
    ptr->kind = toEnum<Kind>(compounddef.getAttr("kind"));
    ptr->empty = false;
    cache.insert(std::make_pair(ptr->refid, ptr));

    // Inner members such as functions
    auto sectiondef = compounddef.firstChildElement("sectiondef");
    while (sectiondef) {
        auto memberdef = sectiondef.firstChildElement("memberdef");
        while (memberdef) {
            const auto childKindStr = memberdef.getAttr("kind");
            const auto childRefid = memberdef.getAttr("id");
            const auto found = findInCache(cache, childRefid);
            const auto child = found ? found : Node::parse(memberdef, childRefid);
            ptr->children.push_back(child);

            if (isGroupOrFile) {
                // Only update child's parent if this is a group and the member has
                // just been created (not in cache)
                if (!found) child->parent = ptr.get();
            } else {
                // Only update child's parent if we are not processing directories
                if (isKindLanguage(ptr->kind) && isKindLanguage(child->kind)) {
                    child->parent = ptr.get();
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
            ptr->children.push_back(child);

            // Only update child's parent if we are not processing directories
            if (!isGroupOrFile ||
                (isGroupOrFile && child->kind == Kind::MODULE) ||
                (isGroupOrFile && child->kind == Kind::FILE) ||
                (isGroupOrFile && child->kind == Kind::DIR)) {
                child->parent = ptr.get();
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
        ptr->parseInheritanceInfo(compounddef);
    }

    ptr->parseBaseInfo(compounddef);
    return ptr;
}

Doxydown::NodePtr Doxydown::Node::parse(Xml::Element& memberdef, const std::string& refid) {
    assert(!refid.empty());

    auto ptr = std::make_shared<Node>(refid);
    ptr->name = assertChild(memberdef, "name").getText();
    ptr->kind = toEnum<Kind>(memberdef.getAttr("kind"));
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
            ptr->children.push_back(value);
            enumvalue = enumvalue.nextSiblingElement("enumvalue");
        }
    }
    return ptr;
}

Doxydown::Xml::Element Doxydown::Node::assertChild(const Xml& xml, const std::string& name) {
    auto child = xml.firstChildElement(name);
    if (!child) throw EXCEPTION("Unable to find <{}> element in root element file {}", name, xml.getPath());
    return child;
}

Doxydown::Xml::Element Doxydown::Node::assertChild(const Xml::Element& xml, const std::string& name) {
    auto child = xml.firstChildElement(name);
    if (!child)
        throw EXCEPTION(
        "Unable to find <{}> element in element <{}> line {} file {}",
        name, xml.getName(), xml.getLine(), xml.getDocument().getPath()
    );
    return child;
}

Doxydown::Node::Node(const std::string& refid)
    : temp(new Temp),
      refid(refid) {

}

Doxydown::Node::~Node() = default;

void Doxydown::Node::parseBaseInfo(const Xml::Element& element) {
    const auto briefdescription = assertChild(element, "briefdescription");
    temp->brief = XmlTextParser::parseParas(briefdescription);
    visibility = toEnum<Visibility>(element.getAttr("prot", "public"));

    switch (kind) {
        case Kind::DEFINE: {
            type = Type::DEFINES;
            break;
        }
        case Kind::FRIEND: {
            type = Type::FRIENDS;
            break;
        }
        case Kind::VARIABLE: {
            type = Type::ATTRIBUTES;
            break;
        }
        case Kind::FUNCTION: {
            type = Type::FUNCTIONS;
            break;
        }
        case Kind::ENUMVALUE:
        case Kind::ENUM:
        case Kind::TYPEDEF: {
            type = Type::TYPES;
            break;
        }
        case Kind::MODULE: {
            type = Type::MODULES;
            break;
        }
        case Kind::NAMESPACE: {
            type = Type::NAMESPACES;
            break;
        }
        case Kind::UNION:
        case Kind::INTERFACE:
        case Kind::STRUCT:
        case Kind::CLASS: {
            type = Type::CLASSES;
            break;
        }
        case Kind::FILE:
        case Kind::DIR: {
            type = Type::DIRORFILE;
            break;
        }
        default: {
            break;
        }
    }
}

void Doxydown::Node::parseInheritanceInfo(const Xml::Element& element) {
    element.allChildElements("basecompoundref", [&](Xml::Element& e) {
        ClassReference base;
        base.refid = e.getAttr("refid", "");
        base.name = e.getText();
        base.virt = toEnum<Virtual>(e.getAttr("virt"));
        base.prot = toEnum<Visibility>(e.getAttr("prot"));
        baseClasses.push_back(base);
    });

    element.allChildElements("derivedcompoundref", [&](Xml::Element& e) {
        ClassReference derived;
        derived.refid = e.getAttr("refid", "");
        derived.name = e.getText();
        derived.virt = toEnum<Virtual>(e.getAttr("virt"));
        derived.prot = toEnum<Visibility>(e.getAttr("prot"));
        derivedClasses.push_back(derived);
    });
}

void Doxydown::Node::finalize(const Config& config,
                              const TextPrinter& plainPrinter,
                              const TextPrinter& markdownPrinter,
                              const NodeCacheMap& cache) {
    static const auto anchorMaker = [](const Node& node) {
        if (!node.isStructured() && node.kind != Kind::MODULE) {
            return "#" + Utils::toLower(toStr(node.kind)) + "-" + Utils::safeAnchorId(node.name);
        } else {
            return std::string("");
        }
    };

    static const auto urlMaker = [](const Config& config, const Node& node) {
        switch (node.kind) {
            case Kind::STRUCT:
            case Kind::CLASS:
            case Kind::NAMESPACE:
            case Kind::MODULE:
            case Kind::DIR:
            case Kind::FILE:
            case Kind::INTERFACE:
            case Kind::UNION: {
                return config.baseUrl + typeToFolderName(config, node.type) + "/" +
                       Utils::stripAnchor(node.refid) + config.linkSuffix +
                       anchorMaker(node);
            }
            case Kind::ENUMVALUE: {
                const auto n = node.parent->parent;
                return config.baseUrl + typeToFolderName(config, n->type) + "/" +
                       Utils::stripAnchor(n->refid) + config.linkSuffix +
                       anchorMaker(node);
            }
            default: {
                const auto n = node.parent;
                return config.baseUrl + typeToFolderName(config, n->type) + "/" +
                       Utils::stripAnchor(n->refid) + config.linkSuffix +
                       anchorMaker(node);
            }
        }
    };
    
    if (temp) {
        brief = markdownPrinter.print(temp->brief);
        summary = plainPrinter.print(temp->brief);
        temp.reset();

        anchor = anchorMaker(*this);
        url = urlMaker(config, *this);
        if (config.linkLowercase) url = Utils::toLower(url);

        for (auto& klass : baseClasses) {
            if (!klass.refid.empty()) {
                const auto& ptr = cache.at(klass.refid);
                klass.url = urlMaker(config, *ptr);
                if (config.linkLowercase) klass.url = Utils::toLower(klass.url);
            }
        }

        for (auto& klass : derivedClasses) {
            if (!klass.refid.empty()) {
                const auto& ptr = cache.at(klass.refid);
                klass.url = urlMaker(config, *ptr);
                if (config.linkLowercase) klass.url = Utils::toLower(klass.url);
            }
        }
    }
}

Doxydown::Node::LoadDataResult Doxydown::Node::loadData(const Config& config,
                                                        const TextPrinter& plainPrinter,
                                                        const TextPrinter& markdownPrinter) const {

    Log::i("Parsing {}", xmlPath);
    Xml xml(xmlPath);

    auto root = assertChild(xml, "doxygen");
    auto compounddef = assertChild(root, "compounddef");

    auto data = loadData(config, plainPrinter, markdownPrinter, compounddef);
    ChildrenData childrenData;

    auto sectiondef = compounddef.firstChildElement("sectiondef");
    while (sectiondef) {
        auto memberdef = sectiondef.firstChildElement("memberdef");
        while (memberdef) {
            const auto childRefid = memberdef.getAttr("id");
            const auto childPtr = this->findChild(childRefid);

            childrenData.insert(std::make_pair(childPtr.get(), loadData(config, plainPrinter, markdownPrinter, memberdef)));
            if (childPtr->kind == Kind::ENUM) {
                auto enumvalue = memberdef.firstChildElement("enumvalue");
                while (enumvalue) {
                    const auto enumvalueRefid = enumvalue.getAttr("id");
                    const auto enumvaluePtr = childPtr->findChild(enumvalueRefid);
                    childrenData.insert(std::make_pair<const Node*, Data>(
                        enumvaluePtr.get(), 
                        loadData(config, plainPrinter, markdownPrinter, enumvalue))
                    );
                    enumvalue = enumvalue.nextSiblingElement("enumvalue");
                }
            }

            memberdef = memberdef.nextSiblingElement("memberdef");
        }
        sectiondef = sectiondef.nextSiblingElement("sectiondef");
    }

    return {data, childrenData};
}

Doxydown::Node::Data Doxydown::Node::loadData(const Config& config,
                                              const TextPrinter& plainPrinter,
                                              const TextPrinter& markdownPrinter,
                                              const Xml::Element& element) const {
    Data data;

    data.isAbstract = element.getAttr("abstract", "no") == "yes";
    data.isStatic = element.getAttr("static", "no") == "yes";
    data.isStrong = element.getAttr("strong", "no") == "yes";
    data.isConst = element.getAttr("const", "no") == "yes";
    data.isExplicit = element.getAttr("explicit", "no") == "yes";
    data.isInline = element.getAttr("inline", "no") == "yes";
    data.virt = toEnum<Virtual>(element.getAttr("virt", "non-virtual"));

    auto locationElement = element.firstChildElement("location");
    if (locationElement) {
        data.location.file = locationElement.getAttr("file", "");
        data.location.line = std::stoi(locationElement.getAttr("line", "0"));
        data.location.column = std::stoi(locationElement.getAttr("column", "0"));
        data.location.bodyFile = locationElement.getAttr("bodyfile", "");
        data.location.bodyStart = std::stoi(locationElement.getAttr("bodystart", "0"));
        data.location.bodyEnd = std::stoi(locationElement.getAttr("bodyend", "0"));
    }

    auto argsString = element.firstChildElement("argsstring");
    if (argsString && argsString.hasText()) data.argsString = argsString.getText();
    auto definition = element.firstChildElement("definition");
    if (definition && definition.hasText()) data.definition = definition.getText();
    auto initializer = element.firstChildElement("initializer");
    if (initializer) {
        data.initializer = markdownPrinter.print(XmlTextParser::parsePara(initializer));
    }

    const auto argsstring = element.firstChildElement("argsstring");
    if (argsstring) {
        data.argsString = markdownPrinter.print(XmlTextParser::parsePara(argsstring));
        data.isDefault = data.argsString.find("=default") != std::string::npos;
        data.isDeleted = data.argsString.find("=deleted") != std::string::npos;
    }

    const auto detaileddescription = assertChild(element, "detaileddescription");
    auto detailsParas = XmlTextParser::parseParas(detaileddescription);
    for (auto& para : detailsParas.children) {
        for (auto it = para.children.begin(); it != para.children.end();) {
            switch (it->type) {
            case XmlTextParser::Node::Type::SIMPLESEC: {
                DetailsSection section;
                section.text = markdownPrinter.print(*it);
                section.type = it->extra;
                data.detailsSections.push_back(std::move(section));
                it = para.children.erase(it);
                break;
            }
            case XmlTextParser::Node::Type::XREFSECT: {
                if (it->children.size() == 2 &&
                    it->children[0].type == XmlTextParser::Node::Type::XREFTITLE &&
                    it->children[1].type == XmlTextParser::Node::Type::XREFDESCRIPTION) {
                    DetailsSection section;
                    section.text = markdownPrinter.print(it->children[1]);
                    section.type = markdownPrinter.print(it->children[0]);
                    data.detailsSections.push_back(std::move(section));
                    it = para.children.erase(it);
                }
                break;
            }
            default: {
                ++it;
                break;
            }
            }
        }
    }
    data.details = markdownPrinter.print(detailsParas);
    const auto inbodydescription = element.firstChildElement("inbodydescription");
    if (inbodydescription) data.inbody = markdownPrinter.print(XmlTextParser::parseParas(inbodydescription));

    const auto includes = element.firstChildElement("includes");
    if (includes) {
        if (includes.getAttr("local", "no") == "no")
            data.includes = "<" + includes.getText() + ">";
        else
            data.includes = "\"" + includes.getText() + "\"";
    }
    const auto type = element.firstChildElement("type");

    if (type) {
        const auto typeParas = XmlTextParser::parsePara(type);
        data.type = markdownPrinter.print(typeParas);
        data.typePlain = plainPrinter.print(typeParas);
        if (data.type.find("friend ") == 0) {
            data.type = data.type.substr(7);
        }
        if (data.typePlain.find("friend ") == 0) {
            data.typePlain = data.typePlain.substr(7);
        }
    }

    auto param = element.firstChildElement("param");
    while (param) {
        Param p;
        const auto paramType = param.firstChildElement("type");
        const auto name = param.firstChildElement("declname");
        const auto defval = param.firstChildElement("defval");
        if (paramType) {
            const auto typeParas = XmlTextParser::parsePara(paramType);
            p.type = markdownPrinter.print(typeParas);
            p.typePlain = plainPrinter.print(typeParas);
        }
        if (name) {
            p.name = markdownPrinter.print(XmlTextParser::parsePara(name));
        }
        if (defval) {
            const auto defvalParas = XmlTextParser::parsePara(defval);
            p.defval = markdownPrinter.print(defvalParas);
            p.defvalPlain = plainPrinter.print(defvalParas);
        }
        param = param.nextSiblingElement("param");
        data.params.push_back(std::move(p));
    }

    return data;
}

Doxydown::NodePtr Doxydown::Node::findChild(const std::string& refid) const {
    for (const auto& ptr : children) {
        if (ptr->refid == refid) return ptr;
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

