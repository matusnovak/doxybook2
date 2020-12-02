#include "Log.hpp"
#include "Macros.hpp"
#include <Doxybook/Enums.hpp>
#include <Doxybook/Exception.hpp>
#include <Doxybook/Parser.hpp>
#include <Doxybook/Xml.hpp>
#include <optional>

using namespace Doxybook2;

template <typename Fn> void allOf(const Xml::Element& elm, const std::string& name, const Fn& fn) {
    auto child = elm.firstChildElement("compound");
    while (child) {
        fn(child);
        child = child.nextSiblingElement("compound");
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

Parser::Parser(const std::filesystem::path& path) {
    const auto indexPath = path / std::filesystem::path("index.xml");
    Log::i("Loading: {}", indexPath.string());

    // Get the root element
    const Xml xml(indexPath);
    const auto root = xml.root();

    // Find all compound nodes from the root
    allOf(root, "compound", [&](Xml::Element& compound) {
        // Only allow compound nodes that we can recognize
        const auto kind = tryKind(compound.getAttr("kind", "unknown"));
        if (!kind.has_value()) {
            return;
        }

        const auto refid = compound.getAttr("refid");
        this->load(path, refid);
    });
}

void Parser::load(const std::filesystem::path& path, const std::string& refid) {
    const auto filePath = path / std::filesystem::path(refid + ".xml");
    Log::i("Loading: {}", filePath.string());

    // Get the root element
    const Xml xml(filePath);
    const auto root = xml.root();
    const auto compounddef = assertFind(root, "compounddef");

    index.children.push_back(parseCompound(compounddef));
}

NodeSharedPtr Parser::parseCompound(const Xml::Element& compound) const {
    auto ptr = std::make_shared<Node>();
    auto& node = *ptr;

    node.kind = parseKind(compound);
    node.name = parseCompoundName(compound);

    return ptr;
}

Kind Parser::parseKind(const Xml::Element& elm) const {
    return toEnumKind(elm.getAttr("kind"));
}

Visibility Parser::parseProt(const Xml::Element& elm) const {
    return toEnumVisibility(elm.getAttr("prot"));
}

std::string Parser::parseCompoundName(const Xml::Element& elm) const {
    const auto compoundname = assertFind(elm, "compoundname");
    return compoundname.getText();
}

Includes Parser::parseIncludes(const Xml::Element& elm) const {
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

Params Parser::parseTemplateParams(const Xml::Element& elm) const {
    Params params;

    const auto templateparamlist = assertFind(elm, "templateparamlist");
    allOf(templateparamlist, "param", [&](Xml::Element& param) { params.push_back(parseParam(param)); });

    return params;
}

Param Parser::parseParam(const Xml::Element& elm) const {
}