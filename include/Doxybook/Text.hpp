#pragma once

#include "Xml.hpp"
#include <filesystem>
#include <map>
#include <nlohmann/json.hpp>
#include <optional>
#include <variant>

namespace Doxybook2 {
namespace Text {
enum class Type {
    Plain,
    Text,
    Paragraph,
    Document,
    Section,
    Bold,
    Italics,
    StrikeThrough,
    UrlLink,
    RefLink,
    ImageLink,
    HRuler,
    ItemizedList,
    OrderedList,
    ListItem,
    Monospaced,
    CodeBlock,
    Table,
    TableRow,
    TableCell
};

extern std::string toStr(Type type);

inline void to_json(nlohmann::json& j, const Type& p) {
    j = toStr(p);
}

using Plain = std::string;

struct Node;
struct NodeCompound;

struct Node {
    Type type;

    bool operator==(const Text::Node& b) const;
    [[nodiscard]] nlohmann::json json() const;
};

using Path = std::filesystem::path;
using RefidResolver = std::function<std::optional<std::string>(const std::string&)>;

struct MarkdownOptions {
    Path dir = "";
    RefidResolver resolver = [](const std::string&) -> std::optional<std::string> { return std::nullopt; };
};

using NodeVariant = std::variant<Plain, Node, NodeCompound>;

inline void to_json(nlohmann::json& j, const NodeVariant& p);

inline bool operator==(const std::vector<NodeVariant>& a, const std::vector<NodeVariant>& b);

struct NodeCompound {
    Type type;
    std::map<std::string, NodeVariant> properties;
    std::vector<NodeVariant> children;

    bool operator==(const Text::NodeCompound& b) const;
    [[nodiscard]] nlohmann::json json() const;
};

inline void to_json(nlohmann::json& j, const NodeVariant& p) {
    switch (p.index()) {
    case 0: {
        j = std::get<0>(p);
        break;
    }
    case 1: {
        j = std::get<1>(p).json();
        break;
    }
    case 2: {
        j = std::get<2>(p).json();
        break;
    }
    default: {
        j = nullptr;
        break;
    }
    }
}

inline bool operator==(const std::vector<NodeVariant>& a, const std::vector<NodeVariant>& b) {
    if (a.size() != b.size()) {
        return false;
    }

    for (size_t i = 0; i < a.size(); i++) {
        if (!(a[i] == b[i])) {
            return false;
        }
    }

    return true;
}

extern NodeVariant parse(const Xml::Element& elm);
extern NodeVariant parsePara(const Xml::Element& elm);
extern NodeVariant parseText(const Xml::Element& elm);
extern NodeVariant parseDecorative(const Xml::Element& elm);
extern NodeVariant parseHRuler(const Xml::Element& elm);
extern NodeVariant parseImage(const Xml::Element& elm);
extern NodeVariant parseRef(const Xml::Element& elm);
extern NodeVariant parseSection(const Xml::Element& elm);
extern NodeVariant parseMaybePlain(const Xml::Node& elm);
extern NodeVariant parseList(const Xml::Element& elm);
extern NodeVariant parseListItem(const Xml::Element& elm);
extern NodeVariant parseUrlLink(const Xml::Element& elm);
extern NodeVariant parseCodeListing(const Xml::Element& elm);
extern NodeVariant parseTable(const Xml::Element& elm);
extern NodeVariant parseTableRow(const Xml::Element& elm);
extern NodeVariant parseTableEntry(const Xml::Element& elm);
extern void parseRecursively(std::vector<NodeVariant>& tree, const Xml::Node& elm);
extern void parseChildrenOf(std::vector<NodeVariant>& tree, const Xml::Element& elm);
extern std::string printMarkdown(const NodeVariant& node, const MarkdownOptions& options = {});

inline std::ostream& operator<<(std::ostream& os, const NodeCompound& value) {
    os << value.json().dump();
    return os;
}

template <typename Variant> inline void print(std::ostream& os, const Variant& value, size_t i) {
    os << "{?}";
}

template <typename Variant, typename T, typename... Ts>
inline void print(std::ostream& os, const Variant& value, size_t i) {
    if (value.index() == i) {
        os << std::get<T>(value);
    } else {
        print<Variant, Ts...>(os, value, i + 1);
    }
}

inline std::ostream& operator<<(std::ostream& os, const NodeVariant& value) {
    print<NodeVariant, Plain, Node, NodeCompound>(os, value, 0);
    return os;
}

} // namespace Text

using TextNode = Text::NodeVariant;

inline bool operator==(const TextNode& a, const Text::Plain& b) {
    if (a.index() != 0) {
        return false;
    }

    return std::get<0>(a) == b;
}

inline bool operator==(const TextNode& a, const Text::Node& b) {
    if (a.index() != 1) {
        return false;
    }

    return std::get<1>(a) == b;
}

inline bool operator==(const TextNode& a, const Text::NodeCompound& b) {
    if (a.index() != 2) {
        return false;
    }

    return std::get<2>(a) == b;
}

inline std::ostream& operator<<(std::ostream& os, const TextNode& value) {
    return Text::operator<<(os, value);
}
} // namespace Doxybook2
