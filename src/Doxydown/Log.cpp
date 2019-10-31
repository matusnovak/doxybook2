#include <iostream>
#include <termcolor/termcolor.hpp>
#include <Doxydown/Log.hpp>

void Doxydown::Log::i(const std::string& msg) {
    std::cout << msg << std::endl;
}

void Doxydown::Log::w(const std::string& msg) {
    std::cerr << termcolor::yellow << msg << termcolor::reset << std::endl;
}

void Doxydown::Log::e(const std::string& msg) {
    std::cerr << termcolor::red << msg << termcolor::reset << std::endl;
}
