#include "Macros.hpp"

#include <Doxybook/Utils.hpp>
#include <iostream>

#include <Doxybook/Doxygen.hpp>
#include <Doxybook/Enums.hpp>
#include <Doxybook/Exception.hpp>
#include <Doxybook/Log.hpp>
#include <Doxybook/Xml.hpp>
#include <optional>

using namespace Doxybook2;

template <typename Fn> void allOf(const Xml::Element& elm, const std::string& name, const Fn& fn) {
    auto child = elm.firstChildElement(name);
    while (child) {
        fn(child);
        child = child.nextSiblingElement(name);
    }
}

static Xml::Element assertFind(const Xml::Element& elm, const std::string& name) {
    const auto child = elm.firstChildElement(name);
    if (!child) {
        const auto& path = elm.getDocument().getPath();
        EXCEPTION("Unable to find element '{}' in file '{}'", name, path.string());
    }

    return child;
}

static void buildCache(Cache& cache, const NodeSharedPtr& node) {
    cache.insert(std::make_pair(node->refid, node));
    for (const auto& child : node->children) {
        ::buildCache(cache, child);
    }
}

template <typename T> static void concat(std::vector<T>& a, std::vector<T>&& b) {
    a.insert(a.end(), b.begin(), b.end());
}

Cache Doxygen::buildCache(const NodeSharedPtr& index) {
    Cache cache;
    for (const auto& child : index->children) {
        ::buildCache(cache, child);
    }
    return cache;
}

static void resolve(const Cache& cache, NodeRef& ref) {
    if (!ref.resolved() && !ref.empty() && !ref.asBasicRef().refid.empty()) {
        const auto ptr = cache.findByRef(ref.asBasicRef().refid);
        if (ptr) {
            ref.resolve(ptr);
        } else {
            Log::w("Unable to resolve reference: {}", ref.asBasicRef().refid);
        }
    }
}

void Doxygen::resolveReferences(const Cache& cache, const NodeSharedPtr& node) {
    if (node->location.has_value()) {
        resolve(cache, node->location.value().file);
    }

    if (node->bodyLocation.has_value()) {
        resolve(cache, node->bodyLocation.value().file);
    }

    for (auto& ref : node->baseClasses) {
        resolve(cache, ref);
    }

    for (auto& ref : node->derivedClasses) {
        resolve(cache, ref);
    }

    for (auto& ref : node->inners) {
        resolve(cache, ref);
        if (ref.resolved()) {
            node->children.push_back(ref.asNode().lock());

            // Groups should not own their children
            if (node->kind == Kind::MODULE) {
                node->children.back()->group = NodeWeakPtr{node};

                // Assing parent to group only if the child is a group!
                // Otherwise we would steal the pointer from it's real parent.
                if (node->children.back()->kind == Kind::MODULE) {
                    node->children.back()->parent = NodeWeakPtr{node};
                }
            }
            // Files should not own their nodes
            else if (node->kind == Kind::FILE) {
                // Skip
            }
            // Must be a class, namespace, etc.
            else {
                node->children.back()->parent = NodeWeakPtr{node};
            }
        }
    }
    node->inners.clear();

    for (const auto& child : node->children) {
        resolveReferences(cache, child);
    }
}

void Doxygen::resolveHierarchy(const NodeSharedPtr& index) {
    // Remove nodes from index of which parent pointer has been updated
    std::vector<NodeSharedPtr> temp;
    std::swap(temp, index->children);

    for (auto& child : temp) {
        if (child->parent.expired()) {
            index->children.push_back(child);
        }
    }
}

static std::string anchorMaker(const Node& node) {
    if (!isKindStructured(node.kind) && node.kind != Kind::MODULE) {
        return "#" + toLower(toStr(node.kind)) + "-" + safeAnchorId(node.name);
    }
    return std::string("");
}

static const auto urlFolderMaker(const Config& config, const Node& node) {
    if (config.useFolders) {
        return config.baseUrl + typeToFolderName(config, node.kind) + "/";
    }
    return config.baseUrl;
};

