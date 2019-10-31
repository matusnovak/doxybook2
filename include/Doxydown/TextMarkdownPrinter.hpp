#pragma once
#include "TextPrinter.hpp"

namespace Doxydown {
    class TextMarkdownPrinter : public TextPrinter {
    public:
        explicit TextMarkdownPrinter(const Config& config, const Doxygen& doxygen)
            : TextPrinter(config, doxygen) {

        }

        std::string print(const XmlTextParser::Node& node) const override;
    private:
        void print(std::stringstream& ss, const XmlTextParser::Node& node) const;
    };
}
