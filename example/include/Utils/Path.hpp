#pragma once
#include <string>

namespace Engine {
	namespace Utils {
		/*!
		 * @brief Filesystem path utility functions
		 * @ingroup Utils
 		 */
		namespace Path {
			std::string getFilename(const std::string& path);
			std::string getExtension(const std::string& path);
		}
	}
}