#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "Doxygen.hpp"
#include "Exception.hpp"
#include "JsonConverter.hpp"

using namespace Doxydown;

static const auto klassEngineGraphicsTexture =
    R"({
  "abstract": true,
  "baseClasses": [
    {
      "brief": "",
      "kind": "class",
      "name": "Engine::Graphics::Handle",
      "prot": "protected",
      "refid": "class_engine_1_1_graphics_1_1_handle",
      "virt": "non-virtual"
    }
  ],
  "brief": "This is a base texture class that serves as a base class for other texture types. ",
  "derivedClasses": [
    {
      "brief": "",
      "kind": "class",
      "name": "Engine::Graphics::Framebuffer",
      "prot": "public",
      "refid": "class_engine_1_1_graphics_1_1_framebuffer",
      "virt": "non-virtual"
    },
    {
      "brief": "",
      "kind": "class",
      "name": "Engine::Graphics::Texture2D",
      "prot": "public",
      "refid": "class_engine_1_1_graphics_1_1_texture2_d",
      "virt": "non-virtual"
    },
    {
      "brief": "",
      "kind": "class",
      "name": "Engine::Graphics::Texture3D",
      "prot": "public",
      "refid": "class_engine_1_1_graphics_1_1_texture3_d",
      "virt": "non-virtual"
    }
  ],
  "kind": "interface",
  "location": {
    "bodyEnd": 34,
    "bodyFile": "include/Graphics/Texture.hpp",
    "bodyStart": 13,
    "column": 1,
    "file": "include/Graphics/Texture.hpp",
    "line": 13
  },
  "name": "Engine::Graphics::Texture",
  "prot": "public",
  "refid": "class_engine_1_1_graphics_1_1_texture",
  "static": false
})"_json;

