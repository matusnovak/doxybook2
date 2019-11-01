#pragma once
#include <fmt/format.h>
#include <Doxydown/Exception.hpp>
#include <Doxydown/Path.hpp>
#include <Doxydown/Log.hpp>

#define __FILENAME__ Doxydown::Path::filename(__FILE__)

#define EXCEPTION(MSG, ...) Doxydown::Exception( \
    fmt::format("Exception at {}:{} ", __FILENAME__, __LINE__) \
    + fmt::format(MSG, ##__VA_ARGS__))

#define WARNING(MSG, ...) Doxydown::Log::w(\
    fmt::format("Warning at {}:{} ", __FILENAME__, __LINE__) \
    + fmt::format(MSG, ##__VA_ARGS__))
