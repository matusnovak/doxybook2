#pragma once
#include <string>

namespace Doxydown {
    namespace TextUtils {
        extern std::string title(std::string str);
        extern std::string toLower(std::string str);
        extern std::string safeAnchorId(std::string str);
        extern std::string date(const std::string& format);
        extern std::string stripNamespace(const std::string& format);
        extern std::string stripAnchor(const std::string& str);
    }
}
