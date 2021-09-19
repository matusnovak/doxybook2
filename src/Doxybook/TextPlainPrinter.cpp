#include <sstream>
#include <Doxybook/TextPlainPrinter.hpp>
#include <Doxybook/Utils.hpp>

std::string Doxybook2::TextPlainPrinter::print(const XmlTextParser::Node& node, const std::string& language) const {
    std::stringstream ss;
    print(ss, node);
    auto str = ss.str();
    while (!str.empty() && str.back() == '\n') str.pop_back();
    return str;
}

void Doxybook2::TextPlainPrinter::print(std::stringstream& ss, const XmlTextParser::Node& node) const {
    switch (node.type) {
        case XmlTextParser::Node::Type::TEXT: {
            ss << node.data;
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
        case XmlTextParser::Node::Type::CODELINE: {
            ss << "\n";
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
}
