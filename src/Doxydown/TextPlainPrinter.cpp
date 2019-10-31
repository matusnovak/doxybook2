#include <sstream>
#include <Doxydown/TextPlainPrinter.hpp>
#include <Doxydown/Utils.hpp>

std::string Doxydown::TextPlainPrinter::print(const XmlTextParser::Node& node) const {
    std::stringstream ss;
    print(ss, node);
    auto str = ss.str();
    while (!str.empty() && str.back() == '\n') str.pop_back();
    return str;
}

void Doxydown::TextPlainPrinter::print(std::stringstream& ss, const XmlTextParser::Node& node) const {
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
}
