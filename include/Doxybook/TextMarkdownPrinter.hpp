#pragma once
#include "TextPrinter.hpp"
#include <sstream>
namespace Doxybook2 {
    class TextMarkdownPrinter : public TextPrinter {
      public:
        explicit TextMarkdownPrinter(const Config& config, std::string inputDir, const Doxygen& doxygen)
            : TextPrinter(config, doxygen), inputDir(std::move(inputDir)) {
        }

        std::string print(const XmlTextParser::Node& node, const std::string& language) const override;

      private:
        struct ListData {
            int counter{0};
            bool ordered{false};
        };

        struct PrintData {
            std::stringstream ss;
            int indent{0};
            std::list<ListData> lists;
            bool quote{false};
            bool eol{false};
            bool tableHeader{false};
            bool inComputerOutput{false};
            bool validLink{false};
        };

        void print(PrintData& data,
            const XmlTextParser::Node* parent,
            const XmlTextParser::Node* node,
            const XmlTextParser::Node* previous,
            const XmlTextParser::Node* next,
            const std::string& language) const;

        void programlisting(PrintData& data, const XmlTextParser::Node& node) const;

        std::string inputDir;
    };
} // namespace Doxybook2
