#pragma once

#include "Properties.hpp"
#include <memory>
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

        explicit NodeRef(BasicRef basic) {
            value = std::move(basic);
        }

        [[nodiscard]] bool empty() const {
            return value.index() == 0;
        }

        [[nodiscard]] bool resolved() const {
            return value.index() == 2;
        }

    private:
        std::variant<std::nullptr_t, BasicRef, NodeWeakPtr> value;
    };

    struct Include {
        NodeRef node;
        bool isLocal{false};
    };

    using Includes = std::vector<Include>;

    struct Location {
        std::string file;
        int line{0};
        int column{0};
        std::string bodyFile;
        int bodyStart{0};
        int bodyEnd{0};
    };

    struct Node {
        Kind kind;

        std::string refid;
        std::string name;
        std::string title;
        std::string brief;

        Properties properties;

        NodeWeakPtr parent;
        NodeWeakPtr group;

        Includes includes;
        std::vector<NodeSharedPtr> children;
        Location location;
    };
} // namespace Doxybook2
