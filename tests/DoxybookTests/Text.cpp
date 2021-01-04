#include <Doxybook/Doxygen.hpp>
#include <Doxybook/Exception.hpp>
#include <Doxybook/Text.hpp>
#include <catch2/catch.hpp>
#include <fmt/format.h>
#include <iostream>

using namespace Doxybook2;

static Xml parse(const char* src) {
    auto s = std::string(src);
    Xml xml(s);
    return xml;
}

TEST_CASE("Sanity Check", "Text") {
    // clang-format off
    Text::NodeCompound a{ Text::Type::Paragraph, {}, {
            Text::Plain{"Hello "},
            Text::NodeCompound{Text::Type::UrlLink, {
                {"url", "https://github.com"}
            }, {
                Text::Plain{"World!"}
            }}
        }
    };
    // clang-format on

    REQUIRE(a == a);
}

TEST_CASE("Simple plain string", "Text") {
    const auto xml = parse(R"(<para>using AudioData = Utils::ArrayView&lt;T&gt;</para>)");
    const auto elm = xml.root();

    const auto parsed = Text::parse(elm);

    // clang-format off
    const Text::NodeVariant expected = Text::NodeCompound{ Text::Type::Paragraph, {}, {
            Text::Plain{"using AudioData = Utils::ArrayView<T>"}
        }
    };
    // clang-format on

    REQUIRE(parsed == expected);

    const auto md = Text::printMarkdown(parsed);

    REQUIRE(md == "using AudioData = Utils::ArrayView<T>\n");
}

