#pragma once
#include <string>

namespace Doxybook2 {

    // Declaration of the Exception class
    class Exception : public std::exception {
    public:
        // Instructs compiler to define the default constructor, even if other constructors are present
        Exception() = default;

        // The "explicit" value informs the compiler to only accept the provided format when using this constructor, and not to assume that there has been a human mistake
        // This constructor moves the provided msg
        explicit Exception(std::string msg)
            : msg(std::move(msg)) {

        }

        // This is used in an exception, allowing the program to inform a user later of the msg.c_str() value in the event of an error
        const char* what() const throw() override {
            return msg.c_str();
        }

    private:
        std::string msg;
    };
}
