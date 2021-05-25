#pragma once

#include <stdexcept>

namespace Doxybook2 {
extern std::string replaceAll(std::string str, const std::string& from, const std::string& to);
extern void backtrace(const std::exception& e, const size_t level = 0);
extern std::string toLower(std::string str);
extern std::string date(const std::string& format);
extern std::string safeAnchorId(std::string str);
extern std::string stripNamespace(const std::string& format);
extern std::string stripAnchor(const std::string& str);
} // namespace Doxybook2
