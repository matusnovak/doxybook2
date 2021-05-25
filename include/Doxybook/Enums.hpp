#pragma once
#include <nlohmann/json.hpp>
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
    USING,
    FRIEND,
    ENUM,
    ENUMVALUE,
    UNION,
    DIR,
    FILE,
    MODULE,
    PAGE,
    EXAMPLE,
    SIGNAL,
    SLOT,
    PROPERTY,
    EVENT,
};

enum class Visibility {
    PUBLIC,
    PROTECTED,
    PRIVATE,
    PACKAGE,
};

enum class Virtual {
    NON_VIRTUAL,
    VIRTUAL,
    PURE_VIRTUAL,
};

enum class FolderCategory {
    CLASSES,
    NAMESPACES,
    MODULES,
    PAGES,
    FILES,
    EXAMPLES,
};

extern Kind toEnumKind(const std::string& str);
extern Visibility toEnumVisibility(const std::string& str);
extern Virtual toEnumVirtual(const std::string& str);

extern std::string toStr(Kind value);
extern std::string toStr(Visibility value);
extern std::string toStr(Virtual value);

extern bool isKindLanguage(Kind kind);
extern bool isKindStructured(Kind kind);
extern bool isKindFile(Kind kind);
extern std::string typeToFolderName(const Config& config, Kind kind);
extern std::string typeToIndexName(const Config& config, FolderCategory type);
extern std::string typeToIndexTemplate(const Config& config, FolderCategory type);
extern std::string typeToIndexTitle(const Config& config, FolderCategory type);

inline void to_json(nlohmann::json& j, const Kind& p) {
    j = toStr(p);
}

inline void to_json(nlohmann::json& j, const Visibility& p) {
    j = toStr(p);
}

inline void to_json(nlohmann::json& j, const Virtual& p) {
    j = toStr(p);
}
} // namespace Doxybook2
