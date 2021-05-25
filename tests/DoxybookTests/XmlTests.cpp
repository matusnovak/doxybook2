#include <Doxybook/Xml.hpp>
#include <catch2/catch.hpp>

using namespace Doxybook2;

TEST_CASE("Parse simple document", "Xml") {
    static const std::string raw = R"(<bookstore>  
      <book category="COOKING">  
        <title lang="en">Everyday Italian</title>  
        <author>Giada De Laurentiis</author>  
        <year>2005</year>  
        <price>30.00</price>  
      </book>  
      <book category="CHILDREN">  
        <title lang="en">Harry Potter</title>  
        <author>J K. Rowling</author>  
        <year>2005</year>  
        <price>29.99</price>  
      </book>  
      <book category="WEB">  
        <title lang="en">Learning XML</title>  
        <author>Erik T. Ray</author>  
        <year>2003</year>  
        <price>39.95</price>  
      </book>  
    </bookstore>)";

    Xml temp(raw);
    Xml xml = std::move(temp);

    auto root = xml.root();
    REQUIRE(root.getName() == "bookstore");
    REQUIRE(root.hasText() == false);

    root = xml.firstChildElement("bookstore");
    REQUIRE(root.getName() == "bookstore");
    REQUIRE(root.hasText() == false);

    auto cooking = root.firstChildElement("book");
    REQUIRE(!!cooking);
    REQUIRE(cooking.getName() == "book");
    REQUIRE(cooking.getAttr("category") == "COOKING");
    REQUIRE_THROWS(cooking.getAttr("unknown"));

    cooking = root.firstChildElement();
    REQUIRE(!!cooking);
    REQUIRE(cooking.getName() == "book");

    const auto title = cooking.firstChildElement("title");
    REQUIRE(title.getName() == "title");
    REQUIRE(title.hasText() == true);
    REQUIRE(title.getText() == "Everyday Italian");

    const auto author = title.nextSiblingElement();
    REQUIRE(!!author);
    REQUIRE(author.getName() == "author");

    const auto year = author.nextSiblingElement();
    REQUIRE(!!year);
    REQUIRE(year.getName() == "year");

    const auto price = year.nextSiblingElement();
    REQUIRE(!!price);
    REQUIRE(price.getName() == "price");

    const auto invalid = price.nextSiblingElement();
    REQUIRE(!invalid);
}
