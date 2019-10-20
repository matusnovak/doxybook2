#include "JsonConverter.hpp"
#include "Exception.hpp"

std::string Doxydown::JsonConverter::toStr(const Node::Kind kind) {
    static std::unordered_map<Node::Kind, std::string> values = {
        {Node::Kind::CLASS, "class"},
        {Node::Kind::DIR, "dir"},
        {Node::Kind::ENUM, "enum"},
        {Node::Kind::ENUMVALUE, "enumvalue"},
        {Node::Kind::FILE, "file"},
        {Node::Kind::FUNCTION, "function"},
        {Node::Kind::GROUP, "group"},
        {Node::Kind::INDEX, "index"},
        {Node::Kind::INTERFACE, "interface"},
        {Node::Kind::NAMESPACE, "namespace"},
        {Node::Kind::STRUCT, "struct"},
        {Node::Kind::VARIABLE, "variable"}
    };

    const auto it = values.find(kind);
    if (it == values.end()) {
        throw EXCEPTION("Kind \"{}\" not recognised, please contact the author", kind);
    }

    return it->second;
}

std::string Doxydown::JsonConverter::toStr(const Node::Visibility prot) {
    static std::unordered_map<Node::Visibility, std::string> values = {
        {Node::Visibility::PUBLIC, "public"},
        {Node::Visibility::PROTECTED, "protected"},
        {Node::Visibility::PRIVATE, "private"},
    };

    const auto it = values.find(prot);
    if (it == values.end()) {
        throw EXCEPTION("Visibility \"{}\" not recognised, please contact the author", prot);
    }

    return it->second;
}

std::string Doxydown::JsonConverter::toStr(const Node::Virtual virt) {
    static std::unordered_map<Node::Virtual, std::string> values = {
        {Node::Virtual::NON_VIRTUAL, "non-virtual"},
        {Node::Virtual::VIRTUAL, "virtual"},
        {Node::Virtual::PURE_VIRTUAL, "pure-virtual"},
    };

    const auto it = values.find(virt);
    if (it == values.end()) {
        throw EXCEPTION("Virtuality \"{}\" not recognised, please contact the author", virt);
    }

    return it->second;
}

nlohmann::json Doxydown::JsonConverter::convert(const Node::DerivedClass& klass) {
    nlohmann::json data;
    data["refid"] = klass.ptr->getRefid();
    data["name"] = klass.ptr->getName();
    if (!klass.ptr->getData().brief.empty())
        data["brief"] = klass.ptr->getData().brief;
    data["kind"] = toStr(klass.ptr->getKind());
    data["visibility"] = toStr(klass.prot);
    data["virtual"] = toStr(klass.virt);
    return data;
}

nlohmann::json Doxydown::JsonConverter::convert(const Node::BaseClass& klass) {
    nlohmann::json data;
    data["refid"] = klass.ptr->getRefid();
    data["name"] = klass.ptr->getName();
    if (!klass.ptr->getData().brief.empty()) 
        data["brief"] = klass.ptr->getData().brief;
    data["kind"] = toStr(klass.ptr->getKind());
    data["visibility"] = toStr(klass.prot);
    data["virtual"] = toStr(klass.virt);
    return data;
}

nlohmann::json Doxydown::JsonConverter::convert(const Node::BaseClasses& klasses) {
    nlohmann::json data = nlohmann::json::array();
    for (const auto& base : klasses) {
        data.push_back(convert(base));
    }
    return data;
}

nlohmann::json Doxydown::JsonConverter::convert(const Node::DerivedClasses& klasses) {
    nlohmann::json data = nlohmann::json::array();
    for (const auto& derived : klasses) {
        data.push_back(convert(derived));
    }
    return data;
}

nlohmann::json Doxydown::JsonConverter::convert(const Node::Location& location) {
    nlohmann::json data;
    data["file"] = location.file;
    data["line"] = location.line;
    data["column"] = location.column;
    if (location.bodyStart > 0)
        data["bodyStart"] = location.bodyStart;
    if (location.bodyEnd > 0)
        data["bodyEnd"] = location.bodyEnd;
    if (!location.bodyFile.empty())
        data["bodyFile"] = location.bodyFile;
    return data;
}

nlohmann::json Doxydown::JsonConverter::convert(const Node& node, const bool deep) {
    nlohmann::json data;
    data["name"] = node.getName();
    data["refid"] = node.getRefid();
    if (!node.getData().brief.empty()) 
        data["brief"] = node.getData().brief;
    data["kind"] = toStr(node.getKind());
    data["static"] = node.getData().isStatic;
    data["abstract"] = node.getData().isAbstract;
    data["visibility"] = toStr(node.getData().prot);
    data["virtual"] = toStr(node.getData().virt);
    if (!node.getData().location.file.empty())
        data["location"] = convert(node.getData().location);
    if (!node.getData().baseClasses.empty())
        data["baseClasses"] = convert(node.getData().baseClasses);
    if (!node.getData().derivedClasses.empty())
        data["derivedClasses"] = convert(node.getData().derivedClasses);
    if (deep && !node.getChildren().empty())
        data["children"] = convert(node.getChildren(), false);
    if (deep && node.getParent()->getKind() != Node::Kind::INDEX && node.getKind() != Node::Kind::ENUM)
        data["parent"] = convert(*node.getParent(), false);
    return data;
}

nlohmann::json Doxydown::JsonConverter::convert(const Node::Children& children, const bool deep) {
    nlohmann::json data = nlohmann::json::array();
    for (const auto& child : children) {
        const auto goDeep = deep || child->getKind() == Node::Kind::ENUM;
        data.push_back(convert(*child, goDeep));
    }
    return data;
}

void Doxydown::JsonConverter::decorate(const Node& node, nlohmann::json& data) {
    
}
