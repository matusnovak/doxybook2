#include <sstream>
#include <fstream>
#include <Doxybook/TextMarkdownPrinter.hpp>
#include <Doxybook/Utils.hpp>
#include <Doxybook/Doxygen.hpp>

std::string Doxybook2::TextMarkdownPrinter::print(const XmlTextParser::Node& node) const {
    PrintData data;
    print(data, nullptr, &node, nullptr, nullptr);
    auto str = data.ss.str();
    while (!str.empty() && str.back() == '\n') str.pop_back();
    return str;
}

void Doxybook2::TextMarkdownPrinter::print(PrintData& data,
                                          const XmlTextParser::Node* parent,
                                          const XmlTextParser::Node* node,
                                          const XmlTextParser::Node* previous,
                                          const XmlTextParser::Node* next) const {

    switch (node->type) {
        case XmlTextParser::Node::Type::TEXT: {
            data.ss << node->data;
            data.eol = false;
            break;
        }
        case XmlTextParser::Node::Type::SECT1: {
            data.ss << "\n# ";
            data.eol = false;
            break;
        }
        case XmlTextParser::Node::Type::SECT2: {
            data.ss << "\n## ";
            data.eol = false;
            break;
        }
        case XmlTextParser::Node::Type::SECT3: {
            data.ss << "\n### ";
            data.eol = false;
            break;
        }
        case XmlTextParser::Node::Type::SECT4: {
            data.ss << "\n#### ";
            data.eol = false;
            break;
        }
        case XmlTextParser::Node::Type::SECT5: {
            data.ss << "\n##### ";
            data.eol = false;
            break;
        }
        case XmlTextParser::Node::Type::SECT6: {
            data.ss << "\n###### ";
            data.eol = false;
            break;
        }
        case XmlTextParser::Node::Type::BOLD: {
            data.ss << "**";
            data.eol = false;
            break;
        }
        case XmlTextParser::Node::Type::EMPHASIS: {
            data.ss << "_";
            data.eol = false;
            break;
        }
        case XmlTextParser::Node::Type::STRIKE: {
            data.ss << "~~";
            data.eol = false;
            break;
        }
        case XmlTextParser::Node::Type::VARIABLELIST:
        case XmlTextParser::Node::Type::ORDEREDLIST:
        case XmlTextParser::Node::Type::ITEMIZEDLIST: {
            if (data.lists.empty()) data.ss << "\n";
            data.ss << "\n";
            data.eol = true;
            data.lists.push_back({0, node->type == XmlTextParser::Node::Type::ORDEREDLIST});
            break;
        }
        case XmlTextParser::Node::Type::LISTITEM: {
            if (data.lists.size() > 1) {
                data.ss << std::string((data.lists.size() - 1) * 4, ' ');
            }
            data.lists.back().counter++;
            if (data.lists.back().ordered && data.lists.size() == 1) {
                data.ss << data.lists.back().counter << ". ";
            } else {
                data.ss << "* ";
            }
            data.eol = false;
            break;
        }
        case XmlTextParser::Node::Type::ULINK:
        case XmlTextParser::Node::Type::REF: {
            data.ss << "[";
            data.eol = false;
            break;
        }
        case XmlTextParser::Node::Type::IMAGE: {
            const auto prefix = config.baseUrl + config.imagesFolder;
            data.ss << "![" << node->extra << "](" << prefix << (prefix.empty() ? "" : "/") << node->extra << ")";
            data.eol = false;
            if (config.copyImages) {
                std::ifstream src(Utils::join(inputDir, node->extra), std::ios::binary);
                if (src && config.useFolders && !config.imagesFolder.empty()) {
                    std::ofstream dst(Utils::join(config.outputDir, config.imagesFolder, node->extra), std::ios::binary);
                    if (dst) dst << src.rdbuf();
                } else if (src) {
                    std::ofstream dst(Utils::join(config.outputDir, node->extra), std::ios::binary);
                    if (dst) dst << src.rdbuf();
                }
            }
            break;
        }
        case XmlTextParser::Node::Type::COMPUTEROUTPUT: {
            data.ss << "`";
            data.eol = false;
            break;
        }
        case XmlTextParser::Node::Type::PROGRAMLISTING: {
            data.ss << "\n\n```cpp\n";
            data.eol = false;
            break;
        }
        case XmlTextParser::Node::Type::SP: {
            data.ss << " ";
            data.eol = false;
            break;
        }
        case XmlTextParser::Node::Type::HRULER: {
            data.ss << "\n\n------------------\n\n";
            data.eol = true;
            break;
        }
        case XmlTextParser::Node::Type::VARLISTENTRY: {
            data.ss << "\n";
            data.eol = true;
            break;
        }
        default: {
            break;
        }
    }

    for (size_t i = 0; i < node->children.size(); i++) {
        const auto childNext = i + 1 < node->children.size() ? &node->children[i + 1] : nullptr;
        const auto childPrevious = i > 0 ? &node->children[i - 1] : nullptr;
        print(data, node, &node->children[i], childPrevious, childNext);
    }

    switch (node->type) {
        case XmlTextParser::Node::Type::TITLE: {
            data.ss << "\n\n";
            data.eol = true;
            break;
        }
        case XmlTextParser::Node::Type::BOLD: {
            data.ss << "**";
            data.eol = false;
            break;
        }
        case XmlTextParser::Node::Type::EMPHASIS: {
            data.ss << "_";
            data.eol = false;
            break;
        }
        case XmlTextParser::Node::Type::STRIKE: {
            data.ss << "~~";
            data.eol = false;
            break;
        }
        case XmlTextParser::Node::Type::ULINK: {
            data.ss << "](" << node->extra << ")";
            data.eol = false;
            break;
        }
        case XmlTextParser::Node::Type::REF: {
            data.ss << "]";
            const auto found = doxygen.getCache().find(node->extra);
            if (found != doxygen.getCache().end()) {
                data.ss << "(" << found->second->getUrl() << ")";
            }
            data.eol = false;
            break;
        }
        case XmlTextParser::Node::Type::PARA: {
            /*auto c = back(ss);
            if (c != '\n') {
                ss << "\n";
            }
            if (parent && parent->type == XmlTextParser::Node::Type::LISTITEM) {
                break;
            } else {
                //ss << "\n";
            }*/
            if (!data.eol) {
                data.ss << "\n";
                data.eol = true;
            }
            break;
        }
        case XmlTextParser::Node::Type::COMPUTEROUTPUT: {
            data.ss << "`";
            data.eol = false;
            break;
        }
        case XmlTextParser::Node::Type::ITEMIZEDLIST:
        case XmlTextParser::Node::Type::VARIABLELIST:
        case XmlTextParser::Node::Type::ORDEREDLIST: {
            data.lists.pop_back();
            break;
        }
        case XmlTextParser::Node::Type::LISTITEM: {
            /*if (back(ss) != '\n') {
                ss << "\n";
            }*/
            //ss << "\n";
            break;
        }
        case XmlTextParser::Node::Type::CODELINE: {
            data.ss << "\n";
            data.eol = false;
            break;
        }
        case XmlTextParser::Node::Type::PROGRAMLISTING: {
            data.ss << "```\n\n";
            data.eol = true;
            break;
        }
        case XmlTextParser::Node::Type::VARLISTENTRY: {
            data.ss << "\n\n";
            data.eol = true;
            break;
        }
        default: {
            break;
        }
    }
}