static const std::unordered_map<std::string, std::string> parentChildRelationMap = {
    {"namespace_engine", "index"},
    {"namespace_engine_1ae3918a7bd8220f317f339ebda3295fa7", "namespace_engine"},
    {"class_engine_1_1_exception", "namespace_engine"},
    {"class_engine_1_1_exception_1ace265added7ec74ac0fe09b30d442621", "class_engine_1_1_exception"},
    {"class_engine_1_1_exception_1a697748d2c21a434b4742910b8d4e53d4", "class_engine_1_1_exception"},
    {"class_engine_1_1_exception_1a4f566b63e5d447cb468d0f89b46bb75c", "class_engine_1_1_exception"},
    {"class_engine_1_1_exception_1ac25e87274c89524b026998678500dd29", "class_engine_1_1_exception"},
    {"namespace_engine_1_1_assets", "namespace_engine"},
    {"class_engine_1_1_assets_1_1_asset", "namespace_engine_1_1_assets"},
    {"class_engine_1_1_assets_1_1_asset_1af26cbe7f9237ea471f12620b0547ba39", "class_engine_1_1_assets_1_1_asset"},
    {"class_engine_1_1_assets_1_1_asset_1acb5958d164f0b8ced1638de58b3ea0b0", "class_engine_1_1_assets_1_1_asset"},
    {"class_engine_1_1_assets_1_1_asset_1aecd651b9312fa89e40d268214576839e", "class_engine_1_1_assets_1_1_asset"},
    {"class_engine_1_1_assets_1_1_asset_1a10931e97cd354bf1770b3824d0e87029", "class_engine_1_1_assets_1_1_asset"},
    {"class_engine_1_1_assets_1_1_asset_1ab25cbfe379d57b414b800c0040e7c63c", "class_engine_1_1_assets_1_1_asset"},
    {"class_engine_1_1_assets_1_1_asset_1a0f49f750cc898cd041d24bad216236a3", "class_engine_1_1_assets_1_1_asset"},
    {"union_engine_1_1_assets_1_1_asset_1_1_event", "class_engine_1_1_assets_1_1_asset"},
    {"union_engine_1_1_assets_1_1_asset_1_1_event_1a44fbeef7562e035debdc9e7b67a4a4d3", "union_engine_1_1_assets_1_1_asset_1_1_event"},
    {"union_engine_1_1_assets_1_1_asset_1_1_event_1a8f9134d4fb444dc5663b84f843f33af9", "union_engine_1_1_assets_1_1_asset_1_1_event"},
    {"union_engine_1_1_assets_1_1_asset_1_1_event_1ae2d2305926d305273bc1d390b15766f4", "union_engine_1_1_assets_1_1_asset_1_1_event"},
    {"struct_engine_1_1_assets_1_1_asset_1_1_event_1_1_loaded", "union_engine_1_1_assets_1_1_asset_1_1_event"},
    {"struct_engine_1_1_assets_1_1_asset_1_1_event_1_1_loaded_1a6df474ccd46caeb0e7ffb2214bb795d3", "struct_engine_1_1_assets_1_1_asset_1_1_event_1_1_loaded"},
    {"struct_engine_1_1_assets_1_1_asset_1_1_event_1_1_loaded_1a4560959655f932a372e0f9cee598ed1c", "struct_engine_1_1_assets_1_1_asset_1_1_event_1_1_loaded"},
    {"struct_engine_1_1_assets_1_1_asset_1_1_event_1_1_unloaded", "union_engine_1_1_assets_1_1_asset_1_1_event"},
    {"struct_engine_1_1_assets_1_1_asset_1_1_event_1_1_unloaded_1a2bd5af73d6bccab761fc36bc699512f5", "struct_engine_1_1_assets_1_1_asset_1_1_event_1_1_unloaded"},
    {"struct_engine_1_1_assets_1_1_asset_1_1_event_1_1_waiting", "union_engine_1_1_assets_1_1_asset_1_1_event"},
    {"struct_engine_1_1_assets_1_1_asset_1_1_event_1_1_waiting_1acbeca9e79efb323226a26b82977d36ec", "struct_engine_1_1_assets_1_1_asset_1_1_event_1_1_waiting"},
    {"namespace_engine_1_1_audio", "namespace_engine"},
    {"namespace_engine_1_1_audio_1afd879d300eefd81460d53b2dd486fdc3", "namespace_engine_1_1_audio"},
    {"class_engine_1_1_audio_1_1_audio_buffer", "namespace_engine_1_1_audio"},
    {"class_engine_1_1_audio_1_1_audio_buffer_1ad6d10d04bef7fa259cdd5029697cf052", "class_engine_1_1_audio_1_1_audio_buffer"},
    {"class_engine_1_1_audio_1_1_audio_buffer_1ad6d10d04bef7fa259cdd5029697cf052a696b031073e74bf2cb98e5ef201d4aa3", "class_engine_1_1_audio_1_1_audio_buffer_1ad6d10d04bef7fa259cdd5029697cf052"},
    {"class_engine_1_1_audio_1_1_audio_buffer_1ad6d10d04bef7fa259cdd5029697cf052aee9d73311ff0658494edfff14c3ec1e3", "class_engine_1_1_audio_1_1_audio_buffer_1ad6d10d04bef7fa259cdd5029697cf052"},
    {"class_engine_1_1_audio_1_1_audio_buffer_1ad6d10d04bef7fa259cdd5029697cf052a5f90af42814c0a419d715d43ae54fd7a", "class_engine_1_1_audio_1_1_audio_buffer_1ad6d10d04bef7fa259cdd5029697cf052"},
    {"class_engine_1_1_audio_1_1_audio_buffer_1ad6d10d04bef7fa259cdd5029697cf052a9003f0c99274c2b98d6804fc0e39bced", "class_engine_1_1_audio_1_1_audio_buffer_1ad6d10d04bef7fa259cdd5029697cf052"},
    {"class_engine_1_1_audio_1_1_audio_buffer_1ad6d10d04bef7fa259cdd5029697cf052a6495adba09844fac8eeb0aba86e6f1bf", "class_engine_1_1_audio_1_1_audio_buffer_1ad6d10d04bef7fa259cdd5029697cf052"},
    {"class_engine_1_1_audio_1_1_audio_buffer_1ad6d10d04bef7fa259cdd5029697cf052a967d280b5c16d95f2947647dd2ca6cc2", "class_engine_1_1_audio_1_1_audio_buffer_1ad6d10d04bef7fa259cdd5029697cf052"},
    {"class_engine_1_1_audio_1_1_audio_buffer_1a2c08e837f7eac332b2003c0989047ba1", "class_engine_1_1_audio_1_1_audio_buffer"},
    {"class_engine_1_1_audio_1_1_audio_buffer_1a85edaa7e5c3ae68dabadd5373890591e", "class_engine_1_1_audio_1_1_audio_buffer"},
    {"class_engine_1_1_audio_1_1_audio_buffer_1ac2405c9952006b799c0d91e126ced774", "class_engine_1_1_audio_1_1_audio_buffer"},
    {"class_engine_1_1_audio_1_1_audio_buffer_1ae7ef7f8f4cecc7f19bbe01319f8aa37d", "class_engine_1_1_audio_1_1_audio_buffer"},
    {"class_engine_1_1_audio_1_1_audio_buffer_1ab3f8002fc80d9bff50cfb6095e10a721", "class_engine_1_1_audio_1_1_audio_buffer"},
    {"class_engine_1_1_audio_1_1_audio_buffer_1aa89013946a6eb2907227ed4c26f052b9", "class_engine_1_1_audio_1_1_audio_buffer"},
    {"class_engine_1_1_audio_1_1_audio_buffer_1a801f334ffcb3d943f1804ea73e1516ea", "class_engine_1_1_audio_1_1_audio_buffer"},
    {"class_engine_1_1_audio_1_1_audio_buffer_1a387243170ec71207123ae326c7dabd1a", "class_engine_1_1_audio_1_1_audio_buffer"},
    {"class_engine_1_1_audio_1_1_audio_buffer_1a821933214a00331f80c1c58cee859eec", "class_engine_1_1_audio_1_1_audio_buffer"},
    {"class_engine_1_1_audio_1_1_audio_buffer_1a560600772af8cde2fd5dc45e1780789c", "class_engine_1_1_audio_1_1_audio_buffer"},
    {"class_engine_1_1_audio_1_1_audio_buffer_1a93722a1dc8b7340e2c9b5e78d50151f5", "class_engine_1_1_audio_1_1_audio_buffer"},
    {"struct_engine_1_1_audio_1_1_audio_buffer_1_1_typed_audio_data", "class_engine_1_1_audio_1_1_audio_buffer"},
    {"struct_engine_1_1_audio_1_1_audio_buffer_1_1_typed_audio_data_1aca7a68c7803b2e48dc21cce6a3f672f7", "struct_engine_1_1_audio_1_1_audio_buffer_1_1_typed_audio_data"},
    {"struct_engine_1_1_audio_1_1_audio_buffer_1_1_typed_audio_data_1ab67e24ae6f8654c77d3fc3060a319e29", "struct_engine_1_1_audio_1_1_audio_buffer_1_1_typed_audio_data"},
    {"class_engine_1_1_audio_1_1_audio_manager", "namespace_engine_1_1_audio"},
    {"class_engine_1_1_audio_1_1_audio_manager_1a6ddfa33f548bda24e5968f448dd88728", "class_engine_1_1_audio_1_1_audio_manager"},
    {"class_engine_1_1_audio_1_1_audio_manager_1aadcf89776de4c18aa2f227628e8a6186", "class_engine_1_1_audio_1_1_audio_manager"},
    {"class_engine_1_1_audio_1_1_audio_manager_1ab518e57bc7c039b0ac985c74244ed027", "class_engine_1_1_audio_1_1_audio_manager"},
    {"namespace_engine_1_1_graphics", "namespace_engine"},
    {"group___graphics_1gae5b9a93609bed06adb3ce5902791df8a", "namespace_engine_1_1_graphics"},
    {"class_engine_1_1_graphics_1_1_framebuffer", "namespace_engine_1_1_graphics"},
    {"class_engine_1_1_graphics_1_1_framebuffer_1ab66e3d8f549898f3f393cedd8f5ac1ad", "class_engine_1_1_graphics_1_1_framebuffer"},
    {"class_engine_1_1_graphics_1_1_framebuffer_1a19316bb17e8c78c8272053a1494f9ce8", "class_engine_1_1_graphics_1_1_framebuffer"},
    {"class_engine_1_1_graphics_1_1_framebuffer_1a97a83fdba1f33cee37553b370be4762c", "class_engine_1_1_graphics_1_1_framebuffer"},
    {"class_engine_1_1_graphics_1_1_framebuffer_1a41ad5c8ee63a4efabecc26f931300656", "class_engine_1_1_graphics_1_1_framebuffer"},
    {"class_engine_1_1_graphics_1_1_framebuffer_1a10930c6d923758bc05c1e0958747958e", "class_engine_1_1_graphics_1_1_framebuffer"},
    {"class_engine_1_1_graphics_1_1_handle", "namespace_engine_1_1_graphics"},
    {"class_engine_1_1_graphics_1_1_handle_1a986328747b7527a0c01728f0ae351b28", "class_engine_1_1_graphics_1_1_handle"},
    {"class_engine_1_1_graphics_1_1_handle_1aab68ea12ecb749124568f6ab3cc414bc", "class_engine_1_1_graphics_1_1_handle"},
    {"class_engine_1_1_graphics_1_1_handle_1a44c63f895a2ed3c095dfd4a9605a9ee4", "class_engine_1_1_graphics_1_1_handle"},
    {"class_engine_1_1_graphics_1_1_handle_1a51c68ca867d2e6520f72ff3f55e43a19", "class_engine_1_1_graphics_1_1_handle"},
    {"class_engine_1_1_graphics_1_1_texture", "namespace_engine_1_1_graphics"},
    {"class_engine_1_1_graphics_1_1_texture_1a31280bb4ea2ce7bb099d8291ebb41bf3", "class_engine_1_1_graphics_1_1_texture"},
    {"class_engine_1_1_graphics_1_1_texture_1a31280bb4ea2ce7bb099d8291ebb41bf3a696b031073e74bf2cb98e5ef201d4aa3", "class_engine_1_1_graphics_1_1_texture_1a31280bb4ea2ce7bb099d8291ebb41bf3"},
    {"class_engine_1_1_graphics_1_1_texture_1a31280bb4ea2ce7bb099d8291ebb41bf3ab4b279046a02077466fa26cabb00c642", "class_engine_1_1_graphics_1_1_texture_1a31280bb4ea2ce7bb099d8291ebb41bf3"},
    {"class_engine_1_1_graphics_1_1_texture_1a31280bb4ea2ce7bb099d8291ebb41bf3a165c6379d01617e12de274a3952efa63", "class_engine_1_1_graphics_1_1_texture_1a31280bb4ea2ce7bb099d8291ebb41bf3"},
    {"class_engine_1_1_graphics_1_1_texture_1a31280bb4ea2ce7bb099d8291ebb41bf3a50989efe15b375dce8ba3a5714fe2543", "class_engine_1_1_graphics_1_1_texture_1a31280bb4ea2ce7bb099d8291ebb41bf3"},
    {"class_engine_1_1_graphics_1_1_texture_1a31280bb4ea2ce7bb099d8291ebb41bf3a1cd42e2af35bdfb43d791f8cecdce4a4", "class_engine_1_1_graphics_1_1_texture_1a31280bb4ea2ce7bb099d8291ebb41bf3"},
    {"class_engine_1_1_graphics_1_1_texture_1a31280bb4ea2ce7bb099d8291ebb41bf3ad9e8e4a9228ed56eceb29f0a1223b02b", "class_engine_1_1_graphics_1_1_texture_1a31280bb4ea2ce7bb099d8291ebb41bf3"},
    {"class_engine_1_1_graphics_1_1_texture_1a31280bb4ea2ce7bb099d8291ebb41bf3ace70a5db6685e6d210c3f905a8fa7d4c", "class_engine_1_1_graphics_1_1_texture_1a31280bb4ea2ce7bb099d8291ebb41bf3"},
    {"class_engine_1_1_graphics_1_1_texture_1ac00e77b373f4de42a738b6a5f0d5d2ee", "class_engine_1_1_graphics_1_1_texture"},
    {"class_engine_1_1_graphics_1_1_texture_1a65eb15ea929c3e09e18dec43eee8e7de", "class_engine_1_1_graphics_1_1_texture"},
    {"class_engine_1_1_graphics_1_1_texture_1a2aa720c8a39a34c503b0f9c7c589125d", "class_engine_1_1_graphics_1_1_texture"},
    {"class_engine_1_1_graphics_1_1_texture_1a7aa1cce1ae133a47d7a8f587ee666ad6", "class_engine_1_1_graphics_1_1_texture"},
    {"class_engine_1_1_graphics_1_1_texture_1a6ee1abf82e30d5300ae8f0c8d922e31a", "class_engine_1_1_graphics_1_1_texture"},
    {"class_engine_1_1_graphics_1_1_texture_1a23af68344fcb00113b6ca7b3483d51d3", "class_engine_1_1_graphics_1_1_texture"},
    {"class_engine_1_1_graphics_1_1_texture_1abb2d22975c917eb2f420534c945cea97", "class_engine_1_1_graphics_1_1_texture"},
    {"class_engine_1_1_graphics_1_1_texture_1ae0b09c21b3d9d3087cbc507ca62a01e8", "class_engine_1_1_graphics_1_1_texture"},
    {"class_engine_1_1_graphics_1_1_texture2_d", "namespace_engine_1_1_graphics"},
    {"class_engine_1_1_graphics_1_1_texture2_d_1a629378d74ba090e1c803ac407cb4fea5", "class_engine_1_1_graphics_1_1_texture2_d"},
    {"class_engine_1_1_graphics_1_1_texture2_d_1a9c3792d370059e1597226b6718065767", "class_engine_1_1_graphics_1_1_texture2_d"},
    {"class_engine_1_1_graphics_1_1_texture2_d_1a33f483de6390f42c9f50770eb0dea0ee", "class_engine_1_1_graphics_1_1_texture2_d"},
    {"class_engine_1_1_graphics_1_1_texture2_d_1a01bc9c7d79de56ee427850505326c48c", "class_engine_1_1_graphics_1_1_texture2_d"},
    {"class_engine_1_1_graphics_1_1_texture2_d_1aca9fcc8dbbda1fc56be5b1f07afc1aba", "class_engine_1_1_graphics_1_1_texture2_d"},
    {"class_engine_1_1_graphics_1_1_texture3_d", "namespace_engine_1_1_graphics"},
    {"class_engine_1_1_graphics_1_1_texture3_d_1a2085a15a9a5a755b553652b84e8ee8ab", "class_engine_1_1_graphics_1_1_texture3_d"},
    {"class_engine_1_1_graphics_1_1_texture3_d_1a1c29881151b000a083509a877239403a", "class_engine_1_1_graphics_1_1_texture3_d"},
    {"class_engine_1_1_graphics_1_1_texture3_d_1af602dd82ea87309879808b7f8465604f", "class_engine_1_1_graphics_1_1_texture3_d"},
    {"class_engine_1_1_graphics_1_1_texture3_d_1a8379e2913526715cdf62ce2a4a76f330", "class_engine_1_1_graphics_1_1_texture3_d"},
    {"class_engine_1_1_graphics_1_1_texture3_d_1ada1260ef0cb755e4a283c617fae8b009", "class_engine_1_1_graphics_1_1_texture3_d"},
    {"namespace_engine_1_1_utils", "namespace_engine"},
    {"class_engine_1_1_utils_1_1_array_view", "namespace_engine_1_1_utils"},
    {"class_engine_1_1_utils_1_1_array_view_1ab73615399bbbb9fd6246fa3982f2cfec", "class_engine_1_1_utils_1_1_array_view"},
    {"class_engine_1_1_utils_1_1_array_view_1a5a726545dac6e3345a746b9f23e7a088", "class_engine_1_1_utils_1_1_array_view"},
    {"class_engine_1_1_utils_1_1_array_view_1a934fde6ed35c2a3c051b11b549214cd6", "class_engine_1_1_utils_1_1_array_view"},
    {"class_engine_1_1_utils_1_1_array_view_1a3079597de87ffeffe28d3cbf144ea8d3", "class_engine_1_1_utils_1_1_array_view"},
    {"namespace_engine_1_1_utils_1_1_path", "namespace_engine_1_1_utils"},
    {"namespace_engine_1_1_utils_1_1_path_1a20779c0c7023ad09e23ae3ce81f36f1f", "namespace_engine_1_1_utils_1_1_path"},
    {"namespace_engine_1_1_utils_1_1_path_1ae52df0e81924b99e45d515f595659628", "namespace_engine_1_1_utils_1_1_path"},
    {"group___engine", "index"},
    {"group___assets", "group___engine"},
    {"group___audio", "group___engine"},
    {"group___graphics", "group___engine"},
    {"group___graphics_1gae5b9a93609bed06adb3ce5902791df8a", "group___graphics"},
    {"group___utils", "group___engine"},
    {"group___utils_1ga175f00127b5cd4fc673a52180eb80eb5", "group___utils"},
    {"dir_d44c64559bbebec7f509842c48db8b23", "index"},
    {"dir_2cd7e651a2b0059902d1cba21662f8f9", "dir_d44c64559bbebec7f509842c48db8b23"},
    {"_asset_8hpp", "dir_2cd7e651a2b0059902d1cba21662f8f9"},
    {"_assets_8hpp", "dir_2cd7e651a2b0059902d1cba21662f8f9"},
    {"dir_4efc9c1854f752659886c9117cbec549", "dir_d44c64559bbebec7f509842c48db8b23"},
    {"_audio_8hpp", "dir_4efc9c1854f752659886c9117cbec549"},
    {"_audio_buffer_8hpp", "dir_4efc9c1854f752659886c9117cbec549"},
    {"_audio_manager_8hpp", "dir_4efc9c1854f752659886c9117cbec549"},
    {"dir_b43dbc6878c2b5c121b79d76cc4886e7", "dir_d44c64559bbebec7f509842c48db8b23"},
    {"_framebuffer_8hpp", "dir_b43dbc6878c2b5c121b79d76cc4886e7"},
    {"_graphics_8hpp", "dir_b43dbc6878c2b5c121b79d76cc4886e7"},
    {"_handle_8hpp", "dir_b43dbc6878c2b5c121b79d76cc4886e7"},
    {"_renderer_8hpp", "dir_b43dbc6878c2b5c121b79d76cc4886e7"},
    {"_texture_8hpp", "dir_b43dbc6878c2b5c121b79d76cc4886e7"},
    {"_texture2_d_8hpp", "dir_b43dbc6878c2b5c121b79d76cc4886e7"},
    {"_texture3_d_8hpp", "dir_b43dbc6878c2b5c121b79d76cc4886e7"},
    {"dir_2d1d991c66e7a032b2ab80c11328e79c", "dir_d44c64559bbebec7f509842c48db8b23"},
    {"_array_view_8hpp", "dir_2d1d991c66e7a032b2ab80c11328e79c"},
    {"_config_8hpp", "dir_2d1d991c66e7a032b2ab80c11328e79c"},
    {"group___utils_1ga175f00127b5cd4fc673a52180eb80eb5", "_config_8hpp"},
    {"_config_8hpp_1a5da40ad5fb2c39d9a9a711979695fea0", "_config_8hpp"},
    {"_path_8hpp", "dir_2d1d991c66e7a032b2ab80c11328e79c"},
    {"_utils_8hpp", "dir_2d1d991c66e7a032b2ab80c11328e79c"},
    {"_engine_8hpp", "dir_d44c64559bbebec7f509842c48db8b23"},
    {"_engine_8hpp_1ae7d52a33d99eaf0d779f82301ce5432f", "_engine_8hpp"},
    {"_exception_8hpp", "dir_d44c64559bbebec7f509842c48db8b23"}
};

