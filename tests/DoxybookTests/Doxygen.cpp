#include <Doxybook/Doxygen.hpp>
#include <Doxybook/Exception.hpp>
#include <catch2/catch.hpp>
#include <fmt/format.h>
#include <iostream>

using namespace Doxybook2;

static Xml parse(const char* src) {
    auto s = std::string(src);
    Xml xml(s);
    return xml;
}

static TextNode asTextPara(const std::string& s) {
    return Text::NodeCompound{
        Text::Type::Text,
        {},
        {
            Text::NodeCompound{
                Text::Type::Paragraph,
                {},
                {
                    Text::Plain{s},
                },
            },
        },
    };
}

TEST_CASE("Parse simple compound", "Doxygen") {
    const auto xml = parse(R"(<?xml version='1.0' encoding='UTF-8' standalone='no'?>
    <doxygen xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="compound.xsd" version="1.9.1" xml:lang="en-US">
    <compounddef id="classEngine_1_1Utils_1_1ArrayView" kind="class" language="C++" prot="public">
        <compoundname>Engine::Utils::ArrayView</compoundname>
        <includes refid="ArrayView_8hpp" local="no">ArrayView.hpp</includes>
        <templateparamlist>
        <param>
            <type>typename T</type>
            <defval>uint8_t</defval>
        </param>
        </templateparamlist>
        <briefdescription>
    <para>Filesystem path utility functions. </para>
        </briefdescription>
        <detaileddescription>
    <para><parameterlist kind="templateparam"><parameteritem>
    <parameternamelist>
    <parametername>T</parametername>
    </parameternamelist>
    <parameterdescription>
    <para>The type of the data, for example uint8_t </para>
    </parameterdescription>
    </parameteritem>
    </parameterlist>
    </para>
        </detaileddescription>
        <location file="src/Utils/ArrayView.hpp" line="11" column="1" bodyfile="src/Utils/ArrayView.hpp" bodystart="11" bodyend="18"/>
        <listofallmembers>
        </listofallmembers>
    </compounddef>
    </doxygen>
    )");
    const auto root = xml.root();

    const NodeSharedPtr compound = Doxygen::parse(root);
    REQUIRE(!!compound);

    REQUIRE(compound->kind == Kind::CLASS);
    REQUIRE(compound->refid == "classEngine_1_1Utils_1_1ArrayView");
    REQUIRE(compound->name == "Engine::Utils::ArrayView");
    REQUIRE(compound->title == TextNode{Text::Plain{"Engine::Utils::ArrayView"}});
    REQUIRE(compound->brief == asTextPara("Filesystem path utility functions. "));

    REQUIRE(compound->properties.visibility == Visibility::PUBLIC);
    REQUIRE(compound->properties.virt == Virtual::NON_VIRTUAL);
    REQUIRE(compound->properties.definition == "");
    REQUIRE(compound->properties.argsString == "");
    REQUIRE(compound->properties.initializer.index() == 0);
    REQUIRE(compound->properties.isAbstract == false);
    REQUIRE(compound->properties.isStatic == false);
    REQUIRE(compound->properties.isConst == false);
    REQUIRE(compound->properties.isStrong == false);
    REQUIRE(compound->properties.isInline == false);
    REQUIRE(compound->properties.isDefault == false);
    REQUIRE(compound->properties.isDeleted == false);
    REQUIRE(compound->properties.isOverride == false);
    REQUIRE(compound->properties.isFinal == false);
    REQUIRE(compound->properties.isSealed == false);
    REQUIRE(compound->properties.params.empty());
    REQUIRE(compound->properties.templateParams.size() == 1);

    const auto& tp = compound->properties.templateParams.front();
    REQUIRE(tp.type == TextNode{Text::Plain{"typename T"}});
    REQUIRE(tp.name.empty());
    REQUIRE(tp.defval == TextNode{Text::Plain{"uint8_t"}});

    const auto& location = compound->location;
    REQUIRE(location.has_value());
    REQUIRE(location.value().file.empty() == false);
    REQUIRE(location.value().file.resolved() == false);
    REQUIRE(location.value().file.asBasicRef().refid == "");
    REQUIRE(location.value().file.asBasicRef().name == "src/Utils/ArrayView.hpp");
    REQUIRE(location.value().line == 11);
    REQUIRE(location.value().column == 1);

    const auto& bodyLocation = compound->bodyLocation;
    REQUIRE(bodyLocation.has_value());
    REQUIRE(bodyLocation.value().file.empty() == false);
    REQUIRE(bodyLocation.value().file.resolved() == false);
    REQUIRE(bodyLocation.value().file.asBasicRef().refid == "");
    REQUIRE(bodyLocation.value().file.asBasicRef().name == "src/Utils/ArrayView.hpp");
    REQUIRE(bodyLocation.value().start == 11);
    REQUIRE(bodyLocation.value().end == 18);
}

