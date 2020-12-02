#include "Log.hpp"
#include <iostream>
#include <rang.hpp>

using namespace Doxybook2;

static bool quiet = false;

void Log::i(const std::string& msg) {
    if (::quiet) {
        return;
    }
    std::cout << msg << std::endl;
}

void Log::w(const std::string& msg) {
    std::cerr << rang::fg::yellow << msg << rang::fg::reset << std::endl;
}

void Log::e(const std::string& msg) {
    std::cerr << rang::fg::red << msg << rang::fg::reset << std::endl;
}

void Log::setQuietMode(const bool quiet) {
    ::quiet = quiet;
}