std::string Doxygen::makeUrl(const Config& config, const Node& node) {
    switch (node.kind) {
    case Kind::INDEX: {
        return "";
    }
    case Kind::STRUCT:
    case Kind::CLASS:
    case Kind::NAMESPACE:
    case Kind::MODULE:
    case Kind::DIR:
    case Kind::FILE:
    case Kind::PAGE:
    case Kind::INTERFACE:
    case Kind::EXAMPLE:
    case Kind::UNION: {
        if (node.refid == config.mainPageName) {
            if (config.mainPageInRoot) {
                return config.baseUrl;
            } else {
                return urlFolderMaker(config, node);
            }
        }
        return urlFolderMaker(config, node) + stripAnchor(node.refid) + config.linkSuffix + anchorMaker(node);
    }
    case Kind::ENUMVALUE: {
        const auto n = node.parent.lock();
        if (!n) {
            EXCEPTION("Enum value '{}' does not have a parent, this should not happen!", node.name);
        }
        const auto np = n->parent.lock();
        if (!np) {
            EXCEPTION("Enum '{}' does not have a parent, this should not happen!", n->name);
        }
        return urlFolderMaker(config, *np) + stripAnchor(np->refid) + config.linkSuffix + anchorMaker(node);
    }
    default: {
        auto n = node.parent.lock();
        if (!n) {
            EXCEPTION("Node '{}' does not have a parent, this should not happen!", node.name);
        }
        if (!node.group.expired()) {
            n = node.group.lock();
        }
        return urlFolderMaker(config, *n) + stripAnchor(n->refid) + config.linkSuffix + anchorMaker(node);
    }
    }
}

static void resolveUrl(const Config& config, const NodeSharedPtr& node) {
    node->url = Doxygen::makeUrl(config, *node);
    for (const auto& child : node->children) {
        if (child->parent.lock() == node) {
            resolveUrl(config, child);
        }
    }
}

// See definition of TextPair struct for more info
static void resolveText(const Text::MarkdownOptions& options, TextPair& pair) {
    if (!pair.markdown.empty()) {
        return;
    }
    pair.markdown = Text::printMarkdown(pair.node);
}

static void resolveText(const Text::MarkdownOptions& options, const NodeSharedPtr& node) {
    resolveText(options, node->brief);
    resolveText(options, node->title);
    resolveText(options, node->properties.type);
    resolveText(options, node->properties.initializer);
    for (auto& param : node->properties.params) {
        resolveText(options, param.defval);
        resolveText(options, param.type);
    }
    for (auto& param : node->properties.templateParams) {
        resolveText(options, param.defval);
        resolveText(options, param.type);
    }

    for (const auto& child : node->children) {
        if (child->parent.lock() == node) {
            resolveText(options, child);
        }
    }
}

static Text::MarkdownOptions getMarkdownOptions(const Config& config, const Cache& cache) {
    Text::MarkdownOptions options;
    options.dir = config.outputDir;
    options.resolver = [&cache](const std::string& refid) -> std::optional<std::string> {
        const auto found = cache.findByRef(refid);
        if (found) {
            return found->url;
        }
        return std::nullopt;
    };
    return options;
}

void Doxygen::finalize(const Config& config, const Cache& cache, const NodeSharedPtr& node) {
    Log::i("Finalizing...");

    const auto markdownOptions = getMarkdownOptions(config, cache);
    for (const auto& child : node->children) {
        resolveUrl(config, child);
    }

    Log::i("Resolving text...");
    for (const auto& child : node->children) {
        resolveText(markdownOptions, child);
    }
}

NodeSharedPtr Doxygen::parseIndex(const std::filesystem::path& path) {
    const auto indexPath = path / std::filesystem::path("index.xml");
    Log::i("Loading: {}", indexPath.string());

    // Get the root element
    const Xml xml(indexPath);
    const auto root = xml.root();

    auto index = std::make_shared<Node>();
    index->kind = Kind::INDEX;

    // Find all compound nodes from the root
    allOf(root, "compound", [&](Xml::Element& compound) {
        // Only allow compound nodes that we can recognize
        const auto kind = toEnumKind(compound.getAttr("kind", "unknown"));
        const auto refid = compound.getAttr("refid");
        index->children.push_back(parse(path, refid));
    });

    return index;
}

