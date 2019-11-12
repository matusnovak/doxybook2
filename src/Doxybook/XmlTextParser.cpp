#include <functional>
#include <unordered_map>
#include <Doxybook/XmlTextParser.hpp>
#include <Doxybook/Log.hpp>
#include <Doxybook/Exception.hpp>
#include "ExceptionUtils.hpp"

Doxybook2::XmlTextParser::Node::Type Doxybook2::XmlTextParser::strToType(const std::string& str) {
    static std::unordered_map<std::string, Node::Type> kinds = {
        {"para", Node::Type::PARA},
        {"bold", Node::Type::BOLD},
        {"emphasis", Node::Type::EMPHASIS},
        {"strike", Node::Type::STRIKE},
        {"hruler", Node::Type::HRULER},
        {"image", Node::Type::IMAGE},
        {"ulink", Node::Type::ULINK},
        {"ref", Node::Type::REF},
        {"listitem", Node::Type::LISTITEM},
        {"itemizedlist", Node::Type::ITEMIZEDLIST},
        {"variablelist", Node::Type::VARIABLELIST},
        {"orderedlist", Node::Type::ORDEREDLIST},
        {"varlistentry", Node::Type::VARLISTENTRY},
        {"term", Node::Type::TERM},
        {"anchor", Node::Type::ANCHOR},
        {"simplesect", Node::Type::SIMPLESEC},
        {"computeroutput", Node::Type::COMPUTEROUTPUT},
        {"parameterdescription", Node::Type::PARAMETERDESCRIPTION},
        {"parametername", Node::Type::PARAMETERNAME},
        {"parameterlist", Node::Type::PARAMETERLIST},
        {"parameteritem", Node::Type::PARAMETERITEM},
        {"parameternamelist", Node::Type::PARAMETERNAMELIST},
        {"type", Node::Type::PARA},
        {"argsstring", Node::Type::PARA},
        {"defval", Node::Type::PARA},
        {"declname", Node::Type::PARA},
        {"xrefsect", Node::Type::XREFSECT},
        {"xreftitle", Node::Type::XREFTITLE},
        {"xrefdescription", Node::Type::XREFDESCRIPTION},
        {"initializer", Node::Type::PARA},
        {"programlisting", Node::Type::PROGRAMLISTING},
        {"codeline", Node::Type::CODELINE},
        {"sp", Node::Type::SP},
        {"highlight", Node::Type::HIGHTLIGHT},
        {"defname", Node::Type::PARA},
        {"title", Node::Type::TITLE},
        {"sect1", Node::Type::SECT1},
        {"sect2", Node::Type::SECT2},
        {"sect3", Node::Type::SECT3},
        {"sect4", Node::Type::SECT4},
        {"sect5", Node::Type::SECT5},
        {"sect6", Node::Type::SECT6},
    };

    const auto it = kinds.find(str);
    if (it == kinds.end()) {
        WARNING("Text tag \"{}\" not recognised, please contact the author", str);
        return Node::Type::UNKNOWN;
    }

    return it->second;
}

Doxybook2::XmlTextParser::Node Doxybook2::XmlTextParser::parseParas(const Xml::Element& element) {
    Node result;
    result.type = Node::Type::PARAS;
    std::vector<Node*> tree = { &result };
    auto para = element.firstChildElement();
    while (para) {
        traverse(tree, para.asNode());
        para = para.nextSiblingElement();
    }
    return result;
}

Doxybook2::XmlTextParser::Node Doxybook2::XmlTextParser::parsePara(const Xml::Element& element) {
    Node result;
    result.type = Node::Type::PARA;
    std::vector<Node*> tree = { &result };
    traverse(tree, element.asNode());
    return result;
}

void Doxybook2::XmlTextParser::traverse(std::vector<Node*> tree, const Xml::Node& element) {
    if (!element) return;

    if (element.isElement()) {
        const auto& e = element.asElement();
        Node node;
        node.type = strToType(e.getName());
        tree.back()->children.push_back(std::move(node));
        const auto ptr = &tree.back()->children.back();
        tree.push_back(ptr);

        switch(ptr->type) {
            case Node::Type::SIMPLESEC: {
                ptr->extra = e.getAttr("kind");
                break;
            }
            case Node::Type::PARAMETERLIST: {
                ptr->extra = e.getAttr("kind");
                break;
            }
            case Node::Type::REF: {
                ptr->extra = e.getAttr("refid");
                break;
            }
            case Node::Type::ULINK: {
                ptr->extra = e.getAttr("url");
                break;
            }
            case Node::Type::IMAGE: {
                ptr->extra = e.getAttr("name");
                break;
            }
            case Node::Type::XREFSECT: {
                const auto id = e.getAttr("id");
                const auto pos = id.find('_');
                if (pos != std::string::npos) {
                    ptr->extra = id.substr(0, pos);
                } else {
                    ptr->extra = id;
                }
                break;
            }
            default: {
                break;
            }
        }
    }

    if (element.hasText()) {
        if (!element.isElement()) {
            const auto text = element.getText();
            if (!text.empty()) {
                Node node;
                node.data = text;
                node.type = Node::Type::TEXT;
                tree.back()->children.push_back(node);
            }
        } else {
            const auto text = element.getText();

        }
    }

    auto child = element.firstChild();
    while (child) {
        traverse(tree, child);
        child = child.nextSibling();
    }

    if (element.isElement()) {
        tree.pop_back();
    }
}
