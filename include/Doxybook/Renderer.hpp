#pragma once
#include <string>
#include <memory>
#include <unordered_map>
#include <nlohmann/json.hpp>
#include "Config.hpp"

namespace inja {
    struct Template;
    class Environment;
}

namespace Doxybook2 {
    class Renderer {
    public:
        explicit Renderer(const Config& config);
        ~Renderer();

        void render(const std::string& name, const std::string& path, const nlohmann::json& data) const;
        std::string render(const std::string& name, const nlohmann::json& data) const;
        void addTemplate(const std::string& name, const std::string& src);

    private:
        const Config& config;

        std::unique_ptr<inja::Environment> env;
        std::unordered_map<std::string, std::unique_ptr<inja::Template>> templates;
    };
}
