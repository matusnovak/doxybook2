#include <fstream>
#include <Doxybook/TemplateFolderLoader.hpp>
#include <Doxybook/Utils.hpp>
#include <Doxybook/Log.hpp>
#include "ExceptionUtils.hpp"
#include <dirent.h>

Doxybook2::TemplateFolderLoader::TemplateFolderLoader(const std::string& path) {
    Log::i("Loading templates from: " + path);
    auto dir = opendir(path.c_str());
    if (dir == nullptr) {
        throw EXCEPTION("Failed to read directory {}", path);
    }

    const auto findInTemplate = [this](const std::string& name) {
        for (auto it = this->templates.begin(); it != this->templates.end(); ++it) {
            if (it->name == name) return it;
        }
        return this->templates.end();
    };

    const auto loadFile = [](const std::string& filename) {
        std::ifstream file(filename);
        if (!file) throw EXCEPTION("Failed to open file {} for reading", filename);
        std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        return str;
    };

    auto ent = readdir((DIR*)dir);
    while(ent != nullptr) {
        const auto file = std::string(ent->d_name);
        if (file.size() > 4 && file.find(".tmpl") == file.size() - 5) {
            const auto name = file.substr(0, file.size() - 5);
            Log::i("Loading template: " + name);
            auto it = findInTemplate(name);
            if (it != templates.end()) {
                it->contents = loadFile(Utils::join(path, name + ".tmpl"));
            } else {
                templates.push_back({name, loadFile(Utils::join(path, name + ".tmpl"))});
            }
        }
        ent = readdir((DIR*)dir);
    }
}
