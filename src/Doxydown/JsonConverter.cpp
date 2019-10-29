#include <unordered_set>
#include "JsonConverter.hpp"
#include "Exception.hpp"
#include "TextUtils.hpp"
#include <set>

Doxydown::JsonConverter::JsonConverter(const Config& config, const TextPrinter& printer)
    : config(config), 
      printer(printer) {

}

nlohmann::json Doxydown::JsonConverter::convert(const Node::ClassReference& klass) const {
    nlohmann::json json;
    if (!klass.refid.empty())
        json["refid"] = klass.refid;
    json["name"] = klass.name;
    json["visibility"] = Node::visibilityToStr(klass.prot);
    json["virtual"] = Node::virtualToStr(klass.virt);
    if (!klass.url.empty())
        json["url"] = klass.url;
    return json;
}

nlohmann::json Doxydown::JsonConverter::convert(const Node::ClassReferences& klasses) const {
    nlohmann::json json = nlohmann::json::array();
    for (const auto& base : klasses) {
        json.push_back(convert(base));
    }
    return json;
}

nlohmann::json Doxydown::JsonConverter::convert(const Node::Location& location) const {
    nlohmann::json json;
    json["file"] = location.file;
    json["line"] = location.line;
    json["column"] = location.column;
    if (location.bodyStart > 0)
        json["bodyStart"] = location.bodyStart;
    if (location.bodyEnd > 0)
        json["bodyEnd"] = location.bodyEnd;
    if (!location.bodyFile.empty())
        json["bodyFile"] = location.bodyFile;
    return json;
}

nlohmann::json Doxydown::JsonConverter::convert(const Node::Param& param) const {
    nlohmann::json json;
    json["type"] = param.type;
    json["typePlain"] = param.typePlain;
    json["name"] = param.name;
    if (!param.defval.empty()) json["defval"] = param.defval;
    if (!param.defvalPlain.empty()) json["defvalPlain"] = param.defvalPlain;
    return json;
}

nlohmann::json Doxydown::JsonConverter::convert(const Node::DetailsSection& section) const {
    nlohmann::json json;
    json["text"] = section.text;
    json["type"] = section.type;
    return json;
}

nlohmann::json Doxydown::JsonConverter::convert(const Node::DetailsSections& sections) const {
    nlohmann::json json = nlohmann::json::array();
    for (const auto& section : sections) {
        json.push_back(convert(section));
    }
    return json;
}

nlohmann::json Doxydown::JsonConverter::convert(const Node& node) const {
    nlohmann::json json;
    json["name"] = node.getName();
    json["refid"] = node.getRefid();
    json["url"] = node.getUrl();
    json["anchor"] = node.getAnchor();
    if (!node.getBrief().empty())
        json["brief"] = node.getBrief();
    if (!node.getSummary().empty())
        json["summary"] = node.getSummary();
    json["kind"] = Node::kindToStr(node.getKind());
    json["type"] = Node::typeToStr(node.getType());
    if (!node.getBaseClasses().empty())
        json["baseClasses"] = convert(node.getBaseClasses());
    if (!node.getDerivedClasses().empty())
        json["derivedClasses"] = convert(node.getDerivedClasses());
    return json;
}

