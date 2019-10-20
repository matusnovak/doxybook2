#pragma once
#include <string>
#include <list>
#include "Xml.hpp"
#include <vector>

namespace Doxydown {
    class TextParser {
    public:
        class Tag;
        typedef std::unique_ptr<Tag> TagPtr;
        typedef std::vector<TagPtr> Children;

        struct Node {
            enum Type {
                UNKNOWN = -1,
                TEXT = 0,
                PARA,
                PARAS,
                DETAILEDDESCRIPTION,
                BRIEFDESCRIPTION,
                BOLD,
                EMPHASIS,
                ULINK,
                REF,
                COMPUTEROUTPUT,
                LISTITEM,
                SIMPLESEC,
                ITEMIZEDLIST
            };

            Type type;
            std::list<Node> children;
            std::string data;
        };

        static Node parse(Xml::Element& element);
        static Node::Type strToType(const std::string& str);
    private:
        static void traverse(std::list<Node*> tree, const Xml::Node& element);
    };
}