NodeSharedPtr Doxygen::parse(const std::filesystem::path& path, const std::string& refid) {
    const auto filePath = path / std::filesystem::path(refid + ".xml");
    Log::i("Loading: {}", filePath.string());

    // Get the root element
    const Xml xml(filePath);
    const auto root = xml.root();
    auto compound = parse(root);

    for (auto& child : compound->children) {
        child->parent = compound;
    }
    return compound;
}

NodeSharedPtr Doxygen::parse(const Xml::Element& root) {
    const auto compounddef = assertFind(root, "compounddef");
    return parseCompound(compounddef);
}

NodeSharedPtr Doxygen::parseCompound(const Xml::Element& compound) {
    auto ptr = std::make_shared<Node>();
    auto& node = *ptr;

    node.refid = parseRefid(compound);
    node.kind = parseKind(compound);
    node.name = parseCompoundName(compound);
    node.title = parseTitle(compound);
    if (node.title == TextNode{}) {
        node.title = node.name;
    }
    node.includes = parseIncludes(compound);
    node.location = parseLocation(compound);
    node.bodyLocation = parseBodyLocation(compound);
    node.brief = parseBrief(compound);
    concat(node.inners, parseInnerClasses(compound));
    concat(node.inners, parseInnerNamespaces(compound));
    concat(node.inners, parseInnerGroups(compound));
    concat(node.inners, parseInnerFiles(compound));
    concat(node.inners, parseInnerDirectories(compound));
    concat(node.inners, parseInnerPages(compound));
    node.baseClasses = parseBaseCompound(compound);
    node.derivedClasses = parseDerivedCompound(compound);

    parseProperties(compound, node.properties);

    allOf(compound, "sectiondef", [&](Xml::Element& section) {
        allOf(section, "memberdef", [&](Xml::Element& memberdef) {
            // Groups should not own their nodes
            // nor they shall create copies!
            // We will save it as a reference, then solve this reference later.
            // By doing this, groups will have children which in reality belong
            // to a different parent. This will solve URL issues and duplicates issues!
            if (node.kind == Kind::MODULE) {
                const auto refid = parseRefid(memberdef);
                node.inners.push_back(NodeRef{BasicRef{refid, ""}});
                return;
            }

            auto child = parseMember(memberdef);
            node.children.push_back(std::move(child));
        });
    });

    return ptr;
}

NodeSharedPtr Doxygen::parseMember(const Xml::Element& memberdef) {
    auto ptr = std::make_shared<Node>();
    auto& node = *ptr;

    node.refid = parseRefid(memberdef);

    if (node.refid == "namespace_engine_1_1_audio_1a9527f056637d57771ee8025589fd596d") {
        Log::i("break");
    }

    node.kind = parseKind(memberdef);
    node.name = parseName(memberdef);
    node.title = parseTitle(memberdef);
    if (node.title == TextNode{}) {
        node.title = node.name;
    }
    node.location = parseLocation(memberdef);
    node.bodyLocation = parseBodyLocation(memberdef);
    node.brief = parseBrief(memberdef);

    parseProperties(memberdef, node.properties);
    parseDetailsNoText(memberdef, node.properties);

    if (node.kind == Kind::ENUM) {
        auto values = parseEnumValues(memberdef);
        for (const auto& value : values) {
            value->parent = ptr;
        }
        std::swap(node.children, values);
    }

    if (node.kind == Kind::TYPEDEF) {
        node.properties.type.node += node.properties.argsString;
    }

    return ptr;
}

void Doxygen::parseProperties(const Xml::Element& elm, Properties& properties) {
    properties.templateParams = parseTemplateParams(elm);
    properties.params = parseParams(elm);

    properties.visibility = parseProt(elm);
    properties.virt = parseVirt(elm);

    properties.type = parseType(elm);
    properties.definition = parseDefinition(elm);
    properties.argsString = parseArgsString(elm);
    properties.initializer = parseInitializer(elm);

    properties.isAbstract = parseBoolAttr(elm, "abstract");
    properties.isStatic = parseBoolAttr(elm, "static");
    properties.isConst = parseBoolAttr(elm, "const");
    properties.isExplicit = parseBoolAttr(elm, "explicit");
    properties.isStrong = parseBoolAttr(elm, "strong");
    properties.isInline = parseBoolAttr(elm, "inline");
    properties.isDefault = properties.argsString.find("=default") != std::string::npos;
    properties.isDeleted = properties.argsString.find("=deleted") != std::string::npos;
    properties.isOverride = properties.argsString.find(" override") != std::string::npos;
    properties.isFinal = parseBoolAttr(elm, "final");
    properties.isSealed = parseBoolAttr(elm, "sealed");
}

