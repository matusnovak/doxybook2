#include "ExceptionUtils.hpp"
#include <Doxybook/Config.hpp>
#include <Doxybook/Enums.hpp>
#include <unordered_map>

using KindStrPair = std::pair<std::string, Doxybook2::Kind>;
using TypeStrPair = std::pair<std::string, Doxybook2::Type>;
using VirtualStrPair = std::pair<std::string, Doxybook2::Virtual>;
using VisibilityStrPair = std::pair<std::string, Doxybook2::Visibility>;
using FolderCategoryStrPair = std::pair<std::string, Doxybook2::FolderCategory>;

// clang-format off
static const std::vector<KindStrPair> KIND_STRS = {
    {"class", Doxybook2::Kind::CLASS},
    {"namespace", Doxybook2::Kind::NAMESPACE},
    {"struct", Doxybook2::Kind::STRUCT},
    {"interface", Doxybook2::Kind::INTERFACE},
    {"function", Doxybook2::Kind::FUNCTION},
    {"variable", Doxybook2::Kind::VARIABLE},
    {"typedef", Doxybook2::Kind::TYPEDEF},
    {"using", Doxybook2::Kind::USING},
    {"enum", Doxybook2::Kind::ENUM},
    {"union", Doxybook2::Kind::UNION},
    {"enumvalue", Doxybook2::Kind::ENUMVALUE},
    {"dir", Doxybook2::Kind::DIR},
    {"file", Doxybook2::Kind::FILE},
    {"group", Doxybook2::Kind::MODULE},
    {"friend", Doxybook2::Kind::FRIEND},
    {"page", Doxybook2::Kind::PAGE},
    {"example", Doxybook2::Kind::EXAMPLE},
    {"signal", Doxybook2::Kind::SIGNAL},
    {"slot", Doxybook2::Kind::SLOT},
    {"property", Doxybook2::Kind::PROPERTY},
    {"event", Doxybook2::Kind::EVENT},
    {"define", Doxybook2::Kind::DEFINE}
};

static const std::vector<TypeStrPair> TYPE_STRS = {
    {"attributes", Doxybook2::Type::ATTRIBUTES},
    {"classes", Doxybook2::Type::CLASSES},
    {"defines", Doxybook2::Type::DEFINES},
    {"files", Doxybook2::Type::FILES},
    {"dirs", Doxybook2::Type::DIRS},
    {"friends", Doxybook2::Type::FRIENDS},
    {"functions", Doxybook2::Type::FUNCTIONS},
    {"modules", Doxybook2::Type::MODULES},
    {"namespaces", Doxybook2::Type::NAMESPACES},
    {"types", Doxybook2::Type::TYPES},
    {"pages", Doxybook2::Type::PAGES},
    {"examples", Doxybook2::Type::EXAMPLES},
    {"signals", Doxybook2::Type::SIGNALS},
    {"slots", Doxybook2::Type::SLOTS},
    {"events", Doxybook2::Type::EVENTS},
    {"properties", Doxybook2::Type::PROPERTIES}
};

static const std::vector<VirtualStrPair> VIRTUAL_STRS = {
    {"non-virtual", Doxybook2::Virtual::NON_VIRTUAL},
    {"virtual", Doxybook2::Virtual::VIRTUAL},
    {"pure", Doxybook2::Virtual::PURE_VIRTUAL},
    {"pure-virtual", Doxybook2::Virtual::PURE_VIRTUAL}
};

static const std::vector<VisibilityStrPair> VISIBILITY_STRS = {
    {"public", Doxybook2::Visibility::PUBLIC},
    {"protected", Doxybook2::Visibility::PROTECTED},
    {"private", Doxybook2::Visibility::PRIVATE},
    {"package", Doxybook2::Visibility::PACKAGE}
};

static const std::vector<FolderCategoryStrPair> FOLDER_CATEGORY_STRS = {
    {"modules", Doxybook2::FolderCategory::MODULES},
    {"namespaces", Doxybook2::FolderCategory::NAMESPACES},
    {"files", Doxybook2::FolderCategory::FILES},
    {"examples", Doxybook2::FolderCategory::EXAMPLES},
    {"classes", Doxybook2::FolderCategory::CLASSES},
    {"pages", Doxybook2::FolderCategory::PAGES}
};
// clang-format on

template <typename Enum> struct EnumName { static inline const auto name = "unknown"; };

template <> struct EnumName<Doxybook2::Kind> { static inline const auto name = "Kind"; };
template <> struct EnumName<Doxybook2::Type> { static inline const auto name = "Type"; };
template <> struct EnumName<Doxybook2::Virtual> { static inline const auto name = "Virtual"; };
template <> struct EnumName<Doxybook2::Visibility> { static inline const auto name = "Visibility"; };
template <> struct EnumName<Doxybook2::FolderCategory> { static inline const auto name = "FolderCategory"; };

