#pragma once
#include <fmt/format.h>
#include <string>

namespace Doxybook2 {
    namespace Log {
        extern void i(const std::string& msg);
        extern void w(const std::string& msg);
        extern void e(const std::string& msg);
        extern void setQuietMode(bool quiet);

        template <typename... Args> inline void i(const std::string& msg, Args&&... args) {
            i(fmt::format(msg, std::forward<Args>(args)...));
        }
        template <typename... Args> inline void w(const std::string& msg, Args&&... args) {
            w(fmt::format(msg, std::forward<Args>(args)...));
        }
        template <typename... Args> inline void e(const std::string& msg, Args&&... args) {
            e(fmt::format(msg, std::forward<Args>(args)...));
        }
    }; // namespace Log
} // namespace Doxybook2
