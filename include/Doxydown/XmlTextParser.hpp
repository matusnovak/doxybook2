#pragma once
#include <string>
#include <vector>
#include "Xml.hpp"

namespace Doxydown {
    class XmlTextParser {
    public:
        class Tag;
        typedef std::unique_ptr<Tag> TagPtr;
        typedef std::vector<TagPtr> Children;

        struct Node {
            enum class Type {
                UNKNOWN = -1,
                TEXT = 0,
                PARA,
                PARAS,
                BOLD,
                EMPHASIS,
                ULINK,
                REF,
                COMPUTEROUTPUT,
                LISTITEM,
                SIMPLESEC,
                ITEMIZEDLIST,
                PARAMETERLIST,
                PARAMETERNAME,
                PARAMETERITEM,
                PARAMETERDESCRIPTION,
                PARAMETERNAMELIST,
                XREFSECT,
                XREFTITLE,
                XREFDESCRIPTION
            };

            Type type{Type::UNKNOWN};
            std::vector<Node> children;
            std::string data;
            std::string extra;
        };

        static Node parseParas(const Xml::Element& element);
        static Node parsePara(const Xml::Element& element);
        static Node::Type strToType(const std::string& str);
    private:
        static void traverse(std::vector<Node*> tree, const Xml::Node& element);
    };
}
