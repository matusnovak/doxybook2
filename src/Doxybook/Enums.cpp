#include "Macros.hpp"
#include <Doxybook/Config.hpp>
#include <Doxybook/Enums.hpp>
#include <Doxybook/Exception.hpp>
#include <unordered_map>

using KindStrPair = std::pair<std::string, Doxybook2::Kind>;
using VirtualStrPair = std::pair<std::string, Doxybook2::Virtual>;
using VisibilityStrPair = std::pair<std::string, Doxybook2::Visibility>;

static const std::vector<KindStrPair> KIND_STRS = {
    {"class", Doxybook2::Kind::CLASS},         {"namespace", Doxybook2::Kind::NAMESPACE},
    {"struct", Doxybook2::Kind::STRUCT},       {"interface", Doxybook2::Kind::INTERFACE},
    {"function", Doxybook2::Kind::FUNCTION},   {"variable", Doxybook2::Kind::VARIABLE},
    {"typedef", Doxybook2::Kind::TYPEDEF},     {"using", Doxybook2::Kind::USING},
    {"enum", Doxybook2::Kind::ENUM},           {"union", Doxybook2::Kind::UNION},
    {"enumvalue", Doxybook2::Kind::ENUMVALUE}, {"dir", Doxybook2::Kind::DIR},
    {"file", Doxybook2::Kind::FILE},           {"group", Doxybook2::Kind::MODULE},
    {"friend", Doxybook2::Kind::FRIEND},       {"page", Doxybook2::Kind::PAGE},
    {"example", Doxybook2::Kind::EXAMPLE},     {"signal", Doxybook2::Kind::SIGNAL},
    {"slot", Doxybook2::Kind::SLOT},           {"property", Doxybook2::Kind::PROPERTY},
    {"event", Doxybook2::Kind::EVENT},         {"define", Doxybook2::Kind::DEFINE},
};

static const std::vector<VirtualStrPair> VIRTUAL_STRS = {
    {"non-virtual", Doxybook2::Virtual::NON_VIRTUAL},
    {"virtual", Doxybook2::Virtual::VIRTUAL},
    {"pure", Doxybook2::Virtual::PURE_VIRTUAL},
    {"pure-virtual", Doxybook2::Virtual::PURE_VIRTUAL},
};

static const std::vector<VisibilityStrPair> VISIBILITY_STRS = {
    {"public", Doxybook2::Visibility::PUBLIC},
    {"protected", Doxybook2::Visibility::PROTECTED},
    {"private", Doxybook2::Visibility::PRIVATE},
    {"package", Doxybook2::Visibility::PACKAGE},
};

Doxybook2::Kind Doxybook2::toEnumKind(const std::string& str) {
    const auto it =
        std::find_if(KIND_STRS.begin(), KIND_STRS.end(), [&](const KindStrPair& pair) { return pair.first == str; });

    if (it == KIND_STRS.end()) {
        EXCEPTION("Kind {} not recognised please contact the author", str);
    }

    return it->second;
}

std::string Doxybook2::toStr(const Doxybook2::Kind value) {
    const auto it =
        std::find_if(KIND_STRS.begin(), KIND_STRS.end(), [&](const KindStrPair& pair) { return pair.second == value; });

    if (it == KIND_STRS.end()) {
        EXCEPTION("Kind {} not recognised please contact the author", int(value));
    }

    return it->first;
}

Doxybook2::Virtual Doxybook2::toEnumVirtual(const std::string& str) {
    const auto it = std::find_if(VIRTUAL_STRS.begin(), VIRTUAL_STRS.end(),
                                 [&](const VirtualStrPair& pair) { return pair.first == str; });

    if (it == VIRTUAL_STRS.end()) {
        EXCEPTION("Virtual {} not recognised please contact the author", str);
    }

    return it->second;
}

std::string Doxybook2::toStr(const Virtual value) {
    const auto it = std::find_if(VIRTUAL_STRS.begin(), VIRTUAL_STRS.end(),
                                 [&](const VirtualStrPair& pair) { return pair.second == value; });

    if (it == VIRTUAL_STRS.end()) {
        EXCEPTION("Virtual {} not recognised please contact the author", int(value));
    }

    return it->first;
}

Doxybook2::Visibility Doxybook2::toEnumVisibility(const std::string& str) {
    const auto it = std::find_if(VISIBILITY_STRS.begin(), VISIBILITY_STRS.end(),
                                 [&](const VisibilityStrPair& pair) { return pair.first == str; });

    if (it == VISIBILITY_STRS.end()) {
        EXCEPTION("Visibility {} not recognised please contact the author", str);
    }

    return it->second;
}

std::string Doxybook2::toStr(const Visibility value) {
    const auto it = std::find_if(VISIBILITY_STRS.begin(), VISIBILITY_STRS.end(),
                                 [&](const VisibilityStrPair& pair) { return pair.second == value; });

    if (it == VISIBILITY_STRS.end()) {
        EXCEPTION("Visibility {} not recognised please contact the author", int(value));
    }

    return it->first;
}

