#include <Doxybook/Log.hpp>
#include <Doxybook/Text.hpp>
#include <catch2/catch.hpp>
#include <sstream>

using namespace Doxybook2;

std::string Text::toStr(const Type type) {
    switch (type) {
    case Type::Plain:
        return "plain";
    case Type::Paragraph:
        return "paragraph";
    case Type::Text:
        return "text";
    case Type::Document:
        return "document";
    case Type::Section:
        return "section";
    case Type::Bold:
        return "bold";
    case Type::Italics:
        return "italics";
    case Type::StrikeThrough:
        return "strike-through";
    case Type::UrlLink:
        return "url";
    case Type::RefLink:
        return "ref";
    case Type::ImageLink:
        return "image";
    case Type::HRuler:
        return "hruler";
    case Type::ItemizedList:
        return "itemized-list";
    case Type::OrderedList:
        return "ordered-list";
    case Type::ListItem:
        return "list-item";
    case Type::Monospaced:
        return "monospaced";
    case Type::CodeBlock:
        return "code";
    case Type::Table:
        return "table";
    case Type::TableRow:
        return "table-row";
    case Type::TableCell:
        return "table-cell";
    default:
        return "unknown";
    }
}

using Callback = std::function<Text::NodeVariant(const Xml::Element&)>;

Callback strToType(const std::string& str) {
    static std::unordered_map<std::string, Callback> kinds = {
        {"briefdescription", Text::parseText},
        {"para", Text::parsePara},
        {"bold", Text::parseDecorative},
        {"emphasis", Text::parseDecorative},
        {"strike", Text::parseDecorative},
        {"hruler", Text::parseHRuler},
        {"image", Text::parseImage},
        {"ulink", Text::parseUrlLink},
        {"ref", Text::parseRef},
        {"listitem", Text::parseText},
        {"itemizedlist", Text::parseList},
        {"variablelist", Text::parseText},
        {"orderedlist", Text::parseList},
        {"varlistentry", Text::parseText},
        {"term", Text::parseText},
        {"anchor", Text::parseText},
        {"simplesect", Text::parseText},
        {"computeroutput", Text::parseDecorative},
        {"parameterdescription", Text::parseText},
        {"parametername", Text::parseText},
        {"parameterlist", Text::parseText},
        {"parameteritem", Text::parseText},
        {"parameternamelist", Text::parseText},
        {"type", Text::parseText},
        {"argsstring", Text::parseText},
        {"defval", Text::parseText},
        {"declname", Text::parseText},
        {"xrefsect", Text::parseText},
        {"xreftitle", Text::parseText},
        {"xrefdescription", Text::parseText},
        {"initializer", Text::parseText},
        {"programlisting", Text::parseCodeListing},
        {"codeline", Text::parseText},
        {"sp", Text::parseText},
        {"highlight", Text::parseText},
        {"defname", Text::parseText},
        {"title", Text::parseText},
        {"sect1", Text::parseSection},
        {"sect2", Text::parseSection},
        {"sect3", Text::parseSection},
        {"sect4", Text::parseSection},
        {"sect5", Text::parseSection},
        {"sect6", Text::parseSection},
        {"heading", Text::parseText},
        {"superscript", Text::parseText},
        {"nonbreakablespace", Text::parseText},
        {"table", Text::parseTable},
        {"row", Text::parseTableRow},
        {"entry", Text::parseText},
        {"verbatim", Text::parseText},
        {"lsquo", Text::parseText},
        {"linebreak", Text::parseText},
        {"ndash", Text::parseText},
        {"mdash", Text::parseText},
        {"onlyfor", Text::parseText},
    };

    const auto it = kinds.find(str);
    if (it == kinds.end()) {
        Log::w("Text tag \"{}\" not recognised, please contact the author", str);
        return Text::parseText;
    }

    return it->second;
}

bool Text::Node::operator==(const Text::Node& b) const {
    return type == b.type;
}

nlohmann::json Text::Node::json() const {
    return nlohmann::json::object({{"type", type}});
}

bool Text::NodeCompound::operator==(const Text::NodeCompound& b) const {
    if (type != b.type) {
        return false;
    }

    if (properties != b.properties) {
        return false;
    }

    return children == b.children;
}

nlohmann::json Text::NodeCompound::json() const {
    nlohmann::json j = nlohmann::json::object({{"type", type}});
    if (children.size() == 1 && children[0].index() == 0) {
        j["children"] = std::get<0>(children[0]);
    } else if (!children.empty()) {
        j["children"] = children;
    }

    for (const auto& pair : properties) {
        j[pair.first] = pair.second;
    }

    return j;
}

template <typename Fn> void allOf(const Xml::Element& elm, const std::string& name, const Fn& fn) {
    auto child = elm.firstChildElement(name);
    while (child) {
        fn(child);
        child = child.nextSiblingElement(name);
    }
}

