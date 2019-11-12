#include <iostream>
#include <termcolor/termcolor.hpp>
#include <Doxybook/Log.hpp>

void Doxybook2::Log::i(const std::string& msg) {
    std::cout << msg << std::endl;
}

void Doxybook2::Log::w(const std::string& msg) {
    std::cerr << termcolor::yellow << msg << termcolor::reset << std::endl;
}

void Doxybook2::Log::e(const std::string& msg) {
    std::cerr << termcolor::red << msg << termcolor::reset << std::endl;
}