TEST_CASE("Url link", "Text") {
    const auto xml =
        parse(R"(<para><ulink url="https://github.com/matusnovak/doxybook2">GitHub repository</ulink></para>)");
    const auto elm = xml.root();

    const auto parsed = Text::parse(elm);

    // clang-format off
    const Text::NodeVariant expected = Text::NodeCompound{ Text::Type::Paragraph, {}, {
            Text::NodeCompound{Text::Type::UrlLink, {
                    {"url", "https://github.com/matusnovak/doxybook2"}
                }, {
                Text::Plain{"GitHub repository"}
            }}
        }
    };
    // clang-format on

    REQUIRE(parsed == expected);

    const auto md = Text::printMarkdown(parsed);
    std::cout << md << std::endl;
}

TEST_CASE("Url link with prefix", "Text") {
    const auto xml = parse(
        R"(<para>Link to: <ulink url="https://github.com/matusnovak/doxybook2">GitHub repository</ulink></para>)");
    const auto elm = xml.root();

    const auto parsed = Text::parse(elm);

    // clang-format off
    const Text::NodeVariant expected = Text::NodeCompound{ Text::Type::Paragraph, {},  {
            Text::Plain{"Link to: "},
            Text::NodeCompound{Text::Type::UrlLink, {
                    {"url", "https://github.com/matusnovak/doxybook2"}
                }, {
                Text::Plain{"GitHub repository"}
            }}
        }
    };
    // clang-format on

    REQUIRE(parsed == expected);
}

TEST_CASE("Url link with suffix", "Text") {
    const auto xml =
        parse(R"(<para><ulink url="https://github.com/matusnovak/doxybook2">GitHub</ulink> repository</para>)");
    const auto elm = xml.root();

    const auto parsed = Text::parse(elm);

    // clang-format off
    const Text::NodeVariant expected = Text::NodeCompound{ Text::Type::Paragraph, {},  {
            Text::NodeCompound{Text::Type::UrlLink, {
                    {"url", "https://github.com/matusnovak/doxybook2"}
                }, {
                Text::Plain{"GitHub"}
            }},
            Text::Plain{" repository"}
        }
    };
    // clang-format on

    REQUIRE(parsed == expected);
}

TEST_CASE("Url link nested", "Text") {
    const auto xml = parse(
        R"(<para><ulink url="https://github.com/matusnovak/doxybook2"><emphasis><bold>GitHub</bold> repository</emphasis></ulink></para>)");
    const auto elm = xml.root();

    const auto parsed = Text::parse(elm);

    // clang-format off
    const Text::NodeVariant expected = Text::NodeCompound{ Text::Type::Paragraph, {}, {
            Text::NodeCompound{Text::Type::UrlLink, {
                    {"url", "https://github.com/matusnovak/doxybook2"}
                }, {
                Text::NodeCompound{Text::Type::Italics, {}, {
                    Text::NodeCompound{Text::Type::Bold, {}, {
                        Text::Plain{"GitHub"}
                    }},
                    Text::Plain{" repository"}
                }},
            }},
        }
    };
    // clang-format on

    REQUIRE(parsed == expected);
}

TEST_CASE("Horizontal ruler", "Text") {
    const auto xml = parse(R"(<para><bold>Hello</bold><hruler/><bold>World!</bold></para>)");
    const auto elm = xml.root();

    const auto parsed = Text::parse(elm);

    // clang-format off
    const Text::NodeVariant expected = Text::NodeCompound{ Text::Type::Paragraph, {}, {
            Text::NodeCompound{Text::Type::Bold, {}, {
                Text::Plain{"Hello"}
            }},
            Text::Node{Text::Type::HRuler},
            Text::NodeCompound{Text::Type::Bold, {}, {
                Text::Plain{"World!"}
            }},
        }
    };
    // clang-format on

    REQUIRE(parsed == expected);
}

TEST_CASE("Ref", "Text") {
    const auto xml = parse(
        R"(<para><ref refid="class_engine_1_1_graphics_1_1_texture" kindref="compound">Some texture class ref</ref></para>)");
    const auto elm = xml.root();

    const auto parsed = Text::parse(elm);

    // clang-format off
    const Text::NodeVariant expected = Text::NodeCompound{ Text::Type::Paragraph, {}, {
            Text::NodeCompound{Text::Type::RefLink, {
                    {"refid", "class_engine_1_1_graphics_1_1_texture"}
                }, {
                Text::Plain{"Some texture class ref"}
            }}
        }
    };
    // clang-format on

    REQUIRE(parsed == expected);
}

TEST_CASE("Image", "Text") {
    const auto xml = parse(R"(<para><image type="html" name="doxygen.png">Title</image> some image</para>)");
    const auto elm = xml.root();

    const auto parsed = Text::parse(elm);

    // clang-format off
    const Text::NodeVariant expected = Text::NodeCompound{ Text::Type::Paragraph, {}, {
            Text::NodeCompound{Text::Type::ImageLink, {
                    {"url", "doxygen.png"}
                }, {
                Text::Plain{"Title"}
            }},
            Text::Plain{" some image"}
        }
    };
    // clang-format on

    REQUIRE(parsed == expected);
}

TEST_CASE("Section", "Text") {
    const auto xml = parse(R"(<sect2 id="index_1autotoc_md0">
<title>Images:</title>
<para><image type="html" name="doxygen.png"></image>
</para>
</sect2>)");
    const auto elm = xml.root();

    const auto parsed = Text::parse(elm);

    // clang-format off
    const Text::NodeVariant expected = Text::NodeCompound{Text::Type::Section, {
            {"id", "index_1autotoc_md0"},
            {"level", "sect2"},
            {"title", "Images:"}
        }, {
            Text::NodeCompound{ Text::Type::Paragraph, {}, {
                Text::NodeCompound{Text::Type::ImageLink, {
                        {"url", "doxygen.png"}
                    }, {}
                }
            }}
        }
    };
    // clang-format on

    REQUIRE(parsed == expected);
}

TEST_CASE("Section nested", "Text") {
    const auto xml = parse(R"(<sect2 id="index_1autotoc_md0">
<title><bold>Images</bold>:</title>
<para><image type="html" name="doxygen.png"></image>
</para>
</sect2>)");
    const auto elm = xml.root();

    const auto parsed = Text::parse(elm);

    // clang-format off
    const Text::NodeVariant expected = Text::NodeCompound{ Text::Type::Section, {
            {"id", "index_1autotoc_md0"},
            {"level", "sect2"},
            {"title", Text::NodeCompound{Text::Type::Text, {}, {
                Text::NodeCompound{Text::Type::Bold, {}, {
                    Text::Plain{"Images"}
                }},
                Text::Plain{":"}
            }}}
        }, {
            Text::NodeCompound{ Text::Type::Paragraph, {}, {
                Text::NodeCompound{Text::Type::ImageLink, {
                        {"url", "doxygen.png"}
                    }, {}
                }
            }}
        }
    };
    // clang-format on

    REQUIRE(parsed == expected);
}

TEST_CASE("Itemized list", "Text") {
    const auto xml = parse(R"(<para>
    <itemizedlist>
        <listitem>
            <para>list item 1<itemizedlist>
                <listitem>
                    <para>sub item 1<itemizedlist>
                        <listitem>
                            <para>sub sub item 1 with <emphasis>italic</emphasis>
                            </para>
                        </listitem>
                        <listitem>
                            <para>sub sub item 2 with <bold>bold</bold>
                            </para>
                        </listitem>
                    </itemizedlist>
                </para>
            </listitem>
            <listitem>
                <para>sub item 2</para>
            </listitem>
        </itemizedlist>
    </para>
</listitem>
<listitem>
    <para>list item 2<itemizedlist>
        <listitem>
            <para>sub item 3<itemizedlist>
                <listitem>
                    <para>sub sub item 3 with <strike>strike through</strike>
                    </para>
                </listitem>
                <listitem>
                    <para>sub sub item 4 with <computeroutput>monospaced</computeroutput>
                    </para>
                </listitem>
            </itemizedlist>
        </para>
    </listitem>
</itemizedlist>
</para>
</listitem>
</itemizedlist>
</para>)");
    const auto elm = xml.root();

    const auto parsed = Text::parse(elm);

    // clang-format off
    const Text::NodeVariant expected = Text::NodeCompound{ Text::Type::Paragraph, {}, {
            Text::NodeCompound{ Text::Type::ItemizedList, {}, {
                Text::NodeCompound{Text::Type::ListItem, {}, {
                    Text::Plain{"list item 1"},
                    Text::NodeCompound{ Text::Type::ItemizedList, {}, {
                        Text::NodeCompound{Text::Type::ListItem, {}, {
                            Text::Plain{"sub item 1"},
                            Text::NodeCompound{ Text::Type::ItemizedList, {}, {
                                Text::NodeCompound{Text::Type::ListItem, {}, {
                                    Text::Plain{"sub sub item 1 with "},
                                    Text::NodeCompound{ Text::Type::Italics, {}, {
                                        Text::Plain{"italic"}
                                    }}
                                }},
                                Text::NodeCompound{Text::Type::ListItem, {}, {
                                    Text::Plain{"sub sub item 2 with "},
                                    Text::NodeCompound{ Text::Type::Bold, {}, {
                                        Text::Plain{"bold"}
                                    }}
                                }}
                            }}
                        }},
                        Text::NodeCompound{Text::Type::ListItem, {}, {
                            Text::Plain{"sub item 2"},
                        }}
                    }}
                }},
                Text::NodeCompound{Text::Type::ListItem, {}, {
                    Text::Plain{"list item 2"},
                    Text::NodeCompound{ Text::Type::ItemizedList, {}, {
                        Text::NodeCompound{Text::Type::ListItem, {}, {
                            Text::Plain{"sub item 3"},
                            Text::NodeCompound{ Text::Type::ItemizedList, {}, {
                                Text::NodeCompound{Text::Type::ListItem, {}, {
                                    Text::Plain{"sub sub item 3 with "},
                                    Text::NodeCompound{ Text::Type::StrikeThrough, {}, {
                                        Text::Plain{"strike through"}
                                    }}
                                }},
                                Text::NodeCompound{Text::Type::ListItem, {}, {
                                    Text::Plain{"sub sub item 4 with "},
                                    Text::NodeCompound{ Text::Type::Monospaced, {}, {
                                        Text::Plain{"monospaced"}
                                    }}
                                }}
                            }}
                        }}
                    }}
                }}
            }}
        }
    };
    // clang-format on

    REQUIRE(parsed == expected);
}

TEST_CASE("Code listing", "Text") {
    const auto xml = parse(R"(<sect2 id="index_1autotoc_md7">
<title>Snippet:</title>
<para><programlisting filename="example-1.cpp"><codeline><highlight class="preprocessor">#include<sp/>&quot;<ref refid="_engine_8hpp" kindref="compound">Engine.hpp</ref>&quot;</highlight><highlight class="normal"></highlight></codeline>
<codeline><highlight class="normal"></highlight></codeline>
<codeline><highlight class="normal"></highlight><highlight class="comment">//<sp/>A<sp/>dummy<sp/>example<sp/></highlight><highlight class="normal"></highlight></codeline>
<codeline><highlight class="normal"></highlight><highlight class="keywordtype">int</highlight><highlight class="normal"><sp/>main()<sp/>{</highlight></codeline>
<codeline><highlight class="normal"><sp/><sp/><sp/><sp/></highlight><highlight class="comment">//<sp/>Create<sp/>pixels<sp/>buffer</highlight><highlight class="normal"></highlight></codeline>
<codeline><highlight class="normal"><sp/><sp/><sp/><sp/></highlight><highlight class="keyword">const</highlight><highlight class="normal"><sp/></highlight><highlight class="keyword">auto</highlight><highlight class="normal"><sp/>pixels<sp/>=<sp/>std::make_unique&lt;uint8_t[]&gt;(</highlight><highlight class="keyword">new</highlight><highlight class="normal"><sp/>uint8_t[1024*1024*3]);</highlight></codeline>
<codeline><highlight class="normal"><sp/><sp/><sp/><sp/>fillData(*pixels,<sp/></highlight><highlight class="stringliteral">&quot;path/to/texture.png&quot;</highlight><highlight class="normal">);</highlight></codeline>
<codeline><highlight class="normal"></highlight></codeline>
<codeline><highlight class="normal"><sp/><sp/><sp/><sp/></highlight><highlight class="comment">//<sp/>Create<sp/>a<sp/>texture<sp/>out<sp/>of<sp/>the<sp/>pixels</highlight><highlight class="normal"></highlight></codeline>
<codeline><highlight class="normal"><sp/><sp/><sp/><sp/><ref refid="class_engine_1_1_graphics_1_1_texture2_d" kindref="compound">Engine::Graphics::Texture2D</ref><sp/>texture(1024,<sp/>1024,<sp/>*data);</highlight></codeline>
<codeline><highlight class="normal"></highlight></codeline>
<codeline><highlight class="normal"><sp/><sp/><sp/><sp/></highlight><highlight class="comment">//<sp/>Done</highlight><highlight class="normal"></highlight></codeline>
<codeline><highlight class="normal"><sp/><sp/><sp/><sp/></highlight><highlight class="keywordflow">return</highlight><highlight class="normal"><sp/>0;</highlight></codeline>
<codeline><highlight class="normal">}</highlight></codeline>
</programlisting> </para>
</sect2>)");

    const auto code = R"(#include "Engine.hpp"

// A dummy example 
int main() {
    // Create pixels buffer
    const auto pixels = std::make_unique<uint8_t[]>(new uint8_t[1024*1024*3]);
    fillData(*pixels, "path/to/texture.png");

    // Create a texture out of the pixels
    Engine::Graphics::Texture2D texture(1024, 1024, *data);

    // Done
    return 0;
})";

    const auto elm = xml.root();

    const auto parsed = Text::parse(elm);

    // clang-format off
    const Text::NodeVariant expected = Text::NodeCompound{ Text::Type::Section, {
        {"id", "index_1autotoc_md7"},
        {"level", "sect2"},
        {"title", "Snippet:"}
    }, {
        Text::NodeCompound{ Text::Type::Paragraph, {}, {
            Text::NodeCompound{ Text::Type::CodeBlock, {
                {"lang", "cpp"},
                {"filename", "example-1.cpp"}
            }, {
                Text::Plain{code}
            }}
        }}
    }};
    // clang-format on

    REQUIRE(parsed == expected);
}

TEST_CASE("Table", "Text") {
    const auto xml = parse(R"(<sect2 id="index_1autotoc_md8">
<title>Table:</title>
<para><table rows="3" cols="3"><row>
<entry thead="yes"><para>First Header </para>
</entry><entry thead="yes"><para>Second Header </para>
</entry><entry thead="yes"><para>Third Header </para>
</entry></row>
<row>
<entry thead="no"><para>Content Cell </para>
</entry><entry thead="no"><para>Content Cell </para>
</entry><entry thead="no"><para>Content Cell </para>
</entry></row>
<row>
<entry thead="no"><para>Content Cell </para>
</entry><entry thead="no"><para>Content Cell </para>
</entry><entry thead="no"><para><bold>Content</bold> Cell </para>
</entry></row>
</table>
</para>
</sect2>)");

    const auto elm = xml.root();

    const auto parsed = Text::parse(elm);

    // clang-format off
    const Text::NodeVariant expected = Text::NodeCompound{ Text::Type::Section, {
        {"id", "index_1autotoc_md8"},
        {"level", "sect2"},
        {"title", "Table:"}
    }, {
        Text::NodeCompound{ Text::Type::Paragraph, {}, {
            Text::NodeCompound{ Text::Type::Table, {}, {
                Text::NodeCompound{ Text::Type::TableRow, {}, {
                    Text::NodeCompound{ Text::Type::TableCell, {}, {
                        Text::Plain{"First Header "}
                    }},
                    Text::NodeCompound{ Text::Type::TableCell, {}, {
                        Text::Plain{"Second Header "}
                    }},
                    Text::NodeCompound{ Text::Type::TableCell, {}, {
                        Text::Plain{"Third Header "}
                    }}
                }},
                Text::NodeCompound{ Text::Type::TableRow, {}, {
                    Text::NodeCompound{ Text::Type::TableCell, {}, {
                        Text::Plain{"Content Cell "}
                    }},
                    Text::NodeCompound{ Text::Type::TableCell, {}, {
                        Text::Plain{"Content Cell "}
                    }},
                    Text::NodeCompound{ Text::Type::TableCell, {}, {
                        Text::Plain{"Content Cell "}
                    }}
                }},
                Text::NodeCompound{ Text::Type::TableRow, {}, {
                    Text::NodeCompound{ Text::Type::TableCell, {}, {
                        Text::Plain{"Content Cell "}
                    }},
                    Text::NodeCompound{ Text::Type::TableCell, {}, {
                        Text::Plain{"Content Cell "}
                    }},
                    Text::NodeCompound{ Text::Type::TableCell, {}, {
                        Text::NodeCompound{ Text::Type::Bold, {}, {
                            Text::Plain{"Content"}
                        }},
                        Text::Plain{" Cell "}
                    }}
                }}
            }}
        }}
    } };
    // clang-format on

    REQUIRE(parsed == expected);
}