bool Doxybook2::isKindStructured(const Kind kind) {
    switch (kind) {
    case Doxybook2::Kind::CLASS:
    case Doxybook2::Kind::NAMESPACE:
    case Doxybook2::Kind::STRUCT:
    case Doxybook2::Kind::UNION:
    case Doxybook2::Kind::INTERFACE:
    case Doxybook2::Kind::FILE:
    case Doxybook2::Kind::DIR: {
        return true;
    }
    default: {
        return false;
    }
    }
}

bool Doxybook2::isKindLanguage(const Kind kind) {
    switch (kind) {
    case Doxybook2::Kind::DEFINE:
    case Doxybook2::Kind::CLASS:
    case Doxybook2::Kind::NAMESPACE:
    case Doxybook2::Kind::STRUCT:
    case Doxybook2::Kind::UNION:
    case Doxybook2::Kind::INTERFACE:
    case Doxybook2::Kind::ENUM:
    case Doxybook2::Kind::FUNCTION:
    case Doxybook2::Kind::TYPEDEF:
    case Doxybook2::Kind::USING:
    case Doxybook2::Kind::FRIEND:
    case Doxybook2::Kind::VARIABLE:
    case Doxybook2::Kind::SIGNAL:
    case Doxybook2::Kind::SLOT:
    case Doxybook2::Kind::PROPERTY:
    case Doxybook2::Kind::EVENT: {
        return true;
    }
    default: {
        return false;
    }
    }
}

bool Doxybook2::isKindFile(const Kind kind) {
    switch (kind) {
    case Doxybook2::Kind::DIR:
    case Doxybook2::Kind::FILE: {
        return true;
    }
    default: {
        return false;
    }
    }
}

std::string Doxybook2::typeToFolderName(const Config& config, const Kind kind) {
    if (!config.useFolders)
        return "";

    switch (kind) {
    case Kind::MODULE: {
        return config.folderGroupsName;
    }
    case Kind::NAMESPACE: {
        return config.folderNamespacesName;
    }
    case Kind::UNION:
    case Kind::INTERFACE:
    case Kind::STRUCT:
    case Kind::CLASS: {
        return config.folderClassesName;
    }
    case Kind::FILE:
    case Kind::DIR: {
        return config.folderFilesName;
    }
    case Kind::PAGE: {
        return config.folderRelatedPagesName;
    }
    case Kind::EXAMPLE: {
        return config.folderExamplesName;
    }
    default: {
        EXCEPTION("Cannot convert kind '{}' to a folder name please contact the author!", int(kind));
    }
    }
}

std::string Doxybook2::typeToIndexName(const Config& config, const FolderCategory type) {
    switch (type) {
    case FolderCategory::MODULES: {
        return config.indexInFolders && config.useFolders ? config.folderGroupsName + "/" + config.indexGroupsName
                                                          : config.indexGroupsName;
    }
    case FolderCategory::CLASSES: {
        return config.indexInFolders && config.useFolders ? config.folderClassesName + "/" + config.indexClassesName
                                                          : config.indexClassesName;
    }
    case FolderCategory::NAMESPACES: {
        return config.indexInFolders && config.useFolders
                   ? config.folderNamespacesName + "/" + config.indexNamespacesName
                   : config.indexNamespacesName;
    }
    case FolderCategory::FILES: {
        return config.indexInFolders && config.useFolders ? config.folderFilesName + "/" + config.indexFilesName
                                                          : config.indexFilesName;
    }
    case FolderCategory::PAGES: {
        return config.indexInFolders && config.useFolders
                   ? config.folderRelatedPagesName + "/" + config.indexRelatedPagesName
                   : config.indexRelatedPagesName;
    }
    case FolderCategory::EXAMPLES: {
        return config.indexInFolders && config.useFolders ? config.folderExamplesName + "/" + config.indexExamplesName
                                                          : config.indexExamplesName;
    }
    default: {
        EXCEPTION("Type {} not recognised please contact the author!", int(type));
    }
    }
}

std::string Doxybook2::typeToIndexTemplate(const Config& config, const FolderCategory type) {
    switch (type) {
    case FolderCategory::MODULES: {
        return config.templateIndexGroups;
    }
    case FolderCategory::CLASSES: {
        return config.templateIndexClasses;
    }
    case FolderCategory::NAMESPACES: {
        return config.templateIndexNamespaces;
    }
    case FolderCategory::FILES: {
        return config.templateIndexFiles;
    }
    case FolderCategory::PAGES: {
        return config.templateIndexRelatedPages;
    }
    case FolderCategory::EXAMPLES: {
        return config.templateIndexExamples;
    }
    default: {
        EXCEPTION("Type {} not recognised please contact the author!", int(type));
    }
    }
}

std::string Doxybook2::typeToIndexTitle(const Config& config, const FolderCategory type) {
    switch (type) {
    case FolderCategory::MODULES: {
        return config.indexGroupsTitle;
    }
    case FolderCategory::CLASSES: {
        return config.indexClassesTitle;
    }
    case FolderCategory::NAMESPACES: {
        return config.indexNamespacesTitle;
    }
    case FolderCategory::FILES: {
        return config.indexFilesTitle;
    }
    case FolderCategory::PAGES: {
        return config.indexRelatedPagesTitle;
    }
    case FolderCategory::EXAMPLES: {
        return config.indexExamplesTitle;
    }
    default: {
        EXCEPTION("Type {} not recognised please contact the author!", int(type));
    }
    }
}
