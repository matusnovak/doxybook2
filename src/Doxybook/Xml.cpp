#include <tinyxml2.h>
#include <Doxybook/Xml.hpp>
#include <Doxybook/Exception.hpp>
#include "ExceptionUtils.hpp"

Doxybook2::Xml::Node::Node(tinyxml2::XMLNode* ptr)
    : ptr(ptr) {

}

Doxybook2::Xml::Node Doxybook2::Xml::Node::nextSibling() const {
    return Node(ptr->NextSibling());
}

Doxybook2::Xml::Node Doxybook2::Xml::Node::firstChild() const {
    return Node(ptr->FirstChild());
}

bool Doxybook2::Xml::Node::hasText() const {
    return ptr->Value() != nullptr;
}

std::string Doxybook2::Xml::Node::getText() const {
    return ptr->Value();
}

bool Doxybook2::Xml::Node::isElement() const {
    return ptr->ToElement() != nullptr;
}

Doxybook2::Xml::Element Doxybook2::Xml::Node::asElement() const {
    return Element(ptr->ToElement());
}

Doxybook2::Xml::Element::Element(tinyxml2::XMLElement* ptr)
    : ptr(ptr) {
}

void Doxybook2::Xml::Element::allChildElements(const std::string& name, const ElementCallback& callback) const {
    auto found = firstChildElement(name);
    while(found) {
        callback(found);
        found = found.nextSiblingElement(name);
    }
}

Doxybook2::Xml::Element Doxybook2::Xml::Element::nextSiblingElement() const {
    return Element(ptr->NextSiblingElement());
}

Doxybook2::Xml::Node Doxybook2::Xml::Element::nextSibling() const {
    return Node(ptr->NextSibling());
}

Doxybook2::Xml::Element Doxybook2::Xml::Element::nextSiblingElement(const std::string& name) const {
    return Element(ptr->NextSiblingElement(name.c_str()));
}

Doxybook2::Xml::Node Doxybook2::Xml::Element::firstChild() const {
    return Node(ptr->FirstChild());
}

Doxybook2::Xml::Element Doxybook2::Xml::Element::firstChildElement() const {
    return Element(ptr->FirstChildElement());
}

Doxybook2::Xml::Element Doxybook2::Xml::Element::firstChildElement(const std::string& name) const {
    return Element(ptr->FirstChildElement(name.empty() ? nullptr : name.c_str()));
}

std::string Doxybook2::Xml::Element::getAttr(const std::string& name) const {
    const auto str = ptr->Attribute(name.c_str());
    if (str == nullptr) throw EXCEPTION("Attribute {} does not exist in element {}", name, ptr->Name());
    return str;
}

std::string Doxybook2::Xml::Element::getAttr(const std::string& name, const std::string& defaultValue) const {
    const auto str = ptr->Attribute(name.c_str());
    if (str == nullptr) return defaultValue;
    return str;
}

std::string Doxybook2::Xml::Element::getName() const {
    return ptr->Name();
}

bool Doxybook2::Xml::Element::hasText() const {
    return ptr->GetText() != nullptr;
}

std::string Doxybook2::Xml::Element::getText() const {
    return ptr->GetText();
}

Doxybook2::Xml::Node Doxybook2::Xml::Element::asNode() const {
    return Node(ptr);
}

int Doxybook2::Xml::Element::getLine() const {
    return ptr->GetLineNum();
}

const Doxybook2::Xml& Doxybook2::Xml::Element::getDocument() const {
    auto xml = reinterpret_cast<const Xml*>(ptr->GetDocument()->GetUserData());
    return *xml;
}

Doxybook2::Xml::Xml(const std::string& path) : doc(new tinyxml2::XMLDocument) {
    this->path = path;
    const auto err = doc->LoadFile(path.c_str());
    if (err != tinyxml2::XMLError::XML_SUCCESS) {
        throw EXCEPTION("{}", doc->ErrorStr());
    }
    doc->SetUserData(this);
}

Doxybook2::Xml::~Xml() = default;

Doxybook2::Xml::Element Doxybook2::Xml::firstChildElement(const std::string& name) const {
    return Element(doc->FirstChildElement(name.c_str()));
}
