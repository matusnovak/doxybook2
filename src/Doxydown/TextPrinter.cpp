#include <sstream>
#include <cassert>
#include "TextPrinter.hpp"
#include "TextUtils.hpp"

Doxydown::TextPrinter::TextPrinter(const Config& config)
    : config(config) {

}

std::string Doxydown::TextPrinter::printMarkdown(const TextParser::Node& node) const {
    assert(cache);
    std::stringstream ss;
    printMarkdown(ss, node);
    auto str = ss.str();
    while (!str.empty() && str.back() == '\n') str.pop_back();
    return str;
}

std::string Doxydown::TextPrinter::printPlain(const TextParser::Node& node) const {
    std::stringstream ss;
    printPlain(ss, node);
    auto str = ss.str();
    while (!str.empty() && str.back() == '\n') str.pop_back();
    return str;
}

void Doxydown::TextPrinter::printMarkdown(std::stringstream& ss, const TextParser::Node& node) const {
    switch (node.type) {
        case TextParser::Node::Type::TEXT: {
            ss << node.data;
            break;
        }
        case TextParser::Node::Type::BOLD: {
            ss << "**";
            break;
        }
        case TextParser::Node::Type::EMPHASIS: {
            ss << "_";
            break;
        }
        case TextParser::Node::Type::ITEMIZEDLIST: {
            ss << "\n\n";
            break;
        }
        case TextParser::Node::Type::LISTITEM: {
            ss << "* ";
            break;
        }
        case TextParser::Node::Type::ULINK:
        case TextParser::Node::Type::REF: {
            ss << "[";
            break;
        }
        case TextParser::Node::Type::COMPUTEROUTPUT: {
            ss << "`";
            break;
        }
        default: {
            break;
        }
    }

    for (const auto& child : node.children) {
        printMarkdown(ss, child);
    }

    switch (node.type) {
        case TextParser::Node::Type::BOLD: {
            ss << "**";
            break;
        }
        case TextParser::Node::Type::EMPHASIS: {
            ss << "_";
            break;
        }
        case TextParser::Node::Type::ULINK: {
            ss << "](" << node.extra << ")";
            break;
        }
        case TextParser::Node::Type::REF: {
            ss << "]";
            const auto found = cache->find(node.extra);
            if (found != cache->end()) {
                ss << "(" << found->second->getUrl() << ")";
            }
            break;
        }
        case TextParser::Node::Type::PARA: {
            ss << "\n";
            break;
        }
        case TextParser::Node::Type::COMPUTEROUTPUT: {
            ss << "`";
            break;
        }
        case TextParser::Node::Type::LISTITEM: {
            ss << "";
            break;
        }
        default: {
            break;
        }
    }
}

void Doxydown::TextPrinter::printPlain(std::stringstream& ss, const TextParser::Node& node) const {
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
