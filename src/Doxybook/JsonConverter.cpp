#include "ExceptionUtils.hpp"
#include <Doxybook/Doxygen.hpp>
#include <Doxybook/Exception.hpp>
#include <Doxybook/JsonConverter.hpp>
#include <Doxybook/Utils.hpp>
#include <iostream>
#include <nlohmann/json.hpp>
#include <unordered_set>

static bool isFunctionType(const Doxybook2::Type type) {
    switch (type) {
        case Doxybook2::Type::FUNCTIONS:
        case Doxybook2::Type::FRIENDS:
        case Doxybook2::Type::SIGNALS:
        case Doxybook2::Type::SLOTS:
        case Doxybook2::Type::EVENTS:
            return true;
        default:
            return false;
    }
}

Doxybook2::JsonConverter::JsonConverter(const Config& config,
    const Doxygen& doxygen,
    const TextPrinter& plainPrinter,
    const TextPrinter& markdownPrinter)
    : config(config), doxygen(doxygen), plainPrinter(plainPrinter), markdownPrinter(markdownPrinter) {
}

nlohmann::json Doxybook2::JsonConverter::convert(const std::vector<std::string>& vec) const {
    nlohmann::json json = nlohmann::json::array();
    for (const auto& item : vec) {
        json.push_back(item);
    }
    return json;
}

nlohmann::json Doxybook2::JsonConverter::convert(const Node::ClassReference& klass) const {
    nlohmann::json json;
    if (!klass.refid.empty())
        json["refid"] = klass.refid;
    json["name"] = klass.name;
    json["visibility"] = toStr(klass.prot);
    json["virtual"] = toStr(klass.virt);
    json["external"] = klass.ptr == nullptr;
    if (klass.ptr)
        json["url"] = klass.ptr->getUrl();
    return json;
}

nlohmann::json Doxybook2::JsonConverter::convert(const Node::ClassReferences& klasses) const {
    nlohmann::json json = nlohmann::json::array();
    for (const auto& base : klasses) {
        json.push_back(convert(base));
    }
    return json;
}