nlohmann::json Doxydown::JsonConverter::convert(const Node& node, const Node::Data& data) const {
    nlohmann::json json;
    if (!data.details.empty())
        json["details"] = data.details;
    if (!data.inbody.empty())
        json["inbody"] = data.inbody;
    if (!data.includes.empty())
        json["includes"] = data.includes;
    if (!data.type.empty())
        json["type"] = data.type;
    json["definition"] = data.definition;
    if (!data.initializer.empty())
        json["initializer"] = data.initializer;
    if (!data.typePlain.empty())
        json["typePlain"] = data.typePlain;
    if (!data.detailsSections.empty())
        json["detailsSections"] = convert(data.detailsSections);
    if (node.getKind() != Node::Kind::ENUMVALUE) {
        json["static"] = data.isStatic;
        json["abstract"] = data.isAbstract;
        json["visibility"] = Node::visibilityToStr(data.visibility);
        json["const"] = data.isConst;
        json["explicit"] = data.isExplicit;
        json["strong"] = data.isStrong;
        json["inline"] = data.isInline;
    }
    if (node.getType() == Node::Type::FUNCTIONS || node.getType() == Node::Type::FRIENDS) {
        json["virtual"] = data.virt == Node::Virtual::VIRTUAL || data.virt == Node::Virtual::PURE_VIRTUAL;
        json["pureVirtual"] = data.virt == Node::Virtual::PURE_VIRTUAL;
        json["argsString"] = data.argsString;
        json["default"] = data.isDefault;
        json["deleted"] = data.isDeleted;
        json["params"] = nlohmann::json::array();
        for (const auto& param : data.params) {
            json["params"].push_back(convert(param));
        }
    }
    if (!data.location.file.empty())
        json["location"] = convert(data.location);
    return json;
}

nlohmann::json Doxydown::JsonConverter::getAsJson(const Node& node) const {
    auto[data, childrenDataMap] = node.loadData(config, printer);

    nlohmann::json json = convert(node);
    nlohmann::json dataJson = convert(node, data);
    json.insert(dataJson.begin(), dataJson.end());
    if (node.getParent()->getKind() != Node::Kind::INDEX) {
        json["parent"] = convert(*node.getParent());
    } else {
        json["parent"] = nullptr;
    }

    static const std::array<Node::Visibility, 3> ALL_VISIBILITIES =
    {
        Node::Visibility::PUBLIC, 
        Node::Visibility::PROTECTED, 
        Node::Visibility::PRIVATE
    };

    // Find all unique groups (for example, public attributes)
    std::unordered_set<Node::Type> uniqueTypes;
    std::unordered_multimap<Node::Type, NodePtr> children;
    for (const auto& child : node.getChildren()) {
        children.insert(std::make_pair(child->getType(), child));
        uniqueTypes.insert(child->getType());
    }

    // public, protected, private...
    for (const auto& visibility : ALL_VISIBILITIES) {
        // attributes, functions, classes...
        for (const auto& type : uniqueTypes) {
            const auto key = Node::visibilityToStr(visibility) + TextUtils::title(Node::typeToStr(type));
            auto arr = nlohmann::json::array();
            const auto range = children.equal_range(type);
            for (auto it = range.first; it != range.second; ++it) {
                const auto& child = it->second;

                if (!child->isStructured() && child->getKind() != Node::Kind::MODULE) {
                    try {
                        const auto& childData = childrenDataMap.at(child.get());
                        if (childData.visibility == visibility) {
                            auto childJson = convert(*child);
                            auto childDataJson = convert(*child, childData);
                            childJson.insert(childDataJson.begin(), childDataJson.end());

                            if (child->getKind() == Node::Kind::ENUM) {
                                auto enumvalues = nlohmann::json::array();
                                for (const auto& enumvalue : child->getChildren()) {
                                    auto enumvalueJson = convert(*enumvalue);
                                    const auto& enumvalueData = childrenDataMap.at(child.get());
                                    auto enumvalueDataJson = convert(*enumvalue, enumvalueData);
                                    enumvalueJson.insert(childDataJson.begin(), childDataJson.end());
                                    enumvalues.push_back(std::move(enumvalueJson));
                                }
                                childJson["enumvalues"] = std::move(enumvalues);
                            }

                            arr.push_back(std::move(childJson));
                        }
                    } catch (std::out_of_range& e) {
                        (void)e;
                        throw EXCEPTION("Refid {} this should never happen please contact the author!", child->getRefid());
                    }
                } else {
                    arr.push_back(convert(*child));
                }
            }

            if (!arr.empty()) {
                if (type == Node::Type::FRIENDS) {
                    json["friends"] = std::move(arr);
                } else if (type == Node::Type::NAMESPACES) {
                    json["namespaces"] = std::move(arr);
                } else if (type == Node::Type::MODULES) {
                    json["groups"] = std::move(arr);
                } else {
                    json[key] = std::move(arr);
                }
            }
        }
    }

    return json;
}
