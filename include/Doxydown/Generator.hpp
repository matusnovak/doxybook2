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

        void print(const Doxygen& doxygen, const Filter& filter = {});
        void printRecursively(const Node& parent, const Filter& filter = {});
        void printIndex(const Doxygen& doxygen, Type type, const Filter& filter);
    private:
        std::string kindToTemplateName(Kind kind);
        nlohmann::json buildIndexRecursively(const Node& node, const Filter& filter);

        const Config& config;
        const JsonConverter& jsonConverter;
        Renderer renderer;
    };
}