TEST_CASE("Parse simple memberdef", "Doxygen") {
    const auto xml = parse(R"(<?xml version='1.0' encoding='UTF-8' standalone='no'?>
    <memberdef kind="function" id="classEngine_1_1Utils_1_1ArrayView_1ab73615399bbbb9fd6246fa3982f2cfec" prot="public" static="no" const="no" explicit="no" inline="no" virt="non-virtual">
        <type></type>
        <definition>Engine::Utils::ArrayView&lt; T &gt;::ArrayView</definition>
        <argsstring>(const T *src, size_t len)</argsstring>
        <name>ArrayView</name>
        <param>
          <type>const T *</type>
          <declname>src</declname>
        </param>
        <param>
          <type>size_t</type>
          <declname>len</declname>
        </param>
        <briefdescription>
        </briefdescription>
        <detaileddescription>
        </detaileddescription>
        <inbodydescription>
        </inbodydescription>
        <location file="src/Utils/ArrayView.hpp" line="13" column="1"/>
      </memberdef>
    )");

    const auto root = xml.root();

    const NodeSharedPtr member = Doxygen::parseMember(root);
    REQUIRE(!!member);

    REQUIRE(member->kind == Kind::FUNCTION);
    REQUIRE(member->refid == "classEngine_1_1Utils_1_1ArrayView_1ab73615399bbbb9fd6246fa3982f2cfec");
    REQUIRE(member->name == "ArrayView");
    REQUIRE(member->title == TextNode{Text::Plain{"ArrayView"}});
    REQUIRE(member->brief == TextNode{});

    REQUIRE(member->properties.visibility == Visibility::PUBLIC);
    REQUIRE(member->properties.virt == Virtual::NON_VIRTUAL);
    REQUIRE(member->properties.definition == "Engine::Utils::ArrayView< T >::ArrayView");
    REQUIRE(member->properties.argsString == "(const T *src, size_t len)");
    REQUIRE(member->properties.initializer == TextNode{});
    REQUIRE(member->properties.isAbstract == false);
    REQUIRE(member->properties.isStatic == false);
    REQUIRE(member->properties.isConst == false);
    REQUIRE(member->properties.isStrong == false);
    REQUIRE(member->properties.isInline == false);
    REQUIRE(member->properties.isDefault == false);
    REQUIRE(member->properties.isDeleted == false);
    REQUIRE(member->properties.isOverride == false);
    REQUIRE(member->properties.isFinal == false);
    REQUIRE(member->properties.isSealed == false);
    REQUIRE(member->properties.params.size() == 2);
    REQUIRE(member->properties.templateParams.empty());

    const auto& p0 = member->properties.params.at(0);
    const auto& p1 = member->properties.params.at(1);

    REQUIRE(p0.type == TextNode{Text::Plain{"const T *"}});
    REQUIRE(p0.name == "src");
    REQUIRE(p0.defval == TextNode{});

    REQUIRE(p1.type == TextNode{Text::Plain{"size_t"}});
    REQUIRE(p1.name == "len");
    REQUIRE(p1.defval == TextNode{});

    const auto& location = member->location;
    REQUIRE(location.has_value());
    REQUIRE(location.value().file.empty() == false);
    REQUIRE(location.value().file.resolved() == false);
    REQUIRE(location.value().file.asBasicRef().refid == "");
    REQUIRE(location.value().file.asBasicRef().name == "src/Utils/ArrayView.hpp");
    REQUIRE(location.value().line == 13);
    REQUIRE(location.value().column == 1);
}

static const Text::MarkdownOptions defaultOptons{
    "",
    [](const std::string& refid) -> std::optional<std::string> { return "some_link.md"; },
};

