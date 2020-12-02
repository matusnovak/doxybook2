#include "Log.hpp"
#include "Macros.hpp"
#include <Doxybook/Config.hpp>
#include <Doxybook/Exception.hpp>
#include <fstream>
#include <nlohmann/json.hpp>

using namespace Doxybook2;

class ConfigArg {
public:
    template <typename T> ConfigArg(T Config::*ref, const std::string& key) : key(std::move(key)) {
        loadFunc = [=](const ConfigArg& self, Config& config, const nlohmann::json& json) {
            try {
                if (json.contains(self.key)) {
                    config.*ref = json.at(self.key).get<T>();
                }
            } catch (std::exception& e) {
                throw Exception(LOCATION, "Failed to get config value {} error: {}", self.key, e.what());
            }
        };
        saveFunc = [=](const ConfigArg& self, const Config& config, nlohmann::json& json) {
            json[self.key] = config.*ref;
        };
    }

    std::string key;
    std::function<void(const ConfigArg&, Config& config, const nlohmann::json&)> loadFunc;
    std::function<void(const ConfigArg&, const Config& config, nlohmann::json&)> saveFunc;
};

static const std::vector<ConfigArg> CONFIG_ARGS = {
    ConfigArg(&Config::baseUrl, "baseUrl"),
    ConfigArg(&Config::fileExt, "fileExt"),
    ConfigArg(&Config::linkSuffix, "linkSuffix"),
    ConfigArg(&Config::linkLowercase, "linkLowercase"),
    ConfigArg(&Config::copyImages, "copyImages"),
    ConfigArg(&Config::sort, "sort"),
    ConfigArg(&Config::useFolders, "useFolders"),
    ConfigArg(&Config::imagesFolder, "imagesFolder"),
    ConfigArg(&Config::mainPageName, "mainPageName"),
    ConfigArg(&Config::mainPageInRoot, "mainPageInRoot"),
    ConfigArg(&Config::folderClassesName, "folderClassesName"),
    ConfigArg(&Config::folderFilesName, "folderFilesName"),
    ConfigArg(&Config::folderGroupsName, "folderGroupsName"),
    ConfigArg(&Config::folderNamespacesName, "folderNamespacesName"),
    ConfigArg(&Config::folderRelatedPagesName, "folderRelatedPagesName"),
    ConfigArg(&Config::folderExamplesName, "folderExamplesName"),
    ConfigArg(&Config::indexInFolders, "indexInFolders"),
    ConfigArg(&Config::indexClassesName, "indexClassesName"),
    ConfigArg(&Config::indexFilesName, "indexFilesName"),
    ConfigArg(&Config::indexGroupsName, "indexGroupsName"),
    ConfigArg(&Config::indexNamespacesName, "indexNamespacesName"),
    ConfigArg(&Config::indexRelatedPagesName, "indexRelatedPagesName"),
    ConfigArg(&Config::indexExamplesName, "indexExamplesName"),
    ConfigArg(&Config::templateIndexClasses, "templateIndexClasses"),
    ConfigArg(&Config::templateIndexFiles, "templateIndexFiles"),
    ConfigArg(&Config::templateIndexGroups, "templateIndexGroups"),
    ConfigArg(&Config::templateIndexNamespaces, "templateIndexNamespaces"),
    ConfigArg(&Config::templateIndexRelatedPages, "templateIndexRelatedPages"),
    ConfigArg(&Config::templateIndexExamples, "templateIndexExamples"),
    ConfigArg(&Config::templateKindGroup, "templateKindGroup"),
    ConfigArg(&Config::templateKindClass, "templateKindClass"),
    ConfigArg(&Config::templateKindDir, "templateKindDir"),
    ConfigArg(&Config::templateKindPage, "templateKindPage"),
    ConfigArg(&Config::templateKindInterface, "templateKindInterface"),
    ConfigArg(&Config::templateKindFile, "templateKindFile"),
    ConfigArg(&Config::templateKindNamespace, "templateKindNamespace"),
    ConfigArg(&Config::templateKindStruct, "templateKindStruct"),
    ConfigArg(&Config::templateKindUnion, "templateKindUnion"),
    ConfigArg(&Config::templateKindExample, "templateKindExample"),
    ConfigArg(&Config::indexClassesTitle, "indexClassesTitle"),
    ConfigArg(&Config::indexNamespacesTitle, "indexNamespacesTitle"),
    ConfigArg(&Config::indexGroupsTitle, "indexGroupsTitle"),
    ConfigArg(&Config::indexRelatedPagesTitle, "indexRelatedPagesTitle"),
    ConfigArg(&Config::indexFilesTitle, "indexFilesTitle"),
    ConfigArg(&Config::indexExamplesTitle, "indexExamplesTitle"),
    ConfigArg(&Config::filesFilter, "filesFilter"),
};

void Doxybook2::loadConfig(Config& config, const std::filesystem::path& path) {
    std::ifstream file(path);
    if (!file) {
        throw Exception(LOCATION, "Failed to open file {} for reading", path.string());
    }

    std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    try {
        const auto json = nlohmann::json::parse(str);

        for (const auto& arg : CONFIG_ARGS) {
            arg.loadFunc(arg, config, json);
        }
    } catch (std::exception& e) {
        throw Exception(LOCATION, "Failed to parse config error {}", e.what());
    }
}

void Doxybook2::loadConfigData(Config& config, const std::string& src) {
    try {
        const auto json = nlohmann::json::parse(src);

        for (const auto& arg : CONFIG_ARGS) {
            arg.loadFunc(arg, config, json);
        }
    } catch (std::exception& e) {
        throw Exception(LOCATION, "Failed to parse config error {}", e.what());
    }
}

void Doxybook2::saveConfig(Config& config, const std::filesystem::path& path) {
    Log::i("Creating default config {}", path.string());
    std::ofstream file(path);
    if (!file) {
        throw Exception(LOCATION, "Failed to open file {} for writing", path.string());
    }

    nlohmann::json json;
    for (const auto& arg : CONFIG_ARGS) {
        arg.saveFunc(arg, config, json);
    }

    file << json.dump(2);
}