void Doxygen::parseDetailsNoText(const Xml::Element& memberdef, Properties& properties) {
    const auto details = memberdef.firstChildElement("detaileddescription");
    if (details) {
        allOf(details, "para", [&](Xml::Element& para) {
            allOf(para, "xrefsect", [&](Xml::Element& xrefsect) {
                const auto id = xrefsect.getAttr("id", "");
                const auto pos = id.find('_');
                if (pos != std::string::npos) {
                    const auto sectionType = id.substr(0, pos);

                    if (sectionType == "deprecated") {
                        properties.isDeprecated = true;
                    }
                }
            });
        });
    }
}

std::string Doxygen::parseRefid(const Xml::Element& elm) {
    return elm.getAttr("id");
}

Kind Doxygen::parseKind(const Xml::Element& elm) {
    return toEnumKind(elm.getAttr("kind"));
}

Visibility Doxygen::parseProt(const Xml::Element& elm) {
    return toEnumVisibility(elm.getAttr("prot", "public"));
}

Virtual Doxygen::parseVirt(const Xml::Element& elm) {
    return toEnumVirtual(elm.getAttr("virt", "non-virtual"));
}

std::string Doxygen::parseCompoundName(const Xml::Element& elm) {
    const auto compoundname = assertFind(elm, "compoundname");
    return compoundname.getText();
}

std::vector<NodeSharedPtr> Doxygen::parseEnumValues(const Xml::Element& memberdef) {
    std::vector<NodeSharedPtr> values;
    allOf(memberdef, "enumvalue", [&](Xml::Element& enumvalue) { values.push_back(parseEnumValue(enumvalue)); });
    return values;
}

NodeSharedPtr Doxygen::parseEnumValue(const Xml::Element& enumvalue) {
    NodeSharedPtr node = std::make_shared<Node>();
    node->kind = Kind::ENUMVALUE;
    node->refid = parseRefid(enumvalue);
    node->name = parseName(enumvalue);
    node->brief = parseBrief(enumvalue);

    parseProperties(enumvalue, node->properties);

    return node;
}

Includes Doxygen::parseIncludes(const Xml::Element& elm) {
    Includes includes;

    allOf(elm, "includes", [&](Xml::Element& inc) {
        const auto refid = inc.getAttr("refid", "");
        const auto isLocal = inc.getAttr("local", "no") == "yes";
        const auto name = inc.getText();

        Include include{NodeRef(BasicRef{refid, name}), isLocal};
        includes.push_back(std::move(include));
    });

    return includes;
}

Params Doxygen::parseTemplateParams(const Xml::Element& elm) {
    Params params;

    const auto templateparamlist = elm.firstChildElement("templateparamlist");
    if (!templateparamlist) {
        return params;
    }

    allOf(templateparamlist, "param", [&](Xml::Element& param) { params.push_back(parseParam(param)); });

    return params;
}

Params Doxygen::parseParams(const Xml::Element& elm) {
    Params params;

    allOf(elm, "param", [&](Xml::Element& param) { params.push_back(parseParam(param)); });

    return params;
}

Param Doxygen::parseParam(const Xml::Element& elm) {
    const auto type = elm.firstChildElement("type");
    const auto declname = elm.firstChildElement("declname");
    const auto defval = elm.firstChildElement("defval");
    const auto arr = elm.firstChildElement("array");

    Param param;
    param.name = "";

    if (type) {
        param.type = Text::parse(type);
    }
    if (declname && declname.hasText()) {
        param.name = declname.getText();
    }
    if (arr && arr.hasText()) {
        param.name += arr.getText();
    }
    if (defval) {
        param.defval = Text::parse(defval);
    }

    return param;
}

std::string Doxygen::parseName(const Xml::Element& elm) {
    const auto name = assertFind(elm, "name");
    if (name.hasText()) {
        return name.getText();
    }
    return "";
}

