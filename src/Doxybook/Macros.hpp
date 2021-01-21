#pragma once

#include <filesystem>
#include <fmt/format.h>
#include <stdexcept>
#include <string>

#ifndef __FILENAME__
#define __FILENAME__ std::filesystem::path(__FILE__).stem().string()
#endif

#ifndef WHERE
#define WHERE (__FILENAME__ + std::string(":") + std::to_string(__LINE__))
#endif

#ifndef EXCEPTION
#define EXCEPTION(...) throw std::runtime_error(fmt::format("Exception at {} {}", WHERE, fmt::format(__VA_ARGS__)));
#endif

#ifndef EXCEPTION_NESTED
#define EXCEPTION_NESTED(...)                                                                                          \
    std::throw_with_nested(std::runtime_error(fmt::format("Exception at {} {}", WHERE, fmt::format(__VA_ARGS__))));
#endif