Text::NodeVariant Text::parse(const Xml::Element& elm) {
    std::vector<NodeVariant> children;

    parseRecursively(children, elm.asNode());

    if (children.empty()) {
        return "";
    }

    NodeVariant res;
    std::swap(res, children.front());

    if (res.index() == 2) {
        if (const auto& c = std::get<2>(res); c.children.empty() && c.type == Type::Text) {
            return "";
        }
    }

    return res;
}

Text::NodeVariant Text::parsePara(const Xml::Element& elm) {
    NodeVariant compound = NodeCompound{Type::Paragraph};

    parseChildrenOf(std::get<NodeCompound>(compound).children, elm);

    return compound;
}

Text::NodeVariant Text::parseText(const Xml::Element& elm) {
    NodeVariant compound = NodeCompound{Type::Text};

    parseChildrenOf(std::get<NodeCompound>(compound).children, elm);

    if (const auto& c = std::get<NodeCompound>(compound); c.children.size() == 1 && c.children[0].index() == 0) {
        return c.children[0];
    }

    return compound;
}

void Text::parseRecursively(std::vector<NodeVariant>& tree, const Xml::Node& elm) {
    if (!elm) {
        return;
    }

    if (elm.isElement()) {
        const auto& e = elm.asElement();
        const auto& func = strToType(e.getName());

        if (func) {
            tree.push_back(func(e));
        }
    } else {
        if (elm.hasText()) {
            const auto text = elm.getText();
            if (!text.empty()) {
                tree.emplace_back(Plain{elm.getText()});
            }
        }
    }
}

void Text::parseChildrenOf(std::vector<NodeVariant>& tree, const Xml::Element& elm) {
    auto child = elm.firstChild();
    while (child) {
        parseRecursively(tree, child);
        child = child.nextSibling();
    }
}

Text::NodeVariant Text::parseUrlLink(const Xml::Element& elm) {
    NodeCompound node{Type::UrlLink};
    node.properties["url"] = elm.getAttr("url", "");
    parseChildrenOf(node.children, elm);
    return node;
}

Text::NodeVariant Text::parseDecorative(const Xml::Element& elm) {
    NodeCompound node{Type::Text};

    const auto name = elm.getName();
    if (name == "bold") {
        node.type = Type::Bold;
    } else if (name == "emphasis") {
        node.type = Type::Italics;
    } else if (name == "strike") {
        node.type = Type::StrikeThrough;
    } else if (name == "computeroutput") {
        node.type = Type::Monospaced;
    }

    parseChildrenOf(node.children, elm);
    return node;
}

Text::NodeVariant Text::parseHRuler(const Xml::Element& elm) {
    return Node{Type::HRuler};
}

Text::NodeVariant Text::parseImage(const Xml::Element& elm) {
    NodeCompound node{Type::ImageLink};
    node.properties["url"] = elm.getAttr("name", "");
    parseChildrenOf(node.children, elm);
    return node;
}

Text::NodeVariant Text::parseRef(const Xml::Element& elm) {
    NodeCompound node{Type::RefLink};
    node.properties["refid"] = elm.getAttr("refid", "");
    parseChildrenOf(node.children, elm);
    return node;
}

Text::NodeVariant Text::parseMaybePlain(const Xml::Node& elm) {
    NodeCompound compound{Type::Text};
    parseRecursively(compound.children, elm);

    if (!compound.children.empty()) {
        return compound.children[0];
    }

    return compound;
}

Text::NodeVariant Text::parseSection(const Xml::Element& elm) {
    NodeCompound node{Type::Section};
    node.properties["id"] = elm.getAttr("id", "");
    node.properties["title"] = "";
    node.properties["level"] = elm.getName();

    auto child = elm.firstChild();
    while (child) {
        if (child.isElement() && child.asElement().getName() == "title") {
            node.properties["title"] = parseMaybePlain(child);
        } else {
            parseRecursively(node.children, child);
        }
        child = child.nextSibling();
    }
    return node;
}

Text::NodeVariant Text::parseList(const Xml::Element& elm) {
    NodeCompound node{Type::ItemizedList};

    const auto name = elm.getName();
    if (name == "orderedlist") {
        node.type = Type::OrderedList;
    }

    allOf(elm, "listitem", [&](const Xml::Element& item) {
        const auto para = item.firstChildElement("para");
        if (para) {
            // parseRecursively(node.children, para.asNode());
            NodeCompound child{Type::ListItem};

            parseChildrenOf(child.children, para);

            node.children.emplace_back(std::move(child));
        }
    });

    return node;
}

