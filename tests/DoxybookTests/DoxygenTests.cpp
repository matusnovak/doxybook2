#include <Doxybook/Doxybook.hpp>
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
    REQUIRE(compound->properties.initializer.empty());
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
    "",
    false,
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

    REQUIRE(Text::printMarkdown(param.type.node, defaultOptons) == "[AudioBuffer](some_link.md) &");
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

    const auto brief = Text::printMarkdown(member->brief.node, defaultOptons);
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

    REQUIRE(Text::printMarkdown(compound->title.node, defaultOptons) == "Graphical related classes");

    const auto& inner = compound->inners;
    REQUIRE(inner.size() == 6);
    REQUIRE(inner.front().asBasicRef().refid == "classEngine_1_1Graphics_1_1Framebuffer");
    REQUIRE(inner.back().asBasicRef().refid == "group__Graphics_1gae5b9a93609bed06adb3ce5902791df8a");

    const auto& children = compound->children;
    REQUIRE(children.size() == 0);
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

    const auto& inner = compound->inners;
    REQUIRE(inner.size() == 6);
    REQUIRE(inner.at(0).asBasicRef().refid == "classEngine_1_1Exception");
    REQUIRE(inner.at(1).asBasicRef().refid == "namespaceEngine_1_1Assets");
}

TEST_CASE("Parse enum", "Doxygen") {
    const auto xml = parse(R"(<?xml version='1.0' encoding='UTF-8' standalone='no'?>
    <memberdef kind="enum" id="class_engine_1_1_audio_1_1_audio_buffer_1ad6d10d04bef7fa259cdd5029697cf052" prot="public" static="no" strong="yes">
        <type></type>
        <name>Type</name>
        <enumvalue id="class_engine_1_1_audio_1_1_audio_buffer_1ad6d10d04bef7fa259cdd5029697cf052a696b031073e74bf2cb98e5ef201d4aa3" prot="public">
          <name>UNKNOWN</name>
          <initializer>= 0</initializer>
          <briefdescription>
          </briefdescription>
          <detaileddescription>
            <para>Dont use this </para>
          </detaileddescription>
        </enumvalue>
        <enumvalue id="class_engine_1_1_audio_1_1_audio_buffer_1ad6d10d04bef7fa259cdd5029697cf052a2dea92647909a0657c5ca657d92ff0c2" prot="public">
          <name>INT_8</name>
          <initializer>= 1 &lt;&lt; 1</initializer>
          <briefdescription>
          </briefdescription>
          <detaileddescription>
            <para>8-bit signed integer </para>
          </detaileddescription>
        </enumvalue>
        <enumvalue id="class_engine_1_1_audio_1_1_audio_buffer_1ad6d10d04bef7fa259cdd5029697cf052a6c4baecc05d360b290e07c50131c7fc2" prot="public">
          <name>INT_16</name>
          <initializer>= 1 &lt;&lt; 2</initializer>
          <briefdescription>
          </briefdescription>
          <detaileddescription>
            <para>16-bit signed integer </para>
          </detaileddescription>
        </enumvalue>
        <enumvalue id="class_engine_1_1_audio_1_1_audio_buffer_1ad6d10d04bef7fa259cdd5029697cf052a1b20706298a204bb53fae40ad0c97bbf" prot="public">
          <name>INT_24</name>
          <initializer>= 1 &lt;&lt; 3</initializer>
          <briefdescription>
          </briefdescription>
          <detaileddescription>
            <para>24-bit signed integer </para>
          </detaileddescription>
        </enumvalue>
        <enumvalue id="class_engine_1_1_audio_1_1_audio_buffer_1ad6d10d04bef7fa259cdd5029697cf052aab97c2f5d9a173a78b47802dce7a1806" prot="public">
          <name>INT_32</name>
          <initializer>= 1 &lt;&lt; 4</initializer>
          <briefdescription>
          </briefdescription>
          <detaileddescription>
            <para>32-bit signed integer </para>
          </detaileddescription>
        </enumvalue>
        <enumvalue id="class_engine_1_1_audio_1_1_audio_buffer_1ad6d10d04bef7fa259cdd5029697cf052a34429544f281bc1e40ec5525cff2a060" prot="public">
          <name>FLOAT_32</name>
          <initializer>= 1 &lt;&lt; 5</initializer>
          <briefdescription>
          </briefdescription>
          <detaileddescription>
            <para>32-bit float </para>
          </detaileddescription>
        </enumvalue>
        <briefdescription>
          <para>Different type of audio formats. </para>
        </briefdescription>
        <detaileddescription>
        </detaileddescription>
        <inbodydescription>
        </inbodydescription>
        <location file="src/Audio/AudioBuffer.hpp" line="65" column="13" bodyfile="src/Audio/AudioBuffer.hpp" bodystart="65" bodyend="90"/>
      </memberdef>
    )");

    const auto root = xml.root();

    const NodeSharedPtr compound = Doxygen::parseMember(root);
    REQUIRE(!!compound);

    REQUIRE(compound->kind == Kind::ENUM);

    const auto& inner = compound->inners;
    REQUIRE(inner.empty());

    REQUIRE(compound->children.size() == 6);
    REQUIRE(compound->refid == "class_engine_1_1_audio_1_1_audio_buffer_1ad6d10d04bef7fa259cdd5029697cf052");

    const auto& value = compound->children.front();
    REQUIRE(value->parent.lock() == compound);
    REQUIRE(value->kind == Kind::ENUMVALUE);
    REQUIRE(value->name == "UNKNOWN");
    REQUIRE(
        value->refid ==
        "class_engine_1_1_audio_1_1_audio_buffer_1ad6d10d04bef7fa259cdd5029697cf052a696b031073e74bf2cb98e5ef201d4aa3");
    REQUIRE(value->properties.visibility == Visibility::PUBLIC);
    REQUIRE(Text::printMarkdown(value->properties.initializer.node) == "= 0");
}

