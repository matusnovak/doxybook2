#pragma once
#include <string>

namespace Doxydown {
    namespace Log {
        extern void i(const std::string& msg);
        extern void w(const std::string& msg);
        extern void e(const std::string& msg);

#ifdef FMT_FORMAT_H_
        template<typename... Args>
        inline void i(const std::string& msg, Args&&... args) {
            i(fmt::format(msg, std::forward<Args>(args)...));
        }
        template<typename... Args>
        inline void w(const std::string& msg, Args&&... args) {
            w(fmt::format(msg, std::forward<Args>(args)...));
        }
        template<typename... Args>
        inline void e(const std::string& msg, Args&&... args) {
            e(fmt::format(msg, std::forward<Args>(args)...));
        }
#endif
    };
}

