#include "Log.hpp"
#include "Macros.hpp"
#include <Doxybook/Doxygen.hpp>
#include <Doxybook/Enums.hpp>
#include <Doxybook/Exception.hpp>
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

static std::optional<Kind> tryKind(const std::string& str) {
    try {
        return {toEnumKind(str)};
    } catch (std::exception& e) {
        return std::nullopt;
    }
}

static Xml::Element assertFind(const Xml::Element& elm, const std::string& name) {
    const auto child = elm.firstChildElement(name);
    if (!child) {
        const auto& path = elm.getDocument().getPath();
        throw Exception(LOCATION, "Unable to find element '{}' in file '{}'", name, path.string());
    }

    return child;
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
        const auto kind = tryKind(compound.getAttr("kind", "unknown"));
        if (!kind.has_value()) {
            return;
        }

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
    const auto compounddef = assertFind(root, "compounddef");

    return parseCompound(compounddef);
}

NodeSharedPtr Doxygen::parseCompound(const Xml::Element& compound) {
    auto ptr = std::make_shared<Node>();
    auto& node = *ptr;

    node.refid = parseRefid(compound);
    node.kind = parseKind(compound);
    node.name = parseCompoundName(compound);
    node.includes = parseIncludes(compound);
    node.location = parseLocation(compound);
    node.bodyLocation = parseBodyLocation(compound);

    parseProperties(compound, node.properties);

    allOf(compound, "sectiondef", [&](Xml::Element& section) {
        allOf(section, "memberdef", [&](Xml::Element& memberdef) {
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
    node.kind = parseKind(memberdef);
    node.name = parseName(memberdef);
    node.location = parseLocation(memberdef);
    node.bodyLocation = parseBodyLocation(memberdef);

    parseProperties(memberdef, node.properties);

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

    Param param;

    if (type && type.hasText()) {
        param.type = type.getText();
    }
    if (declname && declname.hasText()) {
        param.name = declname.getText();
    }
    if (defval && defval.hasText()) {
        param.defval = defval.getText();
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

std::optional<Location> Doxygen::parseLocation(const Xml::Element& elm) {
    const auto location = elm.firstChildElement("location");
    if (!location) {
        return std::nullopt;
    }

    const auto file = location.getAttr("bodyfile", "");
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
