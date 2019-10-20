#include <chrono>
#include <iomanip>
#include <inja/inja.hpp>
#include "Renderer.hpp"
#include "Exception.hpp"
#include "Log.hpp"

Doxydown::Renderer::Renderer() : env(std::make_unique<inja::Environment>()) {
    env->add_callback("title", 1, [](inja::Arguments& args) {
        auto arg = args.at(0)->get<std::string>();
        if (!arg.empty()) arg[0] = toupper(arg[0]);
        return arg;
    });
    env->add_callback("date", 1, [](inja::Arguments& args) -> std::string {
        auto arg = args.at(0)->get<std::string>();
        const auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        char mbstr[100];
        std::strftime(mbstr, sizeof(mbstr), arg.c_str(), std::localtime(&t));
        return mbstr;
    });
}

Doxydown::Renderer::~Renderer() = default;

void Doxydown::Renderer::render(const std::string& name, const std::string& path, const nlohmann::json& data) const {
    const auto it = templates.find(name);
    if (it == templates.end()) {
        throw EXCEPTION("Template {} not found", name);
    }

    std::fstream file(path, std::ios::out);
    if (!file) throw EXCEPTION("Failed to open file for writing {}", path);
    Log::i("Rendering {}", path);
    try {
        env->render_to(file, *it->second, data);
    } catch (std::exception& e) {
        throw EXCEPTION("Failed to render template error {}", e.what());
    }
}

void Doxydown::Renderer::addTemplate(const std::string& name, const std::string& src) {
    try {
        inja::Template tmpl = env->parse(src);
        const auto it = templates.insert(std::make_pair(name, std::make_unique<inja::Template>(std::move(tmpl)))).first;
        env->include_template(it->first, *it->second);
    } catch (std::exception& e) {
        throw EXCEPTION("Failed to parse template named {} error {}", name, e.what());
    }
}