Text::NodeVariant Text::parseCodeListing(const Xml::Element& elm) {
    NodeCompound node{Type::CodeBlock};

    const auto filename = elm.getAttr("filename", "");
    if (!filename.empty()) {
        node.properties["filename"] = filename;
        node.properties["lang"] = std::filesystem::path(filename).extension().string().substr(1);
    } else {
        node.properties["lang"] = "cpp";
    }

    const std::function<std::string(const Xml::Node&)> recursively = [&recursively](const Xml::Node& node) {
        std::stringstream ss;
        auto child = node.firstChild();
        while (child) {
            if (child.isElement()) {
                const auto n = child.asElement().getName();
                if (n == "sp") {
                    ss << " ";
                }

                ss << recursively(child);
            } else {
                if (child.hasText()) {
                    ss << child.getText();
                }
            }
            child = child.nextSibling();
        }

        return ss.str();
    };

    auto first = true;
    std::stringstream ss;
    allOf(elm, "codeline", [&](const Xml::Element& item) {
        if (!first) {
            ss << "\n";
        }
        first = false;
        ss << recursively(item.asNode());
    });

    node.children.emplace_back(Plain{ss.str()});
    return node;
}

Text::NodeVariant Text::parseTable(const Xml::Element& elm) {
    NodeCompound node{Type::Table};

    allOf(elm, "row", [&](const Xml::Element& row) { node.children.push_back(parseTableRow(row)); });

    return node;
}

Text::NodeVariant Text::parseTableRow(const Xml::Element& elm) {
    NodeCompound node{Type::TableRow};

    allOf(elm, "entry", [&](const Xml::Element& entry) {
        const auto para = entry.firstChildElement("para");
        if (para) {
            // parseRecursively(node.children, para.asNode());
            node.children.push_back(parseTableEntry(para));
        }
    });

    return node;
}

Text::NodeVariant Text::parseTableEntry(const Xml::Element& elm) {
    NodeCompound node{Type::TableCell};

    parseChildrenOf(node.children, elm);

    return node;
}

static std::string hTypeToMarkdown(const std::string& type) {
    if (type.empty()) {
        return "######";
    }

    const auto idx = type.back() - '0';
    if (idx <= 0 || idx > 6) {
        return "######";
    }

    static const std::string VALUES[6] = {"#", "##", "###", "####", "#####", "######"};

    return VALUES[idx - 1];
}

static void printPlainSafe(std::stringstream& ss, const Text::Plain& node, const Text::MarkdownOptions& options) {
    ss << node; // TODO
}

static bool empty(std::stringstream& ss) {
    return ss.tellp() == std::streampos(0);
}

static void printMarkdownRecursively(std::stringstream& ss, const Text::NodeVariant& node,
                                     const Text::MarkdownOptions& options) {
    using namespace Text;

    switch (node.index()) {
    case 0: {
        printPlainSafe(ss, std::get<0>(node), options);
        break;
    }
    case 1: {
        const auto& n = std::get<1>(node);
        switch (n.type) {
        case Type::HRuler: {
            if (!empty(ss)) {
                ss << "\n\n";
            }
            ss << "--------------------\n\n";
            break;
        }
        default: {
            break;
        }
        }
        break;
    }
    case 2: {
        const auto& n = std::get<2>(node);

        // Before
        switch (n.type) {
        case Type::Section: {
            ss << hTypeToMarkdown(std::get<0>(n.properties.at("level")));
            ss << " ";
            printMarkdownRecursively(ss, n.properties.at("title"), options);
            ss << "\n\n";
            break;
        }
        case Type::RefLink:
            [[fallthrough]];
        case Type::UrlLink: {
            ss << "[";
            break;
        }
        case Type::Italics: {
            ss << "_";
            break;
        }
        case Type::Bold: {
            ss << "**";
            break;
        }
        case Type::TableRow: {
            ss << "| ";
            break;
        }
        default: {
            break;
        }
        }

        // Middle
        for (const auto& c : n.children) {
            printMarkdownRecursively(ss, c, options);
        }

        // After
        switch (n.type) {
        case Type::Paragraph: {
            ss << "\n\n";
            break;
        }
        case Type::RefLink: {
            ss << "](";

            const auto& refid = std::get<Plain>(n.properties.at("refid"));
            const auto resolved = options.resolver(refid);
            if (resolved.has_value()) {
                ss << resolved.value();
            } else {
                ss << "#";
            }

            ss << ")";
            break;
        }
        case Type::UrlLink: {
            ss << "](";
            ss << std::get<Plain>(n.properties.at("url"));
            ss << ")";
            break;
        }
        case Type::Italics: {
            ss << "_";
            break;
        }
        case Type::Bold: {
            ss << "**";
            break;
        }
        case Type::TableCell: {
            ss << " | ";
            break;
        }
        case Type::TableRow: {
            ss << "\n";
            break;
        }
        default: {
            break;
        }
        }

        break;
    }
    default: {
        break;
    }
    }
}

std::string Text::printMarkdown(const NodeVariant& node, const MarkdownOptions& options) {
    std::stringstream ss;

    printMarkdownRecursively(ss, node, options);

    auto md = ss.str();
    if (md.size() >= 2 && md.at(md.size() - 1) == '\n' && md.at(md.size() - 2) == '\n') {
        md = md.substr(0, md.size() - 2);
    }
    return md;
}
