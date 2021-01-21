#pragma once

#include "Properties.hpp"
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

struct Node {
    Kind kind;

    std::string refid;
    std::string name;
    TextNode title;
    TextNode brief;

    Properties properties;

    NodeWeakPtr parent;
    NodeWeakPtr group;

    Includes includes;
    std::vector<NodeSharedPtr> children;
    std::vector<NodeRef> childrenRefs;
    std::optional<Location> location;
    std::optional<BodyLocation> bodyLocation;
};
} // namespace Doxybook2