TEST_CASE("Parse signals", "Doxygen") {
    const auto xml = parse(R"(
        <memberdef kind="signal" id="classPressableLabel_1a70e5724a77a27c81cd8e324b427dfe90" prot="public" static="no" const="no" explicit="no" inline="no" virt="non-virtual">
            <type>void</type>
            <definition>void PressableLabel::pressed</definition>
            <argsstring>()</argsstring>
            <name>pressed</name>
            <briefdescription>
            </briefdescription>
            <detaileddescription> <para>image has been pressed on with mouse </para> </detaileddescription>
            <inbodydescription>
            </inbodydescription>
            <location file="pressablelabel.h" line="17" column="1"/>
        </memberdef>
    )");

    const auto root = xml.root();

    const NodeSharedPtr compound = Doxygen::parseMember(root);
    REQUIRE(!!compound);

    REQUIRE(compound->kind == Kind::SIGNAL);
    REQUIRE(compound->refid == "classPressableLabel_1a70e5724a77a27c81cd8e324b427dfe90");

    REQUIRE(compound->properties.argsString == "()");
    REQUIRE(compound->properties.definition == "void PressableLabel::pressed");
}

TEST_CASE("Parse slots", "Doxygen") {
    const auto xml = parse(R"(
        <memberdef kind="slot" id="classMainWindow_1a20e17289a70704d7f2afefb65c9d889c" prot="public" static="no" const="no" explicit="no" inline="no" virt="non-virtual">
            <type>void</type>
            <definition>void MainWindow::toggleFullscreen</definition>
            <argsstring>()</argsstring>
            <name>toggleFullscreen</name>
            <briefdescription>
            </briefdescription>
            <detaileddescription> <para>toggle window fullscreen flag </para>        </detaileddescription>
            <inbodydescription>
            </inbodydescription>
            <location file="mainwindow.h" line="56" column="1" bodyfile="mainwindow.cpp" bodystart="119" bodyend="122"/>
        </memberdef>
    )");

    const auto root = xml.root();

    const NodeSharedPtr compound = Doxygen::parseMember(root);
    REQUIRE(!!compound);

    REQUIRE(compound->kind == Kind::SLOT);
    REQUIRE(compound->refid == "classMainWindow_1a20e17289a70704d7f2afefb65c9d889c");

    REQUIRE(compound->properties.argsString == "()");
    REQUIRE(compound->properties.definition == "void MainWindow::toggleFullscreen");
}

TEST_CASE("C# Properties", "Doxygen") {
    const auto xml = parse(R"(
        <memberdef kind="property" id="some_random_property" prot="public" static="no" const="no" explicit="no" inline="no" virt="non-virtual">
            <type>int</type>
            <definition>int MainWindow::myProperty</definition>
            <argsstring>()</argsstring>
            <name>myProperty</name>
            <briefdescription>
            </briefdescription>
            <detaileddescription>
            </detaileddescription>
            <inbodydescription>
            </inbodydescription>
            <location file="mainwindow.h" line="1" column="1" bodyfile="mainwindow.cs" bodystart="1" bodyend="1"/>
        </memberdef>
    )");

    const auto root = xml.root();

    const NodeSharedPtr compound = Doxygen::parseMember(root);
    REQUIRE(!!compound);

    REQUIRE(compound->kind == Kind::PROPERTY);
    REQUIRE(compound->refid == "some_random_property");
}

TEST_CASE("C# Events", "Doxygen") {
    const auto xml = parse(R"(
        <memberdef kind="event" id="some_random_event" prot="public" static="no" const="no" explicit="no" inline="no" virt="non-virtual">
            <type>void</type>
            <definition>void MainWindow::myEvent</definition>
            <argsstring>()</argsstring>
            <name>myEvent</name>
            <briefdescription>
            </briefdescription>
            <detaileddescription>
            </detaileddescription>
            <inbodydescription>
            </inbodydescription>
            <location file="mainwindow.h" line="1" column="1" bodyfile="mainwindow.cs" bodystart="1" bodyend="1"/>
        </memberdef>
    )");

    const auto root = xml.root();

    const NodeSharedPtr compound = Doxygen::parseMember(root);
    REQUIRE(!!compound);

    REQUIRE(compound->kind == Kind::EVENT);
    REQUIRE(compound->refid == "some_random_event");
}