static void recursiveRefidPrint(const Node& node, const int indent = 0) {
    for (const auto& child : node.getChildren()) {
        if (node.getKind() == Node::Kind::DIR || node.getKind() == Node::Kind::FILE) {
            if (child->getKind() != Node::Kind::DIR && child->getKind() != Node::Kind::FILE) {
                continue;
            }
        }

        std::cout << std::string(indent, ' ') << child->getRefid() << " " << child->getName() << " (" << (child->getParent() ? child->getParent()->getRefid() : "") << ")" << std::endl;
        recursiveRefidPrint(*child, indent + 2);
    }
};

template<typename K, typename V>
static bool contains(const std::unordered_map<K, V>& map, const K& key) {
    return map.find(key) != map.end();
}

static const Node::ClassReference& find(const Node::ClassReferences& list, const std::string& key) {
    for (const auto& child : list) {
        if (child.refid == key) {
            return child;
        }
    }
    throw EXCEPTION("Class reference {} not found", key);
}

static void compare(const nlohmann::json& a, const nlohmann::json& b, const std::string& path = ".") {
    if (a.is_array()) {
        if (!b.is_array())
            throw EXCEPTION("{} expected array but got {}", path, b.dump(2));
        if (a.size() != b.size())
            throw EXCEPTION("{} expected array size {} but got {}", path, a.size(), b.size());
        for (size_t i = 0; i < a.size(); i++) {
            compare(a[i], b[i], path + "[" + std::to_string(i) + "].");
        }
    }
    if (a.is_object()) {
        if (!b.is_object())
            throw EXCEPTION("{} expected object but got {}", path, b.dump(2));
        if (a.size() != b.size())
            throw EXCEPTION("{} expected object size {} but got {}", path, a.size(), b.size());
        for (auto it = a.begin(); it != a.end(); ++it) {
            if (!b.contains(it.key())) 
                throw EXCEPTION("{} expected object key {} but got {}", path, it.key(), b.size());
            
            compare(it.value(), b[it.key()], path + it.key() + ".");
        }
    }
    if (a.is_string()) {
        if (!b.is_string())
            throw EXCEPTION("{} expected string {} but got {}", path, a.get<std::string>(), b.dump(2));
        if (a.get<std::string>() != b.get<std::string>())
            throw EXCEPTION("{} expected string {} but got {}", path, a.get<std::string>(), b.get<std::string>());
    }
    if (a.is_number_integer()) {
        if (!b.is_number_integer())
            throw EXCEPTION("{} expected int {} but got {}", path, a.get<int>(), b.dump(2));
        if (a.get<int>() != b.get<int>())
            throw EXCEPTION("{} expected int {} but got {}", path, a.get<int>(), b.get<int>());
    }
}

