#pragma once
#include <string>
#include <fmt/format.h>
#include "Path.hpp"

namespace Doxydown {
    class Exception : public std::exception {
    public:
        Exception() = default;

        explicit Exception(std::string msg)
            : msg(std::move(msg)) {

        }

        template <typename ...Args> explicit Exception(std::string msg, Args&&... args)
            : msg(fmt::format(msg, std::forward<Args>(args)...)) {
        }

        const char* what() const throw() override {
            return msg.c_str();
        }

    private:
        std::string msg;
    };

    class DoxygenException : public Exception {
    public:
        DoxygenException() = default;

        explicit DoxygenException(std::string msg)
            : Exception(std::move(msg)) {

        }

        template <typename ...Args> explicit DoxygenException(std::string msg, Args&&... args)
            : Exception(std::move(msg), std::forward<Args>(args)...) {
        }
    };

    class XmlException : public Exception {
    public:
        XmlException() = default;

        explicit XmlException(std::string msg)
            : Exception(std::move(msg)) {

        }

        template <typename ...Args> explicit XmlException(std::string msg, Args&&... args)
            : Exception(std::move(msg), std::forward<Args>(args)...) {
        }
    };
}

#define EXCEPTION(MSG, ...) Doxydown::Exception( \
    fmt::format("Exception at {}:{} ", __FILENAME__, __LINE__) \
    + fmt::format(MSG, ##__VA_ARGS__))
#define DOXYGEN_EXCEPTION(MSG, ...) Doxydown::DoxygenException( \
    fmt::format("Exception at {}:{} ", __FILENAME__, __LINE__) \
    + fmt::format(MSG, ##__VA_ARGS__))
#define XML_EXCEPTION(MSG, ...) Doxydown::XmlException( \
    fmt::format("Exception at {}:{} ", __FILENAME__, __LINE__) \
    + fmt::format(MSG, ##__VA_ARGS__))
