#pragma once
#include <string>

namespace Doxydown {
    struct Config;

    enum class Kind {
        INDEX,
        DEFINE,
        CLASS,
        NAMESPACE,
        STRUCT,
        INTERFACE,
        FUNCTION,
        VARIABLE,
        TYPEDEF,
        FRIEND,
        ENUM,
        ENUMVALUE,
        UNION,
        DIR,
        FILE,
        MODULE
    };

    enum class Visibility {
        PUBLIC,
        PROTECTED,
        PRIVATE
    };

    enum class Virtual {
        NON_VIRTUAL,
        VIRTUAL,
        PURE_VIRTUAL
    };

    enum class Type {
        NONE,
        DEFINES,
        FUNCTIONS,
        NAMESPACES,
        CLASSES,
        ATTRIBUTES,
        TYPES,
        DIRORFILE,
        MODULES,
        FRIENDS
    };

    template <typename T>
    extern T toEnum(const std::string& str);

    template <typename T>
    extern std::string toStr(T value);

    extern bool isKindLanguage(Kind kind);
    extern bool isKindStructured(Kind kind);
    extern bool isKindFile(Kind kind);
    extern const std::string& typeToFolderName(const Config& config, Type type);
}
