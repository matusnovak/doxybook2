#include <functional>
#include <unordered_map>
#include "TextParser.hpp"
#include "Log.hpp"
#include "Exception.hpp"

Doxydown::TextParser::Node::Type Doxydown::TextParser::strToType(const std::string& str) {
    static std::unordered_map<std::string, Node::Type> kinds = {
        {"briefdescription", Node::Type::BRIEFDESCRIPTION},
        {"detaileddescription", Node::Type::DETAILEDDESCRIPTION},
        {"para", Node::Type::PARA},
        {"bold", Node::Type::BOLD},
        {"emphasis", Node::Type::EMPHASIS},
        {"ulink", Node::Type::ULINK},
        {"ref", Node::Type::REF},
        {"listitem", Node::Type::LISTITEM},
        {"itemizedlis", Node::Type::ITEMIZEDLIST},
        {"simplesect", Node::Type::SIMPLESEC},
        {"computeroutput", Node::Type::COMPUTEROUTPUT}
    };

    const auto it = kinds.find(str);
    if (it == kinds.end()) {
        WARNING("Text tag \"{}\" not recognised, please contact the author", str);
        return Node::Type::UNKNOWN;
    }

    return it->second;
}

Doxydown::TextParser::Node Doxydown::TextParser::parse(Xml::Element& element) {
    Node result;
    result.type = Node::Type::PARAS;
    std::list<Node*> tree = {&result};
    //std::cout << ">>> TRAVERSE:" << std::endl;
    auto para = element.firstChildElement("para");
    while (para) {
        traverse(tree, para.asNode());
        para = element.nextSiblingElement("para");
    }
    //std::cout << std::endl;
    return result;
}

void Doxydown::TextParser::traverse(std::list<Node*> tree, const Xml::Node& element) {
    if (!element) return;

    if (element.isElement()) {
        //std::cout << "<" << element.asElement().getName() << ">";
        Node node;
        node.type = strToType(element.asElement().getName());
        tree.back()->children.push_back(std::move(node));
        const auto ptr = &tree.back()->children.back();
        tree.push_back(ptr);
    }

    if (element.hasText()) {
        if (!element.isElement()) {
            const auto text = element.getText();
            //std::cout << text;
            if (!text.empty()) {
                Node node;
                node.data = text;
                node.type = Node::Type::TEXT;
                tree.back()->children.push_back(node);
            }
        }
    }

    auto child = element.firstChild();
    while (child) {
        traverse(tree, child);
        child = child.nextSibling();
    }

    if (element.isElement()) {
        //std::cout << "</" << element.asElement().getName() << ">";
        tree.pop_back();
    }
}