nlohmann::json Doxybook2::JsonConverter::convert(const Node::Location& location) const {
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

nlohmann::json Doxybook2::JsonConverter::convert(const Node::Param& param) const {
    nlohmann::json json;
    json["type"] = param.type;
    json["typePlain"] = param.typePlain;
    json["name"] = param.name;
    if (!param.defval.empty())
        json["defval"] = param.defval;
    if (!param.defvalPlain.empty())
        json["defvalPlain"] = param.defvalPlain;
    return json;
}

nlohmann::json Doxybook2::JsonConverter::convert(const Node& node) const {
    nlohmann::json json;
    if (node.getKind() == Kind::FILE) {
        if (node.getParent()->getKind() == Kind::DIR) {
            json["name"] = node.getParent()->getName() + "/" + node.getName();
            json["title"] = json["name"];
        } else {
            json["name"] = node.getName();
            json["title"] = json["name"];
        }
    } else {
        json["name"] = node.getName();
        json["title"] = node.getTitle();
        if (node.getQualifiedName().empty()) {
            json["qualifiedname"] = node.getName();
        } else {
            json["qualifiedname"] = node.getQualifiedName();
        }
    }
    if (!node.isStructured() && node.getKind() != Kind::MODULE && node.getKind() != Kind::DEFINE &&
        node.getKind() != Kind::FILE && node.getKind() != Kind::DIR) {

        json["fullname"] = node.getParent()->getName() + "::" + node.getName();
    } else {
        json["fullname"] = json["name"];
    }
    json["refid"] = node.getRefid();
    json["url"] = node.getUrl();
    json["anchor"] = node.getAnchor();
    json["visibility"] = toStr(node.getVisibility());
    if (isFunctionType(node.getType())) {
        json["virtual"] = node.getVirtual() == Virtual::VIRTUAL || node.getVirtual() == Virtual::PURE_VIRTUAL;
        json["pureVirtual"] = node.getVirtual() == Virtual::PURE_VIRTUAL;
    }
    if (!node.getBrief().empty())
        json["brief"] = Utils::replaceNewline(node.getBrief());
    if (!node.getSummary().empty())
        json["summary"] = node.getSummary();
    json["language"] = node.getLanguage();
    json["kind"] = toStr(node.getKind());
    json["language"] = node.getLanguage();
    json["category"] = toStr(node.getType());
    if (!node.getBaseClasses().empty())
        json["baseClasses"] = convert(node.getBaseClasses());
    if (!node.getDerivedClasses().empty())
        json["derivedClasses"] = convert(node.getDerivedClasses());

    // language specific fixups
    if (node.getLanguage() == "java")
    {
        json["name"] = Utils::namespaceToPackage(json["name"]);
        json["fullname"] = Utils::namespaceToPackage(json["fullname"]);
        json["title"] = Utils::namespaceToPackage(json["title"]);

        if (node.getKind() == Kind::NAMESPACE)
            json["kind"] = "package";
    }

    return json;
}

nlohmann::json Doxybook2::JsonConverter::convert(const Node::ParameterListItem& parameterItem) const {
    nlohmann::json json;
    json["name"] = parameterItem.name;
    json["text"] = parameterItem.text;
    return json;
}

nlohmann::json Doxybook2::JsonConverter::convert(const Node::ParameterList& parameterList) const {
    nlohmann::json json = nlohmann::json::array();
    for (const auto& item : parameterList) {
        json.push_back(convert(item));
    }
    return json;
}

nlohmann::json Doxybook2::JsonConverter::convert(const Node& node, const Node::Data& data) const {
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
    if (!data.see.empty())
        json["see"] = convert(data.see);
    if (!data.returns.empty())
        json["returns"] = data.returns;
    if (!data.authors.empty())
        json["authors"] = convert(data.authors);
    if (!data.version.empty())
        json["version"] = data.version;
    if (!data.since.empty())
        json["since"] = data.since;
    if (!data.date.empty())
        json["date"] = data.date;
    if (!data.note.empty())
        json["note"] = data.note;
    if (!data.warning.empty())
        json["warning"] = data.warning;
    if (!data.pre.empty())
        json["pre"] = data.pre;
    if (!data.post.empty())
        json["post"] = data.post;
    if (!data.copyright.empty())
        json["copyright"] = data.copyright;
    if (!data.invariant.empty())
        json["invariant"] = data.invariant;
    if (!data.remark.empty())
        json["remark"] = data.remark;
    if (!data.attention.empty())
        json["attention"] = data.attention;
    if (!data.par.empty())
        json["par"] = data.par;
    if (!data.rcs.empty())
        json["rcs"] = data.rcs;
    if (!data.todos.empty())
        json["todos"] = data.todos;
    if (!data.bugs.empty())
        json["bugs"] = data.bugs;
    if (!data.tests.empty())
        json["tests"] = data.tests;
    if (!data.deprecated.empty())
        json["deprecated"] = data.deprecated;
    if (node.getKind() != Kind::ENUMVALUE) {
        json["static"] = data.isStatic;
        json["abstract"] = data.isAbstract;
        json["const"] = data.isConst;
        json["explicit"] = data.isExplicit;
        json["strong"] = data.isStrong;
        json["inline"] = data.isInline;
        json["override"] = data.isOverride;
    }
    if (isFunctionType(node.getType())) {
        json["argsString"] = data.argsString;
        json["default"] = data.isDefault;
        json["deleted"] = data.isDeleted;
        json["params"] = nlohmann::json::array();
        for (const auto& param : data.params) {
            json["params"].push_back(convert(param));
        }
    }
    if (node.getType() == Type::DEFINES && !data.params.empty()) {
        json["params"] = nlohmann::json::array();
        for (const auto& param : data.params) {
            json["params"].push_back(convert(param));
        }
    }
    if (!data.templateParams.empty()) {
        json["templateParams"] = nlohmann::json::array();
        for (const auto& param : data.templateParams) {
            json["templateParams"].push_back(convert(param));
        }
    }
    if (data.reimplements)
        json["reimplements"] = convert(*data.reimplements);
    if (!data.reimplementedBy.empty()) {
        auto arr = nlohmann::json::array();
        for (const auto& reimplementedBy : data.reimplementedBy) {
            arr.push_back(convert(*reimplementedBy));
        }
        json["reimplementedBy"] = std::move(arr);
    }
    if (!data.programlisting.empty()) {
        json["programlisting"] = data.programlisting;
    }
    if (!data.location.file.empty())
        json["location"] = convert(data.location);
    if (!data.returnsList.empty())
        json["returnsList"] = convert(data.returnsList);
    if (!data.exceptionsList.empty())
        json["exceptionsList"] = convert(data.exceptionsList);
    if (!data.templateParamsList.empty())
        json["templateParamsList"] = convert(data.templateParamsList);
    if (!data.paramList.empty())
        json["paramList"] = convert(data.paramList);
    json["hasDetails"] = !data.details.empty() || !data.templateParams.empty() || !data.inbody.empty() ||
                         !data.returnsList.empty() || !data.exceptionsList.empty() ||
                         !data.templateParamsList.empty() || !data.paramList.empty() || !data.see.empty() ||
                         !data.returns.empty() || !data.bugs.empty() || !data.tests.empty() || !data.todos.empty() ||
                         !data.authors.empty() || !data.version.empty() || !data.since.empty() || !data.date.empty() ||
                         !data.note.empty() || !data.warning.empty() || !data.pre.empty() || !data.post.empty() ||
                         !data.copyright.empty() || !data.invariant.empty() || !data.remark.empty() ||
                         !data.attention.empty() || !data.par.empty() || !data.rcs.empty() || !data.deprecated.empty();
    return json;
}

nlohmann::json Doxybook2::JsonConverter::getAsJson(const Node& node) const {
    auto [data, childrenDataMap] = node.loadData(config, plainPrinter, markdownPrinter, doxygen.getCache());

    nlohmann::json json = convert(node);
    nlohmann::json dataJson = convert(node, data);
    json.insert(dataJson.begin(), dataJson.end());
    if (node.getParent() != nullptr) {
        if (node.getParent()->getKind() != Kind::INDEX) {
            std::list<const Node*> list;
            auto parent = node.getParent();
            while (parent != nullptr) {
                list.push_front(parent);
                parent = parent->getParent();
                if (parent && parent->getKind() == Kind::INDEX)
                    parent = nullptr;
            }
            nlohmann::json breadcrumbs = nlohmann::json::array();
            for (const auto& ptr : list) {
                breadcrumbs.push_back(convert(*ptr));
            }
            json["parentBreadcrumbs"] = std::move(breadcrumbs);
            json["parent"] = convert(*node.getParent());
        } else {
            json["parent"] = nullptr;
        }
    }

    if (node.getGroup() != nullptr) {
        std::list<const Node*> list;
        auto group = node.getGroup();
        while (group != nullptr) {
            list.push_front(group);
            group = group->getGroup();
            if (group && group->getKind() == Kind::INDEX)
                group = nullptr;
        }
        nlohmann::json breadcrumbs = nlohmann::json::array();
        for (const auto& ptr : list) {
            breadcrumbs.push_back(convert(*ptr));
        }
        json["module"] = convert(*node.getGroup());
        json["moduleBreadcrumbs"] = std::move(breadcrumbs);
    }

    static const std::array<Visibility, 3> ALL_VISIBILITIES = {
        Visibility::PUBLIC, Visibility::PROTECTED, Visibility::PRIVATE};

    if (!node.getChildren().empty()) {
        // Find all unique groups (for example, public attributes)
        std::unordered_set<Type> uniqueTypes;
        std::unordered_map<Type, Node::Children> children;
        for (const auto& child : node.getChildren()) {
            auto it = children.find(child->getType());
            if (it == children.end()) {
                it = children.insert(std::make_pair(child->getType(), Node::Children{})).first;
            }
            it->second.push_back(child);
            uniqueTypes.insert(child->getType());
        }

        // public, protected, private...
        for (const auto& visibility : ALL_VISIBILITIES) {
            // attributes, functions, classes...
            for (const auto& type : uniqueTypes) {
                const auto key = toStr(visibility) + Utils::title(toStr(type));
                auto arr = nlohmann::json::array();
                const auto range = children.find(type);
                for (const auto& child : range->second) {
                    if (!child->isStructured() && child->getKind() != Kind::MODULE && child->getKind() != Kind::DIR &&
                        child->getKind() != Kind::FILE) {

                        try {
                            auto it = childrenDataMap.find(child.get()->getRefid());
                            if (it == childrenDataMap.end()) {
                                throw EXCEPTION("Child {} not found in data map", child.get()->getRefid());
                            }
                            const auto& childData = it->second;
                            if (child->getVisibility() == visibility) {
                                auto childJson = convert(*child);
                                auto childDataJson = convert(*child, childData);
                                childJson.insert(childDataJson.begin(), childDataJson.end());

                                if (child->getKind() == Kind::ENUM) {
                                    auto enumvalues = nlohmann::json::array();
                                    for (const auto& enumvalue : child->getChildren()) {
                                        auto enumvalueJson = convert(*enumvalue);
                                        const auto eit = childrenDataMap.find(enumvalue.get()->getRefid());
                                        if (eit == childrenDataMap.end()) {
                                            throw EXCEPTION("Child {} not found in data map", child.get()->getRefid());
                                        }
                                        const auto& enumvalueData = eit->second;
                                        auto enumvalueDataJson = convert(*enumvalue, enumvalueData);
                                        enumvalueJson.insert(enumvalueDataJson.begin(), enumvalueDataJson.end());
                                        enumvalues.push_back(std::move(enumvalueJson));
                                    }
                                    childJson["enumvalues"] = std::move(enumvalues);
                                }

                                arr.push_back(std::move(childJson));
                            }
                        } catch (std::out_of_range& e) {
                            (void)e;
                            throw EXCEPTION("Refid {} this should never happen please contact the author! Error: {}",
                                child->getRefid(),
                                e.what());
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
                    } else if (type == Type::DIRS) {
                        json["dirs"] = std::move(arr);
                    } else if (type == Type::FILES) {
                        json["files"] = std::move(arr);
                    } else if (type == Type::DEFINES) {
                        json["defines"] = std::move(arr);
                    } else {
                        json[key] = std::move(arr);
                    }
                }
            }
        }
    }

    const auto alreadyExists = [&](const std::string& name) -> bool {
        for (const auto& child : node.getChildren()) {
            if (child->getName() == name)
                return true;
        }
        return false;
    };

    auto hasAdditionalMembers = false;
    if (!node.getBaseClasses().empty()) {
        for (auto& base : json["baseClasses"]) {
            if (base["refid"].empty())
                continue;

            try {
                const auto baseNode = doxygen.getCache().at(base["refid"]);
                auto [baseData, baseChildrenDataMap] =
                    baseNode->loadData(config, plainPrinter, markdownPrinter, doxygen.getCache());

                // Get unique types of this base class
                std::unordered_set<Type> baseUniqueTypes;
                std::unordered_map<Type, Node::Children> baseChildren;
                for (const auto& child : baseNode->getChildren()) {
                    if (alreadyExists(child->getName())) {
                        continue;
                    }

                    auto it = baseChildren.find(child->getType());
                    if (it == baseChildren.end()) {
                        it = baseChildren.insert(std::make_pair(child->getType(), Node::Children{})).first;
                    }

                    it->second.push_back(child);
                    baseUniqueTypes.insert(child->getType());
                }

                // public, protected, private...
                for (const auto& visibility : ALL_VISIBILITIES) {
                    // attributes, functions, classes...
                    for (const auto& type : baseUniqueTypes) {
                        const auto key = toStr(visibility) + Utils::title(toStr(type));
                        auto arr = nlohmann::json::array();
                        const auto range = baseChildren.find(type);
                        for (const auto& child : range->second) {
                            if (alreadyExists(child->getName()))
                                continue;

                            if (!child->isStructured() && child->getKind() != Kind::MODULE) {
                                try {
                                    const auto it = baseChildrenDataMap.find(child.get()->getRefid());
                                    if (it == baseChildrenDataMap.end()) {
                                        throw EXCEPTION("Child {} not found in data map", child.get()->getRefid());
                                    }
                                    const auto& childData = it->second;
                                    if (child->getVisibility() == visibility) {
                                        auto childJson = convert(*child);
                                        auto childDataJson = convert(*child, childData);
                                        childJson.insert(childDataJson.begin(), childDataJson.end());

                                        if (child->getKind() == Kind::ENUM) {
                                            auto enumvalues = nlohmann::json::array();
                                            for (const auto& enumvalue : child->getChildren()) {
                                                auto enumvalueJson = convert(*enumvalue);
                                                const auto eit = baseChildrenDataMap.find(enumvalue.get()->getRefid());
                                                if (eit == baseChildrenDataMap.end()) {
                                                    throw EXCEPTION(
                                                        "Child {} not found in data map", child.get()->getRefid());
                                                }
                                                const auto& enumvalueData = it->second;
                                                auto enumvalueDataJson = convert(*enumvalue, enumvalueData);
                                                enumvalueJson.insert(
                                                    enumvalueDataJson.begin(), enumvalueDataJson.end());
                                                enumvalues.push_back(std::move(enumvalueJson));
                                            }
                                            childJson["enumvalues"] = std::move(enumvalues);
                                        }

                                        arr.push_back(std::move(childJson));
                                    }
                                } catch (std::out_of_range& e) {
                                    (void)e;
                                    throw EXCEPTION("Refid {} this should never happen please contact the author!",
                                        child->getRefid());
                                }
                            } else {
                                if (child->getVisibility() == visibility) {
                                    arr.push_back(convert(*child));
                                }
                            }
                        }

                        if (!arr.empty()) {
                            hasAdditionalMembers = true;
                            if (type == Type::FRIENDS) {
                                base["friends"] = std::move(arr);
                            } else if (type == Type::NAMESPACES) {
                                base["namespaces"] = std::move(arr);
                            } else if (type == Type::MODULES) {
                                base["groups"] = std::move(arr);
                            } else {
                                base[key] = std::move(arr);
                            }
                        }
                    }
                }
            } catch (std::out_of_range& e) {
                (void)e;
                throw EXCEPTION(
                    "Refid {} this should never happen please contact the author!", base["refid"].get<std::string>());
            } catch (std::exception& e) {
                throw EXCEPTION("Something went wrong while processing base class {} of {} error {}",
                    base["refid"].get<std::string>(),
                    node.getRefid(),
                    e.what());
            }
        }
    }
    json["hasAdditionalMembers"] = hasAdditionalMembers;

    return json;
}
