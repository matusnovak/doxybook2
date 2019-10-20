#pragma once
#include <string>
#include <sstream>
#include "TextParser.hpp"

namespace Doxydown {
    class TextPrinter {
    public:
        static std::string printMarkdown(const TextParser::Node& node);
        static std::string printPlain(const TextParser::Node& node);
    private:
        static void printMarkdown(std::stringstream& ss, const TextParser::Node& node);
        static void printPlain(std::stringstream& ss, const TextParser::Node& node);
    };
}
