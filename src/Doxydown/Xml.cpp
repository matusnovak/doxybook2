#include <tinyxml2.h>
#include "Xml.hpp"
#include "Exception.hpp"

Doxydown::Xml::Node::Node(tinyxml2::XMLNode* ptr)
    : ptr(ptr) {

}

Doxydown::Xml::Node Doxydown::Xml::Node::nextSibling() const {
    return Node(ptr->NextSibling());
}

Doxydown::Xml::Node Doxydown::Xml::Node::firstChild() const {
    return Node(ptr->FirstChild());
}

bool Doxydown::Xml::Node::hasText() const {
    return ptr->Value() != nullptr;
}

std::string Doxydown::Xml::Node::getText() const {
    return ptr->Value();
}

bool Doxydown::Xml::Node::isElement() const {
    return ptr->ToElement() != nullptr;
}

Doxydown::Xml::Element Doxydown::Xml::Node::asElement() const {
    return Element(ptr->ToElement());
}

Doxydown::Xml::Element::Element(tinyxml2::XMLElement* ptr)
    : ptr(ptr) {
}

void Doxydown::Xml::Element::allChildElements(const std::string& name, const ElementCallback& callback) const {
    auto found = firstChildElement(name);
    while(found) {
        callback(found);
        found = found.nextSiblingElement(name);
    }
}

Doxydown::Xml::Element Doxydown::Xml::Element::nextSiblingElement() const {
    return Element(ptr->NextSiblingElement());
}

Doxydown::Xml::Node Doxydown::Xml::Element::nextSibling() const {
    return Node(ptr->NextSibling());
}

Doxydown::Xml::Element Doxydown::Xml::Element::nextSiblingElement(const std::string& name) const {
    return Element(ptr->NextSiblingElement(name.c_str()));
}

Doxydown::Xml::Node Doxydown::Xml::Element::firstChild() const {
    return Node(ptr->FirstChild());
}

Doxydown::Xml::Element Doxydown::Xml::Element::firstChildElement() const {
    return Element(ptr->FirstChildElement());
}

Doxydown::Xml::Element Doxydown::Xml::Element::firstChildElement(const std::string& name) const {
    return Element(ptr->FirstChildElement(name.empty() ? nullptr : name.c_str()));
}

std::string Doxydown::Xml::Element::getAttr(const std::string& name) const {
    const auto str = ptr->Attribute(name.c_str());
    if (str == nullptr) throw XML_EXCEPTION("Attribute {} does not exist in element {}", name, ptr->Name());
    return str;
}

std::string Doxydown::Xml::Element::getAttr(const std::string& name, const std::string& defaultValue) const {
    const auto str = ptr->Attribute(name.c_str());
    if (str == nullptr) return defaultValue;
    return str;
}

std::string Doxydown::Xml::Element::getName() const {
    return ptr->Name();
}

bool Doxydown::Xml::Element::hasText() const {
    return ptr->GetText() != nullptr;
}

std::string Doxydown::Xml::Element::getText() const {
    return ptr->GetText();
}

Doxydown::Xml::Node Doxydown::Xml::Element::asNode() const {
    return Node(ptr);
}

int Doxydown::Xml::Element::getLine() const {
    return ptr->GetLineNum();
}

const Doxydown::Xml& Doxydown::Xml::Element::getDocument() const {
    auto xml = reinterpret_cast<const Xml*>(ptr->GetDocument()->GetUserData());
    return *xml;
}

Doxydown::Xml::Xml(const std::string& path) : doc(new tinyxml2::XMLDocument) {
    this->path = path;
    const auto err = doc->LoadFile(path.c_str());
    if (err != tinyxml2::XMLError::XML_SUCCESS) {
        throw XML_EXCEPTION("{}", doc->ErrorStr());
    }
    doc->SetUserData(this);
}

Doxydown::Xml::~Xml() = default;

Doxydown::Xml::Element Doxydown::Xml::firstChildElement(const std::string& name) const {
    return Element(doc->FirstChildElement(name.c_str()));
}