TEST_CASE("C++ operator bool class member should not break linking", "Doxygen") {
    const auto xml = parse(R"(
        <memberdef kind="function" id="class_box_1ad051f61bcb0be65272632ee39578e3a3" prot="public" static="no" const="yes" explicit="no" inline="no" virt="non-virtual">
            <type></type>
            <definition>Box::operator bool</definition>
            <argsstring>() const</argsstring>
            <name>operator bool</name>
            <briefdescription>
            </briefdescription>
            <detaileddescription>
            </detaileddescription>
            <inbodydescription>
            </inbodydescription>
            <location file="Core/Types/Box.hpp" line="52" column="9"/>
        </memberdef>
    )");

    const auto root = xml.root();

    const NodeSharedPtr compound = Doxygen::parseMember(root);
    REQUIRE(!!compound);

    const auto klass = std::make_shared<Node>();
    klass->kind = Kind::CLASS;
    klass->name = "SomeClass";
    klass->refid = "class__someclass";
    klass->children.push_back(compound);
    compound->parent = klass;

    Doxygen::finalize(Config{}, Cache{}, klass);

    REQUIRE(compound->kind == Kind::FUNCTION);
    REQUIRE(compound->refid == "class_box_1ad051f61bcb0be65272632ee39578e3a3");

    REQUIRE(compound->url == "Classes/class__someclass.md#function-operator-bool");
}

static NodeSharedPtr createFunction(const NodeSharedPtr& parent, const std::string& hash, const std::string& name) {
    auto node = std::make_shared<Node>();
    node->refid = parent->refid + "_" + hash;
    node->kind = Kind::FUNCTION;
    node->name = name;
    node->title = name;
    node->parent = parent;

    Param param;
    param.name = "foo";
    param.type = Text::Plain{"Foo"};

    node->properties.params.push_back(param);

    return node;
}

static NodeSharedPtr createClass(const std::string& name) {
    auto node = std::make_shared<Node>();
    node->refid = "class__" + name;
    node->kind = Kind::CLASS;
    node->name = name;
    node->title = name;

    return node;
}

static NodeSharedPtr createGroup(const std::string& name) {
    auto node = std::make_shared<Node>();
    node->refid = "group__" + name;
    node->kind = Kind::MODULE;
    node->name = name;
    node->title = name;

    return node;
}

static NodeSharedPtr createFile(const std::string& name) {
    auto node = std::make_shared<Node>();
    node->refid = "file__" + name;
    node->kind = Kind::FILE;
    node->name = name;
    node->title = name;

    return node;
}

static NodeSharedPtr createIndex() {
    auto node = std::make_shared<Node>();
    node->kind = Kind::INDEX;
    node->refid = "index";

    return node;
}

static NodeSharedPtr createSampleStructure() {
    auto index = createIndex();

    auto file = createFile("source");
    index->children.push_back(file);

    auto groupA = createGroup("A");
    auto groupB = createGroup("B");
    auto groupC = createGroup("C");

    index->children.push_back(groupA);
    index->children.push_back(groupB);
    index->children.push_back(groupC);

    groupA->inners.push_back(NodeRef{BasicRef{"group__B", "B"}});
    groupB->inners.push_back(NodeRef{BasicRef{"group__C", "C"}});

    auto func = createFunction(file, "1ae52df0e81924b99e45d515f595659628", "some_global_func");
    file->children.push_back(func);
    groupC->children.push_back(func);

    return index;
}

static NodeSharedPtr find(const NodeSharedPtr& node, const std::string& refid) {
    for (const auto& child : node->children) {
        if (child->refid == refid) {
            return child;
        }
        auto found = find(child, refid);
        if (found) {
            return found;
        }
    }

    if (node->kind == Kind::INDEX) {
        FAIL("Unable to find refid: " + refid);
    }

    return nullptr;
}

TEST_CASE("Restructure and assign parents", "Doxygen") {
    auto index = createSampleStructure();

    REQUIRE(index->children.size() == 4);
    REQUIRE(find(index, "group__A")->children.size() == 0);
    REQUIRE(find(index, "group__B")->children.size() == 0);
    REQUIRE(find(index, "group__C")->children.size() == 1);

    const auto cache = Doxygen::buildCache(index);
    Doxygen::resolveReferences(cache, index);
    Doxygen::resolveHierarchy(index);

    REQUIRE(index->children.size() == 2);
    REQUIRE(find(index, "file__source")->children.size() == 1);
    REQUIRE(find(index, "group__A")->children.size() == 1);
    REQUIRE(find(index, "group__B")->children.size() == 1);
    REQUIRE(find(index, "group__C")->children.size() == 1);
}
