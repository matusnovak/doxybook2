#include <unordered_set>
#include <nlohmann/json.hpp>
#include <Doxydown/JsonConverter.hpp>
#include <Doxydown/Exception.hpp>
#include <Doxydown/Utils.hpp>
#include "ExceptionUtils.hpp"

Doxydown::JsonConverter::JsonConverter(const Config& config,
                                       const TextPrinter& plainPrinter,
                                       const TextPrinter& markdownPrinter)
    : config(config),
      plainPrinter(plainPrinter),
      markdownPrinter(markdownPrinter) {

}

nlohmann::json Doxydown::JsonConverter::convert(const Node::ClassReference& klass) const {
    nlohmann::json json;
    if (!klass.refid.empty())
        json["refid"] = klass.refid;
    json["name"] = klass.name;
    json["visibility"] = toStr(klass.prot);
    json["virtual"] = toStr(klass.virt);
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
    json["visibility"] = toStr(node.getVisibility());
    if (!node.getBrief().empty())
        json["brief"] = node.getBrief();
    if (!node.getSummary().empty())
        json["summary"] = node.getSummary();
    json["kind"] = toStr(node.getKind());
    json["type"] = toStr(node.getType());
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
    if (node.getKind() != Kind::ENUMVALUE) {
        json["static"] = data.isStatic;
        json["abstract"] = data.isAbstract;
        json["const"] = data.isConst;
        json["explicit"] = data.isExplicit;
        json["strong"] = data.isStrong;
        json["inline"] = data.isInline;
    }
    if (node.getType() == Type::FUNCTIONS || node.getType() == Type::FRIENDS) {
        json["virtual"] = data.virt == Virtual::VIRTUAL || data.virt == Virtual::PURE_VIRTUAL;
        json["pureVirtual"] = data.virt == Virtual::PURE_VIRTUAL;
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
    auto[data, childrenDataMap] = node.loadData(config, plainPrinter, markdownPrinter);

    nlohmann::json json = convert(node);
    nlohmann::json dataJson = convert(node, data);
    json.insert(dataJson.begin(), dataJson.end());
    if (node.getParent()->getKind() != Kind::INDEX) {
        json["parent"] = convert(*node.getParent());
    } else {
        json["parent"] = nullptr;
    }

    static const std::array<Visibility, 3> ALL_VISIBILITIES =
    {
        Visibility::PUBLIC, 
        Visibility::PROTECTED, 
        Visibility::PRIVATE
    };

    // Find all unique groups (for example, public attributes)
    std::unordered_set<Type> uniqueTypes;
    std::unordered_multimap<Type, NodePtr> children;
    for (const auto& child : node.getChildren()) {
        children.insert(std::make_pair(child->getType(), child));
        uniqueTypes.insert(child->getType());
    }

    // public, protected, private...
    for (const auto& visibility : ALL_VISIBILITIES) {
        // attributes, functions, classes...
        for (const auto& type : uniqueTypes) {
            const auto key = toStr(visibility) + Utils::title(toStr(type));
            auto arr = nlohmann::json::array();
            const auto range = children.equal_range(type);
            for (auto it = range.first; it != range.second; ++it) {
                const auto& child = it->second;

                if (!child->isStructured() && child->getKind() != Kind::MODULE) {
                    try {
                        const auto& childData = childrenDataMap.at(child.get());
                        if (child->getVisibility() == visibility) {
                            auto childJson = convert(*child);
                            auto childDataJson = convert(*child, childData);
                            childJson.insert(childDataJson.begin(), childDataJson.end());

                            if (child->getKind() == Kind::ENUM) {
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
                    if (child->getVisibility() == visibility) {
                        arr.push_back(convert(*child));
                    }
                }
            }

            if (!arr.empty()) {
                if (type == Type::FRIENDS) {
                    json["friends"] = std::move(arr);
                } else if (type == Type::NAMESPACES) {
                    json["namespaces"] = std::move(arr);
                } else if (type == Type::MODULES) {
                    json["groups"] = std::move(arr);
                } else {
                    json[key] = std::move(arr);
                }
            }
        }
    }

    return json;
}
