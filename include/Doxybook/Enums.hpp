#pragma once
#include <string>

namespace Doxybook2 {
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
        MODULE,
        PAGE,
        EXAMPLE
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
        DIRS,
        FILES,
        MODULES,
        FRIENDS,
        PAGES,
        EXAMPLES
    };

    enum class FolderCategory {
        CLASSES,
        NAMESPACES,
        MODULES,
        PAGES,
        FILES,
        EXAMPLES
    };

    // Declarations of external functions in the Doxybook2 namespace
    // The "extern" specifier allows for the Type of the function to be declared here and defined somewhere else in the program
    extern Kind toEnumKind(const std::string& str);
    extern Type toEnumType(const std::string& str);
    extern Visibility toEnumVisibility(const std::string& str);
    extern Virtual toEnumVirtual(const std::string& str);

    extern std::string toStr(Kind value);
    extern std::string toStr(Type value);
    extern std::string toStr(Visibility value);
    extern std::string toStr(Virtual value);

    extern bool isKindLanguage(Kind kind);
    extern bool isKindStructured(Kind kind);
    extern bool isKindFile(Kind kind);
    extern std::string typeToFolderName(const Config& config, Type type);
    extern std::string typeToIndexName(const Config& config, FolderCategory type);
    extern std::string typeToIndexTemplate(const Config& config, FolderCategory type);
    extern std::string typeToIndexTitle(const Config& config, FolderCategory type);
}
