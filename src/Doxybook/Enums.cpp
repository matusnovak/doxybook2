#include "ExceptionUtils.hpp"
#include <Doxybook/Config.hpp>
#include <Doxybook/Enums.hpp>
#include <unordered_map>

Doxybook2::Kind Doxybook2::toEnumKind(const std::string& str) {
    static std::unordered_map<std::string, Kind> kinds = {{"define", Kind::DEFINE},
        {"class", Kind::CLASS},
        {"namespace", Kind::NAMESPACE},
        {"struct", Kind::STRUCT},
        {"interface", Kind::INTERFACE},
        {"function", Kind::FUNCTION},
        {"variable", Kind::VARIABLE},
        {"typedef", Kind::TYPEDEF},
        {"using", Kind::USING},
        {"enum", Kind::ENUM},
        {"union", Kind::UNION},
        {"enumvalue", Kind::ENUMVALUE},
        {"dir", Kind::DIR},
        {"file", Kind::FILE},
        {"group", Kind::MODULE},
        {"friend", Kind::FRIEND},
        {"page", Kind::PAGE},
        {"example", Kind::EXAMPLE}};

    const auto it = kinds.find(str);
    if (it == kinds.end()) {
        throw EXCEPTION("Kind {} not recognised please contact the author", str);
    }

    return it->second;
}

std::string Doxybook2::toStr(const Kind value) {
    switch (value) {
        case Kind::DEFINE:
            return "define";
        case Kind::CLASS:
            return "class";
        case Kind::DIR:
            return "dir";
        case Kind::ENUM:
            return "enum";
        case Kind::ENUMVALUE:
            return "enumvalue";
        case Kind::FILE:
            return "file";
        case Kind::FUNCTION:
            return "function";
        case Kind::MODULE:
            return "module";
        case Kind::INDEX:
            return "index";
        case Kind::INTERFACE:
            return "interface";
        case Kind::NAMESPACE:
            return "namespace";
        case Kind::UNION:
            return "union";
        case Kind::STRUCT:
            return "struct";
        case Kind::VARIABLE:
            return "variable";
        case Kind::TYPEDEF:
            return "typedef";
        case Kind::USING:
            return "using";
        case Kind::FRIEND:
            return "friend";
        case Kind::PAGE:
            return "page";
        case Kind::EXAMPLE:
            return "example";
        default: {
            throw EXCEPTION("Kind {} not recognised please contact the author", int(value));
        }
    }
}

Doxybook2::Virtual Doxybook2::toEnumVirtual(const std::string& str) {
    static std::unordered_map<std::string, Virtual> values = {
        {"non-virtual", Virtual::NON_VIRTUAL},
        {"virtual", Virtual::VIRTUAL},
        {"pure", Virtual::PURE_VIRTUAL},
        {"pure-virtual", Virtual::PURE_VIRTUAL},
    };

    const auto it = values.find(str);
    if (it == values.end()) {
        throw EXCEPTION("Virtual {} not recognised please contact the author", str);
    }

    return it->second;
}

std::string Doxybook2::toStr(const Virtual value) {
    switch (value) {
        case Virtual::NON_VIRTUAL:
            return "non-virtual";
        case Virtual::VIRTUAL:
            return "virtual";
        case Virtual::PURE_VIRTUAL:
            return "pure-virtual";
        default: {
            throw EXCEPTION("Virtual {} not recognised please contact the author", int(value));
        }
    };
}

Doxybook2::Visibility Doxybook2::toEnumVisibility(const std::string& str) {
    static std::unordered_map<std::string, Visibility> values = {
        {"public", Visibility::PUBLIC},
        {"protected", Visibility::PROTECTED},
        {"private", Visibility::PRIVATE},
    };

    const auto it = values.find(str);
    if (it == values.end()) {
        throw EXCEPTION("Visibility {} not recognised please contact the author", str);
    }

    return it->second;
}

std::string Doxybook2::toStr(const Visibility value) {
    switch (value) {
        case Visibility::PUBLIC:
            return "public";
        case Visibility::PROTECTED:
            return "protected";
        case Visibility::PRIVATE:
            return "private";
        default: {
            throw EXCEPTION("Visibility {} not recognised please contact the author", int(value));
        }
    };
}

Doxybook2::Type Doxybook2::toEnumType(const std::string& str) {
    static std::unordered_map<std::string, Type> values = {
        {"attributes", Type::ATTRIBUTES},
        {"classes", Type::CLASSES},
        {"defines", Type::DEFINES},
        {"files", Type::FILES},
        {"dirs", Type::DIRS},
        {"friends", Type::FRIENDS},
        {"functions", Type::FUNCTIONS},
        {"modules", Type::MODULES},
        {"namespaces", Type::NAMESPACES},
        {"types", Type::TYPES},
        {"pages", Type::PAGES},
        {"examples", Type::EXAMPLES},
    };

    const auto it = values.find(str);
    if (it == values.end()) {
        throw EXCEPTION("Type {} not recognised please contact the author", str);
    }

    return it->second;
}

std::string Doxybook2::toStr(const Type value) {
    switch (value) {
        case Type::DEFINES:
            return "defines";
        case Type::ATTRIBUTES:
            return "attributes";
        case Type::FUNCTIONS:
            return "functions";
        case Type::CLASSES:
            return "classes";
        case Type::NAMESPACES:
            return "namespaces";
        case Type::MODULES:
            return "groups";
        case Type::TYPES:
            return "types";
        case Type::FILES:
            return "files";
        case Type::DIRS:
            return "dirs";
        case Type::FRIENDS:
            return "friends";
        case Type::PAGES:
            return "pages";
        case Type::EXAMPLES:
            return "examples";
        default: {
            throw EXCEPTION("Type {} not recognised please contact the author", int(value));
        }
    }
}

bool Doxybook2::isKindStructured(const Kind kind) {
    switch (kind) {
        case Kind::CLASS:
        case Kind::NAMESPACE:
        case Kind::STRUCT:
        case Kind::UNION:
        case Kind::INTERFACE: {
            return true;
        }
        default: {
            return false;
        }
    }
}

bool Doxybook2::isKindLanguage(const Kind kind) {
    switch (kind) {
        case Kind::DEFINE:
        case Kind::CLASS:
        case Kind::NAMESPACE:
        case Kind::STRUCT:
        case Kind::UNION:
        case Kind::INTERFACE:
        case Kind::ENUM:
        case Kind::FUNCTION:
        case Kind::TYPEDEF:
        case Kind::USING:
        case Kind::FRIEND:
        case Kind::VARIABLE: {
            return true;
        }
        default: {
            return false;
        }
    }
}

bool Doxybook2::isKindFile(const Kind kind) {
    switch (kind) {
        case Kind::DIR:
        case Kind::FILE: {
            return true;
        }
        default: {
            return false;
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
