#pragma once
#include <nlohmann/json.hpp>
#include "TextPrinter.hpp"
#include "Node.hpp"

namespace Doxydown {
    class JsonConverter {
    public:
        static nlohmann::json convert(const Node::DerivedClass& klass);
        static nlohmann::json convert(const Node::BaseClass& klass);
        static nlohmann::json convert(const Node::DerivedClasses& klasses);
        static nlohmann::json convert(const Node::BaseClasses& klasses);
        static nlohmann::json convert(const Node::Location& location);
        static nlohmann::json convert(const Node& node, bool deep = true);
        static nlohmann::json convert(const Node::Children& children, bool deep = true);
        // Used to decorate the existing data out of convert(Node)
        // with markdown fields such as detailed description. This also decorates 
        // children (not classes nor structs, only functions, variables, etc.)
        static void decorate(const Node& node, nlohmann::json& data);
    private:
        static std::string toStr(Node::Kind kind);
        static std::string toStr(Node::Visibility prot);
        static std::string toStr(Node::Virtual virt);
    };
}
