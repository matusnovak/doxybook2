#pragma once
#include <string>
#include <unordered_set>
#include "Renderer.hpp"
#include "JsonConverter.hpp"
#include "TemplateLoader.hpp"

namespace Doxydown {
    class Generator {
    public:
        typedef std::unordered_set<Kind> Filter;

        explicit Generator(const Config& config, const JsonConverter& jsonConverter, const TemplateLoader& templateLoader);

        void print(const Doxygen& doxygen);
        void printRecursively(const Node& parent);
        void printIndex(const Doxygen& doxygen, const std::string& name, const Filter& filter);
    private:
        std::string kindToTemplateName(Kind kind);
        std::string indexToPath(const std::string& name);
        std::string indexToTemplateName(const std::string& name);
        nlohmann::json buildIndexRecursively(const Node& node, const Filter& filter);

        const Config& config;
        const JsonConverter& jsonConverter;
        Renderer renderer;
    };
}
