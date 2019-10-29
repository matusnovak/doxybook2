#pragma once
#include <string>
#include <unordered_set>
#include "Doxygen.hpp"
#include "Renderer.hpp"
#include "JsonConverter.hpp"

namespace Doxydown {
    class Generator {
    public:
        typedef std::unordered_set<Node::Kind> Filter;

        explicit Generator(const Config& config, const JsonConverter& jsonConverter);

        void print(const Doxygen& doxygen);
        void printRecursively(const Node& parent);
        void printIndex(const Doxygen& doxygen, const std::string& name, const Filter& filter);
    private:
        std::string kindToTemplateName(Node::Kind kind);
        std::string indexToPath(const std::string& name);
        std::string indexToTemplateName(const std::string& name);
        void dump(const std::string& path, const nlohmann::json& data);
        nlohmann::json buildIndexRecursively(const Node& node, const Filter& filter);

        const Config& config;
        const JsonConverter& jsonConverter;
        Renderer renderer;
    };
}
