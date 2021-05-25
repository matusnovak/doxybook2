#include <Doxybook/Doxybook.hpp>
#include <Doxybook/Doxygen.hpp>
#include <Doxybook/Exception.hpp>
#include <catch2/catch.hpp>
#include <fmt/format.h>
#include <iostream>

using namespace Doxybook2;

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

static NodeSharedPtr find(const Doxybook& doxybook, const std::string& refid) {
    return find(doxybook.getIndex(), refid);
}

static bool isChildOf(const NodeSharedPtr& node, const NodeSharedPtr& parent) {
    if (node->parent.lock() != parent) {
        return false;
    }

    const auto it = std::find_if(parent->children.begin(), parent->children.end(),
                                 [&](const NodeSharedPtr& child) { return child == node; });

    return it != parent->children.end();
}

static Config config{};

static Doxybook& loadXml() {
    static std::unique_ptr<Doxybook> doxy;
    if (!doxy) {
        doxy.reset(new Doxybook(config, std::filesystem::path{"xml"}));
    }
    return *doxy;
}

TEST_CASE("Inner groups must be resolved correctly", "Doxygen") {
    auto& doxy = loadXml();

    auto groupEngine = find(doxy, "group___engine");
    auto groupUtils = find(doxy, "group___utils");
    auto groupAudio = find(doxy, "group___audio");

    REQUIRE(groupEngine->parent.expired());
    REQUIRE(isChildOf(groupUtils, groupEngine));
    REQUIRE(isChildOf(groupAudio, groupEngine));
}

TEST_CASE("Prefer linking to entities in groups instead of entities in namespaces", "Doxygen") {
    auto& doxy = loadXml();

    auto group = find(doxy, "group___utils");
    auto func = find(doxy, "group___utils_1gaef03bfdf7ea06e105903c76f06f22deb");

    // Parent should not change!
    REQUIRE(func->parent.lock()->refid == "namespace_engine_1_1_utils");

    // But URL should point to the group page
    REQUIRE(func->url == "Modules/group___utils.md#function-name_with_underscores");
}

TEST_CASE("Classes must reference base and dervied classes (#36)", "Doxygen") {
    auto& doxy = loadXml();

    auto handle = find(doxy, "class_engine_1_1_graphics_1_1_handle");
    auto texture = find(doxy, "class_engine_1_1_graphics_1_1_texture");
    auto texture2d = find(doxy, "class_engine_1_1_graphics_1_1_texture2_d");

    REQUIRE(texture2d->baseClasses.size() == 1);
    REQUIRE(texture2d->derivedClasses.size() == 0);
    REQUIRE(texture2d->baseClasses.front().asNode().lock() == texture);

    REQUIRE(texture->baseClasses.size() == 1);
    REQUIRE(texture->derivedClasses.size() == 3);
    REQUIRE(texture->baseClasses.front().asNode().lock() == handle);

    REQUIRE(handle->derivedClasses.size() == 1);
    REQUIRE(handle->derivedClasses.front().asNode().lock() == texture);
}

TEST_CASE("Unions defined globally vs as a child (#22)", "Doxygen") {
    auto& doxy = loadXml();

    auto unionSome = find(doxy, "union_some_union");
    REQUIRE(unionSome->parent.expired());

    auto classAsset = find(doxy, "class_engine_1_1_assets_1_1_asset");
    auto unionEvent = find(doxy, "union_engine_1_1_assets_1_1_asset_1_1_event");
    REQUIRE(isChildOf(unionEvent, classAsset));
}

TEST_CASE("Deprecated functions (#9)", "Doxygen") {
    auto& doxy = loadXml();

    auto func = find(doxy, "class_engine_1_1_audio_1_1_audio_buffer_1a9e2d6ba7577091ff59e68f2d520e9174");
    REQUIRE(func->properties.isDeprecated == true);
}

TEST_CASE("Functions with arrays (#6)", "Doxygen") {
    auto& doxy = loadXml();

    auto func = find(doxy, "class_engine_1_1_audio_1_1_audio_buffer_1a51ef075612af06edcb53d3c9b6c4fc7c");
    REQUIRE(func->properties.params.size() == 1);
    REQUIRE(func->properties.params.front().name == "data[Size]");
}

TEST_CASE("Typedef arguments (#8)", "Doxygen") {
    auto& doxy = loadXml();

    auto typedf = find(doxy, "namespace_engine_1_1_audio_1a9527f056637d57771ee8025589fd596d");
    REQUIRE(typedf->properties.type.markdown == "typedef int(*)(AudioBuffer &buffer, size_t offset)");
}

TEST_CASE("C# Properties throwing errors (#20)", "Doxygen") {
}

TEST_CASE("Order of functions (#16)", "Doxygen") {
}

TEST_CASE("Kind signal not recognized (#15)", "Doxygen") {
}

TEST_CASE("C++ operator bool class member breaks linking (#14)", "Doxygen") {
}
