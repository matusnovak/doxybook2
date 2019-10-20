#include "TextPrinter.hpp"

std::string Doxydown::TextPrinter::printMarkdown(const TextParser::Node& node) {
    std::stringstream ss;
    return ss.str();
}

std::string Doxydown::TextPrinter::printPlain(const TextParser::Node& node) {
    std::stringstream ss;
    printPlain(ss, node);
    return ss.str();
}

void Doxydown::TextPrinter::printMarkdown(std::stringstream& ss, const TextParser::Node& node) {

}

void Doxydown::TextPrinter::printPlain(std::stringstream& ss, const TextParser::Node& node) {
    switch (node.type) {
        case TextParser::Node::Type::TEXT: {
            ss << node.data;
            break;
        }
        default: {
            break;
        }
    }

    for (const auto& child : node.children) {
        printPlain(ss, child);
    }
}

