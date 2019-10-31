#include <unordered_map>
#include <Doxydown/Enums.hpp>
#include <Doxydown/Config.hpp>
#include "ExceptionUtils.hpp"

template <>
extern Doxydown::Kind Doxydown::toEnum<Doxydown::Kind>(const std::string& str) {
    static std::unordered_map<std::string, Kind> kinds = {
        {"define", Kind::DEFINE},
        {"class", Kind::CLASS},
        {"namespace", Kind::NAMESPACE},
        {"struct", Kind::STRUCT},
        {"interface", Kind::INTERFACE},
        {"function", Kind::FUNCTION},
        {"variable", Kind::VARIABLE},
        {"typedef", Kind::TYPEDEF},
        {"enum", Kind::ENUM},
        {"union", Kind::UNION},
        {"enumvalue", Kind::ENUMVALUE},
        {"dir", Kind::DIR},
        {"file", Kind::FILE},
        {"group", Kind::MODULE},
        {"friend", Kind::FRIEND}
    };

    const auto it = kinds.find(str);
    if (it == kinds.end()) {
        throw EXCEPTION("Kind {} not recognised please contact the author", str);
    }

    return it->second;
}

template <>
extern std::string Doxydown::toStr<Doxydown::Kind>(const Kind value) {
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
            return "group";
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
        case Kind::FRIEND:
            return "friend";
        default: {
            throw EXCEPTION("Kind {} not recognised please contact the author", int(value));
        }
    }
}

template <>
extern Doxydown::Virtual Doxydown::toEnum<Doxydown::Virtual>(const std::string& str) {
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

template <>
extern std::string Doxydown::toStr<Doxydown::Virtual>(const Virtual value) {
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

template <>
extern Doxydown::Visibility Doxydown::toEnum<Doxydown::Visibility>(const std::string& str) {
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

template <>
extern std::string Doxydown::toStr<Doxydown::Visibility>(const Visibility value) {
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

template <>
extern Doxydown::Type Doxydown::toEnum<Doxydown::Type>(const std::string& str) {
    static std::unordered_map<std::string, Type> values = {
        {"attributes", Type::ATTRIBUTES},
        {"classes", Type::CLASSES},
        {"defines", Type::DEFINES},
        {"files", Type::DIRORFILE},
        {"friends", Type::FRIENDS},
        {"functions", Type::FUNCTIONS},
        {"modules", Type::MODULES},
        {"namespaces", Type::NAMESPACES},
        {"types", Type::TYPES},
    };

    const auto it = values.find(str);
    if (it == values.end()) {
        throw EXCEPTION("Type {} not recognised please contact the author", str);
    }

    return it->second;
}

template <>
extern std::string Doxydown::toStr<Doxydown::Type>(const Type value) {
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
        case Type::DIRORFILE:
            return "files";
        case Type::FRIENDS:
            return "friends";
        default: {
            throw EXCEPTION("Type {} not recognised please contact the author", int(value));
        }
    }
}


bool Doxydown::isKindStructured(const Kind kind) {
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

bool Doxydown::isKindLanguage(const Kind kind) {
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
        case Kind::FRIEND:
        case Kind::VARIABLE: {
            return true;
        }
        default: {
            return false;
        }
    }
}

bool Doxydown::isKindFile(const Kind kind) {
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

const std::string& Doxydown::typeToFolderName(const Config& config, const Type type) {
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
        case Type::DIRORFILE: {
            return config.folderFilesName;
        }
        default: {
            throw EXCEPTION("Type {} not recognised please contant the author!", int(type));
        }
    }
}