static void traverse(const Node& node, const std::function<void(const Node*, const Node*)>& callback) {
    for (const auto& child : node.getChildren()) {
        callback(&node, child.get());
        if (child)
            traverse(*child, callback);
    }
}

TEST_CASE("Load everything") {
    Config config;
    TextPrinter printer(config);
    JsonConverter jsonConverter(config, printer);
    Doxygen doxygen(IMPORT_DIR);

    doxygen.load();
    printer.setCache(&doxygen.getCache());
    doxygen.finalize(config, printer);

    const auto& index = doxygen.getIndex();
    //recursiveRefidPrint(index);

    SECTION("Make sure everyone has a parent") {
        traverse(doxygen.getIndex(), [](const Node* parent, const Node* node) {
            INFO(node->getRefid());
            CHECK(node->getParent() != nullptr);
        });
    }

    SECTION("Make sure everyone does not belong to index except root objects") {
        for (const auto& child : doxygen.getIndex().getChildren()) {
            traverse(*child, [&](const Node* parent, const Node* node) {
                if (!Node::isKindLanguage(parent->getKind())) return;

                INFO(node->getRefid());
                CHECK(node->getParent() != &doxygen.getIndex());
            });
        }
    }

    SECTION("Validate child-parent relation") {
        traverse(doxygen.getIndex(), [&](const Node* parent, const Node* node) {
            if (!Node::isKindLanguage(parent->getKind())) return;

            INFO(node->getRefid());
            const auto refid = parentChildRelationMap.at(node->getRefid());
            CHECK(refid == parent->getRefid());
        });
    }

    SECTION("Validate child-parent relation for groups only") {
        traverse(doxygen.getIndex(), [&](const Node* parent, const Node* node) {
            if (node->getKind() != Node::Kind::MODULE) return;

            INFO(node->getRefid());
            const auto refid = parentChildRelationMap.at(node->getRefid());
            CHECK(refid == parent->getRefid());
        });
    }

    SECTION("Validate child-parent relation for files and dirs only") {
        traverse(doxygen.getIndex(), [&](const Node* parent, const Node* node) {
            if (node->getKind() != Node::Kind::FILE && node->getKind() != Node::Kind::DIR) return;

            INFO(node->getRefid());
            const auto refid = parentChildRelationMap.at(node->getRefid());
            CHECK(refid == parent->getRefid());
        });
    }

    SECTION("Random lookup via find cuntion") {
        CHECK(index.getRefid() == "index");
        CHECK(index.find("group___engine")->getRefid() == "group___engine");
        CHECK(index.find("group___engine")->findChild("group___audio"));
        CHECK(index.find("group___engine")->findChild("group___utils"));
        CHECK(index.find("group___audio")->getParent()->getRefid() == "group___engine");
        CHECK(index.find("group___utils")->getParent()->getRefid() == "group___engine");
        CHECK(index.find("group___utils")->getKind() == Node::Kind::MODULE);
        CHECK(index.find("group___audio")->getKind() == Node::Kind::MODULE);
        CHECK(index.find("namespace_engine")->getRefid() == "namespace_engine");
        CHECK(index.find("namespace_engine")->findChild("namespace_engine_1_1_audio"));
        CHECK(index.find("namespace_engine")->findChild("namespace_engine_1_1_utils"));
        CHECK(index.find("namespace_engine_1_1_audio")->getParent()->getRefid() == "namespace_engine");
        CHECK(index.find("namespace_engine_1_1_utils")->getParent()->getRefid() == "namespace_engine");
        CHECK(index.find("namespace_engine")->getName() == "Engine");
        CHECK(index.find("namespace_engine_1_1_audio")->getName() == "Engine::Audio");
        CHECK(index.find("namespace_engine_1_1_utils")->getName() == "Engine::Utils");
        CHECK(index.find("namespace_engine_1_1_utils_1_1_path")->getName() == "Engine::Utils::Path");
        CHECK(index.find("class_engine_1_1_audio_1_1_audio_buffer_1a2c08e837f7eac332b2003c0989047ba1")->getKind() == Node::Kind::TYPEDEF);
    }

    SECTION("Classes with inheritance must have base and derived classes") {
        const auto& base = index.find("class_engine_1_1_graphics_1_1_texture2_d")->getBaseClasses();
        CHECK(find(base, "class_engine_1_1_graphics_1_1_texture").prot == Node::Visibility::PUBLIC);

        const auto& derived = index.find("class_engine_1_1_graphics_1_1_texture")->getDerivedClasses();
        CHECK(find(derived, "class_engine_1_1_graphics_1_1_texture2_d").prot == Node::Visibility::PUBLIC);
    }
    SECTION("Classes must have valid location data") {
        const auto [data, childrenData] = index.find("class_engine_1_1_audio_1_1_audio_manager")->loadData(config, printer);
        const auto& location = data.location;
        CHECK(location.file == "include/Audio/AudioManager.hpp");
        CHECK(location.line == 17);
        CHECK(location.column == 1);
    }
    /*SECTION("Validate generated JSON with pre-defined one") {
        auto data = JsonConverter::convert(*index.find("class_engine_1_1_audio_1_1_audio_buffer"));
        std::cout << data.dump(2) << std::endl;
        //REQUIRE_NOTHROW(compare(klassEngineGraphicsTexture, data));
    }*/
}