template <typename Enum>
static Enum toEnum(const std::vector<std::pair<std::string, Enum>>& pairs, const std::string& str) {
    const auto it = std::find_if(
        pairs.begin(), pairs.end(), [&](const std::pair<std::string, Enum>& pair) { return pair.first == str; });

    if (it == pairs.end()) {
        throw EXCEPTION("String '{}' not recognised as a valid enum of '{}'", str, EnumName<Enum>::name);
    }

    return it->second;
}

template <typename Enum>
static std::string fromEnum(const std::vector<std::pair<std::string, Enum>>& pairs, const Enum value) {
    const auto it = std::find_if(
        pairs.begin(), pairs.end(), [&](const std::pair<std::string, Enum>& pair) { return pair.second == value; });

    if (it == pairs.end()) {
        throw EXCEPTION(
            "Enum '{}' of value '{}' not recognised please contact the author", EnumName<Enum>::name, int(value));
    }

    return it->first;
}

Doxybook2::Kind Doxybook2::toEnumKind(const std::string& str) {
    return toEnum<Kind>(KIND_STRS, str);
}

std::string Doxybook2::toStr(const Kind value) {
    return fromEnum<Kind>(KIND_STRS, value);
}

Doxybook2::Virtual Doxybook2::toEnumVirtual(const std::string& str) {
    return toEnum<Virtual>(VIRTUAL_STRS, str);
}

std::string Doxybook2::toStr(const Virtual value) {
    return fromEnum<Virtual>(VIRTUAL_STRS, value);
}

Doxybook2::Visibility Doxybook2::toEnumVisibility(const std::string& str) {
    return toEnum<Visibility>(VISIBILITY_STRS, str);
}

std::string Doxybook2::toStr(const Visibility value) {
    return fromEnum<Visibility>(VISIBILITY_STRS, value);
}

Doxybook2::Type Doxybook2::toEnumType(const std::string& str) {
    return toEnum<Type>(TYPE_STRS, str);
}

std::string Doxybook2::toStr(const Type value) {
    return fromEnum<Type>(TYPE_STRS, value);
}

Doxybook2::FolderCategory Doxybook2::toEnumFolderCategory(const std::string& str) {
    return toEnum<FolderCategory>(FOLDER_CATEGORY_STRS, str);
}

std::string Doxybook2::toStr(const FolderCategory value) {
    return fromEnum<FolderCategory>(FOLDER_CATEGORY_STRS, value);
}

bool Doxybook2::isKindStructured(const Kind kind) {
    switch (kind) {
        case Doxybook2::Kind::CLASS:
        case Doxybook2::Kind::NAMESPACE:
        case Doxybook2::Kind::STRUCT:
        case Doxybook2::Kind::UNION:
        case Doxybook2::Kind::INTERFACE: {
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

std::string Doxybook2::typeFolderCategoryToFolderName(const Config& config, FolderCategory type) {
    if (!config.useFolders)
        return "";

    switch (type) {
        case FolderCategory::MODULES: {
            return config.folderGroupsName;
        }
        case FolderCategory::CLASSES: {
            return config.folderClassesName;
        }
        case FolderCategory::NAMESPACES: {
            return config.folderNamespacesName;
        }
        case FolderCategory::FILES: {
            return config.folderFilesName;
        }
        case FolderCategory::PAGES: {
            return config.folderRelatedPagesName;
        }
        case FolderCategory::EXAMPLES: {
            return config.folderExamplesName;
        }
        default: {
            throw EXCEPTION("FolderCategory {} not recognised please contant the author!", int(type));
        }
    }
}

std::string Doxybook2::typeToFolderName(const Config& config, const Type type) {
    if (!config.useFolders)
        return "";

    switch (type) {
        case Type::MODULES: {
            return config.folderGroupsName;
        }
        case Type::CLASSES: {
            return config.folderClassesName;
        }
        case Type::NAMESPACES: {
            return config.folderNamespacesName;
        }
        case Type::DIRS:
        case Type::FILES: {
            return config.folderFilesName;
        }
        case Type::PAGES: {
            return config.folderRelatedPagesName;
        }
        case Type::EXAMPLES: {
            return config.folderExamplesName;
        }
        default: {
            throw EXCEPTION("Type {} not recognised please contant the author!", int(type));
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
            return config.indexInFolders && config.useFolders
                       ? config.folderExamplesName + "/" + config.indexExamplesName
                       : config.indexExamplesName;
        }
        default: {
            throw EXCEPTION("Type {} not recognised please contant the author!", int(type));
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
            throw EXCEPTION("Type {} not recognised please contant the author!", int(type));
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
            throw EXCEPTION("Type {} not recognised please contant the author!", int(type));
        }
    }
}