TEST_CASE("Params with refs", "Doxygen") {
    const auto xml = parse(R"(<?xml version='1.0' encoding='UTF-8' standalone='no'?>
    <memberdef kind="friend" id="classEngine_1_1Audio_1_1AudioBuffer_1a8a93ebadfbe0508d87d2605f062b4b7f" prot="public" static="no" const="no" explicit="no" inline="no" virt="non-virtual">
        <type>void</type>
        <definition>void Audio::doSomething</definition>
        <argsstring>(AudioBuffer &amp;buffer)</argsstring>
        <name>Audio::doSomething</name>
        <param>
          <type><ref refid="classEngine_1_1Audio_1_1AudioBuffer" kindref="compound">AudioBuffer</ref> &amp;</type>
          <declname>buffer</declname>
        </param>
        <briefdescription>
        </briefdescription>
        <detaileddescription>
        </detaileddescription>
        <inbodydescription>
        </inbodydescription>
        <location file="src/Audio/AudioBuffer.hpp" line="139" column="25"/>
      </memberdef>
    )");

    const auto root = xml.root();

    const NodeSharedPtr member = Doxygen::parseMember(root);
    REQUIRE(!!member);

    REQUIRE(member->properties.params.size() == 1);
    const auto& param = member->properties.params.front();

    REQUIRE(Text::printMarkdown(param.type, defaultOptons) == "[AudioBuffer](some_link.md) &");
}

TEST_CASE("Explicit constructor with brief", "Doxygen") {
    const auto xml = parse(R"(<?xml version='1.0' encoding='UTF-8' standalone='no'?>
    <memberdef kind="function" id="classEngine_1_1Audio_1_1AudioBuffer_1ab3f8002fc80d9bff50cfb6095e10a721" prot="public" static="no" const="no" explicit="yes" inline="no" virt="non-virtual">
        <type></type>
        <definition>Engine::Audio::AudioBuffer::AudioBuffer</definition>
        <argsstring>(const std::string &amp;filename)</argsstring>
        <name>AudioBuffer</name>
        <param>
          <type>const std::string &amp;</type>
          <declname>filename</declname>
        </param>
        <briefdescription>
<para>Constructor for <ref refid="classEngine_1_1Audio_1_1AudioBuffer" kindref="compound">Audio::AudioBuffer</ref>. </para>
        </briefdescription>
        <detaileddescription>
        </detaileddescription>
        <inbodydescription>
        </inbodydescription>
        <location file="src/Audio/AudioBuffer.hpp" line="103" column="22"/>
      </memberdef>
    )");

    const auto root = xml.root();

    const NodeSharedPtr member = Doxygen::parseMember(root);
    REQUIRE(!!member);

    REQUIRE(member->properties.isExplicit == true);

    const auto brief = Text::printMarkdown(member->brief, defaultOptons);
    REQUIRE(brief == "Constructor for [Audio::AudioBuffer](some_link.md). ");
}

TEST_CASE("Parse group", "Doxygen") {
    const auto xml = parse(R"(<?xml version='1.0' encoding='UTF-8' standalone='no'?>
    <doxygen xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="compound.xsd" version="1.9.1" xml:lang="en-US">
    <compounddef id="group__Graphics" kind="group">
        <compoundname>Graphics</compoundname>
        <title>Graphical related classes</title>
        <innerclass refid="classEngine_1_1Graphics_1_1Framebuffer" prot="public">Engine::Graphics::Framebuffer</innerclass>
        <innerclass refid="classEngine_1_1Graphics_1_1Handle" prot="public">Engine::Graphics::Handle</innerclass>
        <innerclass refid="classEngine_1_1Graphics_1_1Texture2D" prot="public">Engine::Graphics::Texture2D</innerclass>
        <innerclass refid="classEngine_1_1Graphics_1_1Texture3D" prot="public">Engine::Graphics::Texture3D</innerclass>
        <innerclass refid="classEngine_1_1Graphics_1_1Texture" prot="public">Engine::Graphics::Texture</innerclass>
        <sectiondef kind="func">
        <memberdef kind="function" id="group__Graphics_1gae5b9a93609bed06adb3ce5902791df8a" prot="public" static="no" const="no" explicit="no" inline="yes" virt="non-virtual">
            <type>int</type>
            <definition>int Engine::Graphics::getHandle</definition>
            <argsstring>(Handle &amp;handle)</argsstring>
            <name>getHandle</name>
            <param>
            <type>Handle &amp;</type>
            <declname>handle</declname>
            </param>
            <briefdescription>
    <para>Some namespace inline function. </para>
            </briefdescription>
            <detaileddescription>
            </detaileddescription>
            <inbodydescription>
            </inbodydescription>
            <location file="src/Graphics/Handle.hpp" line="26" column="20" bodyfile="src/Graphics/Handle.hpp" bodystart="26" bodyend="28"/>
        </memberdef>
        </sectiondef>
        <briefdescription>
    <para>This is a brief description to the graphics group. </para>
        </briefdescription>
        <detaileddescription>
    <para>Some detailed description here </para>
        </detaileddescription>
    </compounddef>
    </doxygen>
    )");

    const auto root = xml.root();

    const NodeSharedPtr compound = Doxygen::parse(root);
    REQUIRE(!!compound);

    REQUIRE(compound->kind == Kind::MODULE);

    REQUIRE(Text::printMarkdown(compound->title, defaultOptons) == "Graphical related classes");

    const auto& inner = compound->childrenRefs;
    REQUIRE(inner.size() == 5);
    REQUIRE(inner.front().asBasicRef().refid == "classEngine_1_1Graphics_1_1Framebuffer");

    const auto& children = compound->children;
    REQUIRE(children.size() == 1);

    const auto& func = children.front();
    REQUIRE(func->kind == Kind::FUNCTION);
    REQUIRE(func->refid == "group__Graphics_1gae5b9a93609bed06adb3ce5902791df8a");
}

