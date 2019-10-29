#pragma once
#include <exception>
#include <string>

namespace Engine {
	/*!
	 * @ingroup Engine
	 */
	class Exception: public std::exception {
	public:
		Exception() = default;

        explicit Exception(std::string msg)
            : msg(std::move(msg)) {

        }

        const char* what() const throw() override {
            return msg.c_str();
        }

    private:
        std::string msg;
	};
}
