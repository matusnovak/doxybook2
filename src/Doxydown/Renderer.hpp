#pragma once
#include <string>
#include <nlohmann/json.hpp>

namespace inja {
    struct Template;
    class Environment;
}

namespace Doxydown {
    class Renderer {
    public:
        Renderer();
        ~Renderer();

        void render(const std::string& name, const std::string& path, const nlohmann::json& data) const;
        void addTemplate(const std::string& name, const std::string& src);
    private:
        std::unique_ptr<inja::Environment> env;
        std::unordered_map<std::string, std::unique_ptr<inja::Template>> templates;
    };
}
