#pragma once

#include <filesystem>
#include <string>

#ifndef __FILENAME__
#define __FILENAME__ std::filesystem::path(__FILE__).stem().string()
#endif

#ifndef LOCATION
#define LOCATION (__FILENAME__ + std::string(":") + std::to_string(__LINE__))
#endif