TEST_CASE("Parse namespace", "Doxygen") {
    const auto xml = parse(R"(<?xml version='1.0' encoding='UTF-8' standalone='no'?>
    <doxygen xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="compound.xsd" version="1.9.1" xml:lang="en-US">
    <compounddef id="namespaceEngine" kind="namespace" language="C++">
        <compoundname>Engine</compoundname>
        <innerclass refid="classEngine_1_1Exception" prot="public">Engine::Exception</innerclass>
        <innernamespace refid="namespaceEngine_1_1Assets">Engine::Assets</innernamespace>
        <innernamespace refid="namespaceEngine_1_1Audio">Engine::Audio</innernamespace>
        <innernamespace refid="namespaceEngine_1_1Graphics">Engine::Graphics</innernamespace>
        <innernamespace refid="namespaceEngine_1_1Gui">Engine::Gui</innernamespace>
        <innernamespace refid="namespaceEngine_1_1Utils">Engine::Utils</innernamespace>
        <sectiondef kind="func">
        <memberdef kind="function" id="namespaceEngine_1ae3918a7bd8220f317f339ebda3295fa7" prot="public" static="no" const="no" explicit="no" inline="no" virt="non-virtual">
            <type>std::string</type>
            <definition>std::string Engine::getVersion</definition>
            <argsstring>()</argsstring>
            <name>getVersion</name>
            <briefdescription>
            </briefdescription>
            <detaileddescription>
            </detaileddescription>
            <inbodydescription>
            </inbodydescription>
            <location file="src/Engine.hpp" line="46" column="20" declfile="src/Engine.hpp" declline="46" declcolumn="20"/>
        </memberdef>
        </sectiondef>
        <briefdescription>
    <para>This namespace contains all of the necessary engine components. </para>
        </briefdescription>
        <detaileddescription>
    <para><simplesect kind="see"><para><ref refid="namespaceEngine_1_1Assets" kindref="compound">Engine::Assets</ref> </para>
    </simplesect>
    <simplesect kind="see"><para><ref refid="namespaceEngine_1_1Audio" kindref="compound">Engine::Audio</ref> </para>
    </simplesect>
    <simplesect kind="see"><para><ref refid="namespaceEngine_1_1Graphics" kindref="compound">Engine::Graphics</ref> </para>
    </simplesect>
    <simplesect kind="see"><para><ref refid="namespaceEngine_1_1Utils" kindref="compound">Engine::Utils</ref> </para>
    </simplesect>
    </para>
        </detaileddescription>
        <location file="src/Assets/Asset.hpp" line="5" column="1"/>
    </compounddef>
    </doxygen>
    )");

    const auto root = xml.root();

    const NodeSharedPtr compound = Doxygen::parse(root);
    REQUIRE(!!compound);

    REQUIRE(compound->kind == Kind::NAMESPACE);

    const auto& inner = compound->childrenRefs;
    REQUIRE(inner.size() == 6);
    REQUIRE(inner.at(0).asBasicRef().refid == "classEngine_1_1Exception");
    REQUIRE(inner.at(1).asBasicRef().refid == "namespaceEngine_1_1Assets");
}
