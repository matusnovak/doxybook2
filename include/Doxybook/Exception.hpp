#pragma once
#include <fmt/format.h>
#include <string>

namespace Doxybook2 {
    class Exception : public std::exception {
    public:
        Exception() = default;

        explicit Exception(const std::string& location, const std::string& msg) : msg(location + " " + msg) {
        }

        template <typename... Args>
        explicit Exception(const std::string& location, const std::string& msg, Args&&... args)
            : msg(location + " " + fmt::format(msg, std::forward<Args>(args)...)) {
        }

        [[nodiscard]] const char* what() const noexcept override {
            return msg.c_str();
        }

    private:
        std::string msg;
    };
} // namespace Doxybook2