std::string Doxygen::parseTitle(const Xml::Element& elm) {
    const auto title = elm.firstChildElement("title");
    if (title && title.hasText()) {
        return title.getText();
    }
    return "";
}

std::optional<Location> Doxygen::parseLocation(const Xml::Element& elm) {
    const auto location = elm.firstChildElement("location");
    if (!location) {
        return std::nullopt;
    }

    const auto file = location.getAttr("file", "");
    if (file.empty()) {
        return std::nullopt;
    }

    Location loc;

    loc.file = NodeRef(BasicRef{"", file});
    loc.line = std::stol(location.getAttr("line", "0"));
    loc.column = std::stol(location.getAttr("column", "0"));

    return {loc};
}

std::optional<BodyLocation> Doxygen::parseBodyLocation(const Xml::Element& elm) {
    const auto location = elm.firstChildElement("location");
    if (!location) {
        return std::nullopt;
    }

    const auto file = location.getAttr("bodyfile", "");
    if (file.empty()) {
        return std::nullopt;
    }

    BodyLocation loc;

    loc.file = NodeRef(BasicRef{"", file});
    loc.start = std::stol(location.getAttr("bodystart", "0"));
    loc.end = std::stol(location.getAttr("bodyend", "0"));

    return {loc};
}

NodeRef Doxygen::parseRef(const Xml::Element& elm) {
    const auto refid = elm.getAttr("refid", "");
    std::string name;
    if (elm.hasText()) {
        name = elm.getText();
    }

    return NodeRef{BasicRef{refid, name}};
}

static std::vector<NodeRef> parseInner(const Xml::Element& elm, const std::string& name) {
    std::vector<NodeRef> inner;
    allOf(elm, name, [&](Xml::Element& ref) { inner.push_back(Doxygen::parseRef(ref)); });
    return inner;
}

std::vector<NodeRef> Doxygen::parseInnerClasses(const Xml::Element& elm) {
    return parseInner(elm, "innerclass");
}

std::vector<NodeRef> Doxygen::parseInnerNamespaces(const Xml::Element& elm) {
    return parseInner(elm, "innernamespace");
}

std::vector<NodeRef> Doxygen::parseInnerFiles(const Xml::Element& elm) {
    return parseInner(elm, "innerfile");
}

std::vector<NodeRef> Doxygen::parseInnerDirectories(const Xml::Element& elm) {
    return parseInner(elm, "innerdir");
}

std::vector<NodeRef> Doxygen::parseInnerPages(const Xml::Element& elm) {
    return parseInner(elm, "innerpage");
}

std::vector<NodeRef> Doxygen::parseInnerGroups(const Xml::Element& elm) {
    return parseInner(elm, "innergroup");
}

std::vector<NodeRef> Doxygen::parseBaseCompound(const Xml::Element& elm) {
    return parseInner(elm, "basecompoundref");
}

std::vector<NodeRef> Doxygen::parseDerivedCompound(const Xml::Element& elm) {
    return parseInner(elm, "derivedcompoundref");
}

bool Doxygen::parseBoolAttr(const Xml::Element& elm, const std::string& key) {
    return elm.getAttr(key, "no") == "yes";
}

std::string Doxygen::parseTextBody(const Xml::Element& elm, const std::string& key) {
    const auto child = elm.firstChildElement(key);
    if (child && child.hasText()) {
        return child.getText();
    }

    return "";
}

std::string Doxygen::parseArgsString(const Xml::Element& elm) {
    return parseTextBody(elm, "argsstring");
}

std::string Doxygen::parseDefinition(const Xml::Element& elm) {
    return parseTextBody(elm, "definition");
}

TextNode Doxygen::parseTextNode(const Xml::Element& elm, const std::string& key) {
    const auto child = elm.firstChildElement(key);
    if (child) {
        return Text::parse(child);
    }

    return "";
}

TextNode Doxygen::parseType(const Xml::Element& elm) {
    return parseTextNode(elm, "type");
}

TextNode Doxygen::parseInitializer(const Xml::Element& elm) {
    return parseTextNode(elm, "initializer");
}

TextNode Doxygen::parseBrief(const Xml::Element& elm) {
    return parseTextNode(elm, "briefdescription");
}
