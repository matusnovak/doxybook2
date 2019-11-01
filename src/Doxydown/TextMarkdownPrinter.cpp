#include <sstream>
#include <Doxydown/TextMarkdownPrinter.hpp>
#include <Doxydown/Utils.hpp>
#include <Doxydown/Doxygen.hpp>

std::string Doxydown::TextMarkdownPrinter::print(const XmlTextParser::Node& node) const {
    std::stringstream ss;
    print(ss, node);
    auto str = ss.str();
    while (!str.empty() && str.back() == '\n') str.pop_back();
    return str;
}

void Doxydown::TextMarkdownPrinter::print(std::stringstream& ss, const XmlTextParser::Node& node) const {
    switch (node.type) {
        case XmlTextParser::Node::Type::TEXT: {
            ss << node.data;
            break;
        }
        case XmlTextParser::Node::Type::BOLD: {
            ss << "**";
            break;
        }
        case XmlTextParser::Node::Type::EMPHASIS: {
            ss << "_";
            break;
        }
        case XmlTextParser::Node::Type::ITEMIZEDLIST: {
            ss << "\n\n";
            break;
        }
        case XmlTextParser::Node::Type::LISTITEM: {
            ss << "* ";
            break;
        }
        case XmlTextParser::Node::Type::ULINK:
        case XmlTextParser::Node::Type::REF: {
            ss << "[";
            break;
        }
        case XmlTextParser::Node::Type::COMPUTEROUTPUT: {
            ss << "`";
            break;
        }
        case XmlTextParser::Node::Type::PROGRAMLISTING: {
            ss << "\n\n```cpp\n";
            break;
        }
        case XmlTextParser::Node::Type::SP: {
            ss << " ";
            break;
        }
        default: {
            break;
        }
    }

    for (const auto& child : node.children) {
        print(ss, child);
    }

    switch (node.type) {
        case XmlTextParser::Node::Type::BOLD: {
            ss << "**";
            break;
        }
        case XmlTextParser::Node::Type::EMPHASIS: {
            ss << "_";
            break;
        }
        case XmlTextParser::Node::Type::ULINK: {
            ss << "](" << node.extra << ")";
            break;
        }
        case XmlTextParser::Node::Type::REF: {
            ss << "]";
            const auto found = doxygen.getCache().find(node.extra);
            if (found != doxygen.getCache().end()) {
                ss << "(" << found->second->getUrl() << ")";
            }
            break;
        }
        case XmlTextParser::Node::Type::PARA: {
            ss << "\n";
            break;
        }
        case XmlTextParser::Node::Type::COMPUTEROUTPUT: {
            ss << "`";
            break;
        }
        case XmlTextParser::Node::Type::LISTITEM: {
            ss << "";
            break;
        }
        case XmlTextParser::Node::Type::CODELINE: {
            ss << "\n";
            break;
        }
        case XmlTextParser::Node::Type::PROGRAMLISTING: {
            ss << "```\n\n";
            break;
        }
        default: {
            break;
        }
    }
}
