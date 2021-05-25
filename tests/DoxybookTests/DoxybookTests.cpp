#include <Doxybook/Doxybook.hpp>
#include <Doxybook/Doxygen.hpp>
#include <Doxybook/Exception.hpp>
#include <Doxybook/Log.hpp>
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

TEST_CASE("Parse test xml files", "Doxybook") {
    Config config{};
    Log::setQuietMode(true);
    Doxybook doxy(config, std::filesystem::path{"xml"});
    Log::setQuietMode(false);

    SECTION("Inner groups must be resolved correctly") {
        auto groupEngine = find(doxy, "group___engine");
        auto groupUtils = find(doxy, "group___utils");
        auto groupAudio = find(doxy, "group___audio");

        REQUIRE(groupEngine->parent.expired());
        REQUIRE(isChildOf(groupUtils, groupEngine));
        REQUIRE(isChildOf(groupAudio, groupEngine));
    }

    SECTION("Prefer linking to entities in groups instead of entities in namespaces") {
        auto group = find(doxy, "group___utils");
        auto func = find(doxy, "group___utils_1gaef03bfdf7ea06e105903c76f06f22deb");

        // Parent should not change!
        REQUIRE(func->parent.lock()->refid == "namespace_engine_1_1_utils");

        // But URL should point to the group page
        REQUIRE(func->url == "Modules/group___utils.md#function-name_with_underscores");
    }

    SECTION("Classes must reference base and dervied classes") {
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

    SECTION("Unions defined globally vs as a child") {
        auto unionSome = find(doxy, "union_some_union");
        REQUIRE(unionSome->parent.expired());

        auto classAsset = find(doxy, "class_engine_1_1_assets_1_1_asset");
        auto unionEvent = find(doxy, "union_engine_1_1_assets_1_1_asset_1_1_event");
        REQUIRE(isChildOf(unionEvent, classAsset));
    }

    SECTION("Deprecated functions") {
        auto func = find(doxy, "class_engine_1_1_audio_1_1_audio_buffer_1a9e2d6ba7577091ff59e68f2d520e9174");
        REQUIRE(func->kind == Kind::FUNCTION);
        REQUIRE(func->properties.isDeprecated == true);
    }

    SECTION("Functions with arrays") {
        auto func = find(doxy, "class_engine_1_1_audio_1_1_audio_buffer_1a51ef075612af06edcb53d3c9b6c4fc7c");
        REQUIRE(func->kind == Kind::FUNCTION);
        REQUIRE(func->properties.params.size() == 1);
        REQUIRE(func->properties.params.front().name == "data[Size]");
    }

    SECTION("Typedef arguments") {
        auto typedf = find(doxy, "namespace_engine_1_1_audio_1a9527f056637d57771ee8025589fd596d");
        REQUIRE(typedf->kind == Kind::TYPEDEF);
        REQUIRE(typedf->properties.type.markdown == "int(*)(AudioBuffer &buffer, size_t offset)");
    }

    SECTION("Order of functions") {
        auto klass = find(doxy, "class_engine_1_1_audio_1_1_audio_buffer");

        REQUIRE(klass->kind == Kind::CLASS);

        std::vector<NodeSharedPtr> functions;
        std::copy_if(klass->children.begin(), klass->children.end(), std::back_inserter(functions),
                     [](NodeSharedPtr& node) { return node->kind == Kind::FUNCTION; });

        REQUIRE(functions.size() == 11);
        REQUIRE(functions.front()->refid ==
                "class_engine_1_1_audio_1_1_audio_buffer_1ab3f8002fc80d9bff50cfb6095e10a721");
        REQUIRE(functions.back()->refid ==
                "class_engine_1_1_audio_1_1_audio_buffer_1a93722a1dc8b7340e2c9b5e78d50151f5");
    }
}
