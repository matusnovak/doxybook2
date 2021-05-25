#pragma once

#include "Enums.hpp"
#include "Text.hpp"
#include <memory>
#include <optional>
#include <variant>

namespace Doxybook2 {
struct Node;

using NodeWeakPtr = std::weak_ptr<Node>;
using NodeSharedPtr = std::shared_ptr<Node>;

struct BasicRef {
    std::string refid;
    std::string name;
};

class NodeRef {
public:
    NodeRef() = default;

    explicit NodeRef(const BasicRef& basic) {
        value = basic;
    }

    [[nodiscard]] bool empty() const {
        return value.index() == 0;
    }

    [[nodiscard]] bool resolved() const {
        return value.index() == 2;
    }

    [[nodiscard]] const BasicRef& asBasicRef() const {
        return std::get<1>(value);
    }

    [[nodiscard]] const NodeWeakPtr& asNode() const {
        return std::get<2>(value);
    }

    void resolve(const NodeSharedPtr& ptr) {
        value = NodeWeakPtr{ptr};
    }

private:
    std::variant<std::nullptr_t, BasicRef, NodeWeakPtr> value{nullptr};
};

struct Include {
    NodeRef node;
    bool isLocal{false};
};

using Includes = std::vector<Include>;

struct Location {
    NodeRef file;
    int line{0};
    int column{0};
};

struct BodyLocation {
    NodeRef file;
    int start{0};
    int end{0};
};

// This holds the intermediate text representation of some properties of Node class.
// This could be the parameter description, return type, etc.
// We can not resolve some of the internal links that may occur inside of the text.
// What if the text references some other node? We won't know its exact URL until a
// much later stage when we have built the cache and solved hierarchy issues.
// After that, this TextPair will be "resolved" by printing the "node"
// into plain and markdown representations.
struct TextPair {
    TextPair() = default;

    explicit TextPair(TextNode node) : node(std::move(node)) {
    }

    explicit TextPair(const std::string& other) : node(other) {
    }

    TextPair& operator=(const TextNode& other) {
        node = other;
        return *this;
    }

    TextPair& operator=(TextNode&& other) {
        node = std::move(other);
        return *this;
    }

    TextPair& operator=(const std::string& other) {
        node = other;
        return *this;
    }

    TextNode node;
    std::string plain;
    std::string markdown;

    [[nodiscard]] bool operator==(const TextNode& other) const {
        return node == other;
    }

    [[nodiscard]] bool empty() const {
        return plain.empty() && markdown.empty();
    }
};

struct Param {
    TextPair type;
    std::string name;
    TextPair defval;
};

typedef std::vector<Param> Params;

struct Properties {
    TextPair type;
    std::string definition;
    std::string argsString;
    TextPair initializer;
    bool isAbstract{false};
    bool isStatic{false};
    bool isConst{false};
    bool isExplicit{false};
    bool isStrong{false};
    bool isInline{false};
    bool isDefault{false};
    bool isDeleted{false};
    bool isOverride{false};
    bool isFinal{false};
    bool isSealed{false};
    bool isDeprecated{false};
    Params params;
    Params templateParams;
    Visibility visibility;
    Virtual virt;
};

struct Node {
    Kind kind;

    std::string refid;
    std::string name;
    TextPair title;
    TextPair brief;

    Properties properties;

    // We can not have shared pointers pointing to each other...
    NodeWeakPtr parent;
    NodeWeakPtr group;

    Includes includes;
    std::vector<NodeSharedPtr> children;
    std::vector<NodeRef> inners;
    std::vector<NodeRef> baseClasses;
    std::vector<NodeRef> derivedClasses;
    std::optional<Location> location;
    std::optional<BodyLocation> bodyLocation;

    std::string url;
};
} // namespace Doxybook2
