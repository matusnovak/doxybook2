#pragma once
#include <iostream>
#include <fmt/format.h>
#include "Path.hpp"

namespace Doxydown {
    class Log {
    public:
        template<typename... Args>
        static void i(const std::string& msg, Args&&... args) {
            std::cout << fmt::format(msg, std::forward<Args>(args)...) << std::endl;
        }

        template<typename... Args>
        static void w(const std::string& msg, Args&&... args) {
            std::cerr << fmt::format(msg, std::forward<Args>(args)...) << std::endl;
        }

        template<typename... Args>
        static void e(const std::string& msg, Args&&... args) {
            std::cerr << fmt::format(msg, std::forward<Args>(args)...) << std::endl;
        }
    };
}

#define WARNING(MSG, ...) Doxydown::Log::w(fmt::format("Warning at {}:{} ", __FILENAME__, __LINE__) + fmt::format(MSG, ##__VA_ARGS__))
