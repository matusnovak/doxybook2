#pragma once
#include <fmt/format.h>
#include <Doxybook/Exception.hpp>
#include <Doxybook/Path.hpp>


#define __FILENAME__ Doxybook2::Path::filename(__FILE__)

#define EXCEPTION(MSG, ...) Doxybook2::Exception( \
    fmt::format("Exception at {}:{} ", __FILENAME__, __LINE__) \
    + fmt::format(MSG, ##__VA_ARGS__))

#define WARNING(MSG, ...) Doxybook2::spdlog::warn(\
    fmt::format("Warning at {}:{} ", __FILENAME__, __LINE__) \
    + fmt::format(MSG, ##__VA_ARGS__))
