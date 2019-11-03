#include <fstream>
#include <nlohmann/json.hpp>
#include <Doxydown/Config.hpp>
#include "ExceptionUtils.hpp"

class ConfigArg {
public:
    ConfigArg(std::string Doxydown::Config::*ref, std::string key) : key(std::move(key)) {
        loadFunc = [=](const ConfigArg& self, Doxydown::Config& config, const nlohmann::json& json) {
            try {
                if (json.contains(self.key)) {
                    config.*ref = json.at(self.key).get<std::string>();
                }
            } catch (std::exception& e) {
                throw EXCEPTION("Failed to get config string value {} error: {}", self.key, e.what());
            }
        };
        saveFunc = [=](const ConfigArg& self, const Doxydown::Config& config,  nlohmann::json& json) {
            json[self.key] = config.*ref;
        };
    }

    ConfigArg(bool Doxydown::Config::*ref, std::string key) : key(std::move(key)) {
        loadFunc = [=](const ConfigArg& self, Doxydown::Config& config, const nlohmann::json& json) {
            try {
                if (json.contains(self.key)) {
                    config.*ref = json.at(self.key).get<bool>();
                }
            } catch (std::exception& e) {
                throw EXCEPTION("Failed to get config bool value {} error: {}", self.key, e.what());
            }
        };
        saveFunc = [=](const ConfigArg& self, const Doxydown::Config& config,  nlohmann::json& json) {
            json[self.key] = config.*ref;
        };
    }

    std::string key;
    std::function<void(const ConfigArg&, Doxydown::Config& config, const nlohmann::json&)> loadFunc;
    std::function<void(const ConfigArg&, const Doxydown::Config& config, nlohmann::json&)> saveFunc;
};

static const std::vector<ConfigArg> configArgs = {
    ConfigArg(&Doxydown::Config::baseUrl, "baseUrl"),
    ConfigArg(&Doxydown::Config::fileExt, "fileExt"),
    ConfigArg(&Doxydown::Config::linkSuffix, "linkSuffix"),
    ConfigArg(&Doxydown::Config::linkLowercase, "linkLowercase"),
    ConfigArg(&Doxydown::Config::copyImages, "copyImages"),
    ConfigArg(&Doxydown::Config::imagesFolder, "imagesFolder"),
    ConfigArg(&Doxydown::Config::mainPageName, "mainPageName"),
    ConfigArg(&Doxydown::Config::mainPageInRoot, "mainPageInRoot"),
    ConfigArg(&Doxydown::Config::folderClassesName, "folderClassesName"),
    ConfigArg(&Doxydown::Config::folderFilesName, "folderFilesName"),
    ConfigArg(&Doxydown::Config::folderGroupsName, "folderGroupsName"),
    ConfigArg(&Doxydown::Config::folderNamespacesName, "folderNamespacesName"),
    ConfigArg(&Doxydown::Config::folderRelatedPagesName, "folderRelatedPagesName"),
    ConfigArg(&Doxydown::Config::indexInFolders, "indexInFolders"),
    ConfigArg(&Doxydown::Config::indexClassesName, "indexClassesName"),
    ConfigArg(&Doxydown::Config::indexFilesName, "indexFilesName"),
    ConfigArg(&Doxydown::Config::indexGroupsName, "indexGroupsName"),
    ConfigArg(&Doxydown::Config::indexNamespacesName, "indexNamespacesName"),
    ConfigArg(&Doxydown::Config::indexRelatedPagesName, "indexRelatedPagesName"),
    ConfigArg(&Doxydown::Config::templateIndexClasses, "templateIndexClasses"),
    ConfigArg(&Doxydown::Config::templateIndexFiles, "templateIndexFiles"),
    ConfigArg(&Doxydown::Config::templateIndexGroups, "templateIndexGroups"),
    ConfigArg(&Doxydown::Config::templateIndexNamespaces, "templateIndexNamespaces"),
    ConfigArg(&Doxydown::Config::templateIndexRelatedPages, "templateIndexRelatedPages"),
    ConfigArg(&Doxydown::Config::templateKindGroup, "templateKindGroup"),
    ConfigArg(&Doxydown::Config::templateKindClass, "templateKindClass"),
    ConfigArg(&Doxydown::Config::templateKindDir, "templateKindDir"),
    ConfigArg(&Doxydown::Config::templateKindPage, "templateKindPage"),
    ConfigArg(&Doxydown::Config::templateKindInterface, "templateKindInterface"),
    ConfigArg(&Doxydown::Config::templateKindFile, "templateKindFile"),
    ConfigArg(&Doxydown::Config::templateKindNamespace, "templateKindNamespace"),
    ConfigArg(&Doxydown::Config::templateKindStruct, "templateKindStruct"),
    ConfigArg(&Doxydown::Config::templateKindUnion, "templateKindUnion"),
};

void Doxydown::loadConfig(Config& config, const std::string& path) {
    std::ifstream file(path);
    if (!file) throw EXCEPTION("Failed to open file {} for reading", path);
    std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    const auto json = nlohmann::json::parse(str);

    for (const auto& arg : configArgs) {
        arg.loadFunc(arg, config, json);
    }
}

void Doxydown::saveConfig(Config& config, const std::string& path) {
    Log::i("Creating default config {}", path);
    std::ofstream file(path);
    if (!file) throw EXCEPTION("Failed to open file {} for writing", path);

    nlohmann::json json;
    for (const auto& arg : configArgs) {
        arg.saveFunc(arg, config, json);
    }

    file << json.dump(2);
}
