#pragma once
#include "TextPrinter.hpp"

namespace Doxydown {
    class TextMarkdownPrinter : public TextPrinter {
    public:
        explicit TextMarkdownPrinter(const Config& config, std::string inputDir, const Doxygen& doxygen)
            : TextPrinter(config, doxygen), inputDir(std::move(inputDir)) {

        }

        std::string print(const XmlTextParser::Node& node) const override;
    private:
        struct ListData {
            int counter{0};
            bool ordered{false};
        };

        struct PrintData {
            std::stringstream ss;
            int indent{0};
            std::list<ListData> lists;
            bool eol{false};
        };

        void print(PrintData& data,
                   const XmlTextParser::Node* parent,
                   const XmlTextParser::Node* node,
                   const XmlTextParser::Node* previous,
                   const XmlTextParser::Node* next) const;

        std::string inputDir;
    };
}
