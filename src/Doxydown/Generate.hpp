#pragma once
#include <string>
#include "Doxygen.hpp"
#include "Renderer.hpp"
#include "JsonConverter.hpp"

namespace Doxydown {
    void generateMarkdown(Doxygen& doxygen, const std::string& outputDir);
    void generateJson(Doxygen& doxygen, const std::string& outputDir);
}
