#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <fmt/format.h>
#include <nlohmann/json.hpp>
#include <Doxybook/Doxygen.hpp>
#include <Doxybook/Exception.hpp>
#include <Doxybook/JsonConverter.hpp>
#include <Doxybook/TextMarkdownPrinter.hpp>
#include <Doxybook/TextPlainPrinter.hpp>
#include <ExceptionUtils.hpp>

using namespace Doxybook2;


typedef std::unordered_map<std::string, std::string> RelationMap;

#if defined(__linux__) || defined(__APPLE__)
static const RelationMap relationMap = {
    {"namespaceEngine", "index"},
    {"namespaceEngine_1ae3918a7bd8220f317f339ebda3295fa7", "namespaceEngine"},
    {"classEngine_1_1Exception", "namespaceEngine"},
    {"classEngine_1_1Exception_1ace265added7ec74ac0fe09b30d442621", "classEngine_1_1Exception"},
    {"classEngine_1_1Exception_1a697748d2c21a434b4742910b8d4e53d4", "classEngine_1_1Exception"},
    {"classEngine_1_1Exception_1a4f566b63e5d447cb468d0f89b46bb75c", "classEngine_1_1Exception"},
    {"classEngine_1_1Exception_1ac25e87274c89524b026998678500dd29", "classEngine_1_1Exception"},
    {"namespaceEngine_1_1Assets", "namespaceEngine"},
    {"classEngine_1_1Assets_1_1Asset", "namespaceEngine_1_1Assets"},
    {"classEngine_1_1Assets_1_1Asset_1af26cbe7f9237ea471f12620b0547ba39", "classEngine_1_1Assets_1_1Asset"},
    {"classEngine_1_1Assets_1_1Asset_1acb5958d164f0b8ced1638de58b3ea0b0", "classEngine_1_1Assets_1_1Asset"},
    {"classEngine_1_1Assets_1_1Asset_1aecd651b9312fa89e40d268214576839e", "classEngine_1_1Assets_1_1Asset"},
    {"classEngine_1_1Assets_1_1Asset_1a10931e97cd354bf1770b3824d0e87029", "classEngine_1_1Assets_1_1Asset"},
    {"classEngine_1_1Assets_1_1Asset_1ab25cbfe379d57b414b800c0040e7c63c", "classEngine_1_1Assets_1_1Asset"},
    {"classEngine_1_1Assets_1_1Asset_1a0f49f750cc898cd041d24bad216236a3", "classEngine_1_1Assets_1_1Asset"},
    {"unionEngine_1_1Assets_1_1Asset_1_1Event", "classEngine_1_1Assets_1_1Asset"},
    {"unionEngine_1_1Assets_1_1Asset_1_1Event_1a44fbeef7562e035debdc9e7b67a4a4d3", "unionEngine_1_1Assets_1_1Asset_1_1Event"},
    {"unionEngine_1_1Assets_1_1Asset_1_1Event_1a8f9134d4fb444dc5663b84f843f33af9", "unionEngine_1_1Assets_1_1Asset_1_1Event"},
    {"unionEngine_1_1Assets_1_1Asset_1_1Event_1ae2d2305926d305273bc1d390b15766f4", "unionEngine_1_1Assets_1_1Asset_1_1Event"},
    {"structEngine_1_1Assets_1_1Asset_1_1Event_1_1Loaded", "unionEngine_1_1Assets_1_1Asset_1_1Event"},
    {"structEngine_1_1Assets_1_1Asset_1_1Event_1_1Loaded_1a6df474ccd46caeb0e7ffb2214bb795d3", "structEngine_1_1Assets_1_1Asset_1_1Event_1_1Loaded"},
    {"structEngine_1_1Assets_1_1Asset_1_1Event_1_1Loaded_1a4560959655f932a372e0f9cee598ed1c", "structEngine_1_1Assets_1_1Asset_1_1Event_1_1Loaded"},
    {"structEngine_1_1Assets_1_1Asset_1_1Event_1_1Unloaded", "unionEngine_1_1Assets_1_1Asset_1_1Event"},
    {"structEngine_1_1Assets_1_1Asset_1_1Event_1_1Unloaded_1a2bd5af73d6bccab761fc36bc699512f5", "structEngine_1_1Assets_1_1Asset_1_1Event_1_1Unloaded"},
    {"structEngine_1_1Assets_1_1Asset_1_1Event_1_1Waiting", "unionEngine_1_1Assets_1_1Asset_1_1Event"},
    {"structEngine_1_1Assets_1_1Asset_1_1Event_1_1Waiting_1acbeca9e79efb323226a26b82977d36ec", "structEngine_1_1Assets_1_1Asset_1_1Event_1_1Waiting"},
    {"namespaceEngine_1_1Audio", "namespaceEngine"},
    {"namespaceEngine_1_1Audio_1afd879d300eefd81460d53b2dd486fdc3", "namespaceEngine_1_1Audio"},
    {"classEngine_1_1Audio_1_1AudioBuffer", "namespaceEngine_1_1Audio"},
    {"classEngine_1_1Audio_1_1AudioBuffer_1ad6d10d04bef7fa259cdd5029697cf052", "classEngine_1_1Audio_1_1AudioBuffer"},
    {"classEngine_1_1Audio_1_1AudioBuffer_1ad6d10d04bef7fa259cdd5029697cf052a696b031073e74bf2cb98e5ef201d4aa3", "classEngine_1_1Audio_1_1AudioBuffer_1ad6d10d04bef7fa259cdd5029697cf052"},
    {"classEngine_1_1Audio_1_1AudioBuffer_1ad6d10d04bef7fa259cdd5029697cf052a2dea92647909a0657c5ca657d92ff0c2", "classEngine_1_1Audio_1_1AudioBuffer_1ad6d10d04bef7fa259cdd5029697cf052"},
    {"classEngine_1_1Audio_1_1AudioBuffer_1ad6d10d04bef7fa259cdd5029697cf052aab97c2f5d9a173a78b47802dce7a1806", "classEngine_1_1Audio_1_1AudioBuffer_1ad6d10d04bef7fa259cdd5029697cf052"},
    {"classEngine_1_1Audio_1_1AudioBuffer_1ad6d10d04bef7fa259cdd5029697cf052a1b20706298a204bb53fae40ad0c97bbf", "classEngine_1_1Audio_1_1AudioBuffer_1ad6d10d04bef7fa259cdd5029697cf052"},
    {"classEngine_1_1Audio_1_1AudioBuffer_1ad6d10d04bef7fa259cdd5029697cf052a6c4baecc05d360b290e07c50131c7fc2", "classEngine_1_1Audio_1_1AudioBuffer_1ad6d10d04bef7fa259cdd5029697cf052"},
    {"classEngine_1_1Audio_1_1AudioBuffer_1ad6d10d04bef7fa259cdd5029697cf052a34429544f281bc1e40ec5525cff2a060", "classEngine_1_1Audio_1_1AudioBuffer_1ad6d10d04bef7fa259cdd5029697cf052"},
    {"classEngine_1_1Audio_1_1AudioBuffer_1a2c08e837f7eac332b2003c0989047ba1", "classEngine_1_1Audio_1_1AudioBuffer"},
    {"classEngine_1_1Audio_1_1AudioBuffer_1a85edaa7e5c3ae68dabadd5373890591e", "classEngine_1_1Audio_1_1AudioBuffer"},
    {"classEngine_1_1Audio_1_1AudioBuffer_1a8a93ebadfbe0508d87d2605f062b4b7f", "classEngine_1_1Audio_1_1AudioBuffer"},
    {"classEngine_1_1Audio_1_1AudioBuffer_1ae7ef7f8f4cecc7f19bbe01319f8aa37d", "classEngine_1_1Audio_1_1AudioBuffer"},
    {"classEngine_1_1Audio_1_1AudioBuffer_1ab3f8002fc80d9bff50cfb6095e10a721", "classEngine_1_1Audio_1_1AudioBuffer"},
    {"classEngine_1_1Audio_1_1AudioBuffer_1aa89013946a6eb2907227ed4c26f052b9", "classEngine_1_1Audio_1_1AudioBuffer"},
    {"classEngine_1_1Audio_1_1AudioBuffer_1a801f334ffcb3d943f1804ea73e1516ea", "classEngine_1_1Audio_1_1AudioBuffer"},
    {"classEngine_1_1Audio_1_1AudioBuffer_1a387243170ec71207123ae326c7dabd1a", "classEngine_1_1Audio_1_1AudioBuffer"},
    {"classEngine_1_1Audio_1_1AudioBuffer_1a821933214a00331f80c1c58cee859eec", "classEngine_1_1Audio_1_1AudioBuffer"},
    {"classEngine_1_1Audio_1_1AudioBuffer_1a560600772af8cde2fd5dc45e1780789c", "classEngine_1_1Audio_1_1AudioBuffer"},
    {"classEngine_1_1Audio_1_1AudioBuffer_1a93722a1dc8b7340e2c9b5e78d50151f5", "classEngine_1_1Audio_1_1AudioBuffer"},
    {"structEngine_1_1Audio_1_1AudioBuffer_1_1TypedAudioData", "classEngine_1_1Audio_1_1AudioBuffer"},
    {"structEngine_1_1Audio_1_1AudioBuffer_1_1TypedAudioData_1aca7a68c7803b2e48dc21cce6a3f672f7", "structEngine_1_1Audio_1_1AudioBuffer_1_1TypedAudioData"},
    {"structEngine_1_1Audio_1_1AudioBuffer_1_1TypedAudioData_1ab67e24ae6f8654c77d3fc3060a319e29", "structEngine_1_1Audio_1_1AudioBuffer_1_1TypedAudioData"},
    {"classEngine_1_1Audio_1_1AudioManager", "namespaceEngine_1_1Audio"},
    {"classEngine_1_1Audio_1_1AudioManager_1a6ddfa33f548bda24e5968f448dd88728", "classEngine_1_1Audio_1_1AudioManager"},
    {"classEngine_1_1Audio_1_1AudioManager_1aadcf89776de4c18aa2f227628e8a6186", "classEngine_1_1Audio_1_1AudioManager"},
    {"classEngine_1_1Audio_1_1AudioManager_1ab518e57bc7c039b0ac985c74244ed027", "classEngine_1_1Audio_1_1AudioManager"},
    {"namespaceEngine_1_1Graphics", "namespaceEngine"},
    {"group__Graphics_1gae5b9a93609bed06adb3ce5902791df8a", "namespaceEngine_1_1Graphics"},
    {"classEngine_1_1Graphics_1_1Framebuffer", "namespaceEngine_1_1Graphics"},
    {"classEngine_1_1Graphics_1_1Framebuffer_1ab66e3d8f549898f3f393cedd8f5ac1ad", "classEngine_1_1Graphics_1_1Framebuffer"},
    {"classEngine_1_1Graphics_1_1Framebuffer_1a19316bb17e8c78c8272053a1494f9ce8", "classEngine_1_1Graphics_1_1Framebuffer"},
    {"classEngine_1_1Graphics_1_1Framebuffer_1a97a83fdba1f33cee37553b370be4762c", "classEngine_1_1Graphics_1_1Framebuffer"},
    {"classEngine_1_1Graphics_1_1Framebuffer_1a41ad5c8ee63a4efabecc26f931300656", "classEngine_1_1Graphics_1_1Framebuffer"},
    {"classEngine_1_1Graphics_1_1Framebuffer_1a10930c6d923758bc05c1e0958747958e", "classEngine_1_1Graphics_1_1Framebuffer"},
    {"classEngine_1_1Graphics_1_1Handle", "namespaceEngine_1_1Graphics"},
    {"classEngine_1_1Graphics_1_1Handle_1a986328747b7527a0c01728f0ae351b28", "classEngine_1_1Graphics_1_1Handle"},
    {"classEngine_1_1Graphics_1_1Handle_1aab68ea12ecb749124568f6ab3cc414bc", "classEngine_1_1Graphics_1_1Handle"},
    {"classEngine_1_1Graphics_1_1Handle_1a44c63f895a2ed3c095dfd4a9605a9ee4", "classEngine_1_1Graphics_1_1Handle"},
    {"classEngine_1_1Graphics_1_1Handle_1a51c68ca867d2e6520f72ff3f55e43a19", "classEngine_1_1Graphics_1_1Handle"},
    {"classEngine_1_1Graphics_1_1Texture", "namespaceEngine_1_1Graphics"},
    {"classEngine_1_1Graphics_1_1Texture_1a31280bb4ea2ce7bb099d8291ebb41bf3", "classEngine_1_1Graphics_1_1Texture"},
    {"classEngine_1_1Graphics_1_1Texture_1a31280bb4ea2ce7bb099d8291ebb41bf3a696b031073e74bf2cb98e5ef201d4aa3", "classEngine_1_1Graphics_1_1Texture_1a31280bb4ea2ce7bb099d8291ebb41bf3"},
    {"classEngine_1_1Graphics_1_1Texture_1a31280bb4ea2ce7bb099d8291ebb41bf3aa23cde681dadb7db981dc69ef3b1eb5b", "classEngine_1_1Graphics_1_1Texture_1a31280bb4ea2ce7bb099d8291ebb41bf3"},
    {"classEngine_1_1Graphics_1_1Texture_1a31280bb4ea2ce7bb099d8291ebb41bf3a542b7a8cf84322d8b6d6ce98a6d86d84", "classEngine_1_1Graphics_1_1Texture_1a31280bb4ea2ce7bb099d8291ebb41bf3"},
    {"classEngine_1_1Graphics_1_1Texture_1a31280bb4ea2ce7bb099d8291ebb41bf3a33ba5e65ee977da3cbdbdca56a054a01", "classEngine_1_1Graphics_1_1Texture_1a31280bb4ea2ce7bb099d8291ebb41bf3"},
    {"classEngine_1_1Graphics_1_1Texture_1a31280bb4ea2ce7bb099d8291ebb41bf3ac0182f4ea7c5e9b625c36eae16149ccc", "classEngine_1_1Graphics_1_1Texture_1a31280bb4ea2ce7bb099d8291ebb41bf3"},
    {"classEngine_1_1Graphics_1_1Texture_1a31280bb4ea2ce7bb099d8291ebb41bf3aab36754052ec07e2737b63f43cc214f1", "classEngine_1_1Graphics_1_1Texture_1a31280bb4ea2ce7bb099d8291ebb41bf3"},
    {"classEngine_1_1Graphics_1_1Texture_1a31280bb4ea2ce7bb099d8291ebb41bf3ac6324250e415ea439a02714b399179a6", "classEngine_1_1Graphics_1_1Texture_1a31280bb4ea2ce7bb099d8291ebb41bf3"},
    {"classEngine_1_1Graphics_1_1Texture_1ac00e77b373f4de42a738b6a5f0d5d2ee", "classEngine_1_1Graphics_1_1Texture"},
    {"classEngine_1_1Graphics_1_1Texture_1a65eb15ea929c3e09e18dec43eee8e7de", "classEngine_1_1Graphics_1_1Texture"},
    {"classEngine_1_1Graphics_1_1Texture_1a2aa720c8a39a34c503b0f9c7c589125d", "classEngine_1_1Graphics_1_1Texture"},
    {"classEngine_1_1Graphics_1_1Texture_1a7aa1cce1ae133a47d7a8f587ee666ad6", "classEngine_1_1Graphics_1_1Texture"},
    {"classEngine_1_1Graphics_1_1Texture_1a6ee1abf82e30d5300ae8f0c8d922e31a", "classEngine_1_1Graphics_1_1Texture"},
    {"classEngine_1_1Graphics_1_1Texture_1a23af68344fcb00113b6ca7b3483d51d3", "classEngine_1_1Graphics_1_1Texture"},
    {"classEngine_1_1Graphics_1_1Texture_1abb2d22975c917eb2f420534c945cea97", "classEngine_1_1Graphics_1_1Texture"},
    {"classEngine_1_1Graphics_1_1Texture_1ae0b09c21b3d9d3087cbc507ca62a01e8", "classEngine_1_1Graphics_1_1Texture"},
    {"classEngine_1_1Graphics_1_1Texture2D", "namespaceEngine_1_1Graphics"},
    {"classEngine_1_1Graphics_1_1Texture2D_1a629378d74ba090e1c803ac407cb4fea5", "classEngine_1_1Graphics_1_1Texture2D"},
    {"classEngine_1_1Graphics_1_1Texture2D_1a9c3792d370059e1597226b6718065767", "classEngine_1_1Graphics_1_1Texture2D"},
    {"classEngine_1_1Graphics_1_1Texture2D_1a33f483de6390f42c9f50770eb0dea0ee", "classEngine_1_1Graphics_1_1Texture2D"},
    {"classEngine_1_1Graphics_1_1Texture2D_1a01bc9c7d79de56ee427850505326c48c", "classEngine_1_1Graphics_1_1Texture2D"},
    {"classEngine_1_1Graphics_1_1Texture2D_1aca9fcc8dbbda1fc56be5b1f07afc1aba", "classEngine_1_1Graphics_1_1Texture2D"},
    {"classEngine_1_1Graphics_1_1Texture3D", "namespaceEngine_1_1Graphics"},
    {"classEngine_1_1Graphics_1_1Texture3D_1a2085a15a9a5a755b553652b84e8ee8ab", "classEngine_1_1Graphics_1_1Texture3D"},
    {"classEngine_1_1Graphics_1_1Texture3D_1a1c29881151b000a083509a877239403a", "classEngine_1_1Graphics_1_1Texture3D"},
    {"classEngine_1_1Graphics_1_1Texture3D_1af602dd82ea87309879808b7f8465604f", "classEngine_1_1Graphics_1_1Texture3D"},
    {"classEngine_1_1Graphics_1_1Texture3D_1a8379e2913526715cdf62ce2a4a76f330", "classEngine_1_1Graphics_1_1Texture3D"},
    {"classEngine_1_1Graphics_1_1Texture3D_1ada1260ef0cb755e4a283c617fae8b009", "classEngine_1_1Graphics_1_1Texture3D"},
    {"namespaceEngine_1_1Utils", "namespaceEngine"},
    {"classEngine_1_1Utils_1_1ArrayView", "namespaceEngine_1_1Utils"},
    {"classEngine_1_1Utils_1_1ArrayView_1ab73615399bbbb9fd6246fa3982f2cfec", "classEngine_1_1Utils_1_1ArrayView"},
    {"classEngine_1_1Utils_1_1ArrayView_1a5a726545dac6e3345a746b9f23e7a088", "classEngine_1_1Utils_1_1ArrayView"},
    {"classEngine_1_1Utils_1_1ArrayView_1a934fde6ed35c2a3c051b11b549214cd6", "classEngine_1_1Utils_1_1ArrayView"},
    {"classEngine_1_1Utils_1_1ArrayView_1a3079597de87ffeffe28d3cbf144ea8d3", "classEngine_1_1Utils_1_1ArrayView"},
    {"namespaceEngine_1_1Utils_1_1Path", "namespaceEngine_1_1Utils"},
    {"namespaceEngine_1_1Utils_1_1Path_1a20779c0c7023ad09e23ae3ce81f36f1f", "namespaceEngine_1_1Utils_1_1Path"},
    {"namespaceEngine_1_1Utils_1_1Path_1ae52df0e81924b99e45d515f595659628", "namespaceEngine_1_1Utils_1_1Path"},
    {"group__Engine", "index"},
    {"group__Assets", "group__Engine"},
    {"group__Audio", "group__Engine"},
    {"group__Graphics", "group__Engine"},
    {"group__Utils", "group__Engine"},
    {"group__Utils_1ga175f00127b5cd4fc673a52180eb80eb5", "group__Utils"},
    {"group__Utils_1gac7c70714c30c998f33d7cb5639cec5fc", "group__Utils"},
    {"group__Utils_1gaef03bfdf7ea06e105903c76f06f22deb", "namespaceEngine_1_1Utils"},
    {"dir_68267d1309a1af8e8297ef4c3efbcdba", "index"},
    {"dir_e0c1ede47f21c4a643a1257f949f98e0", "dir_68267d1309a1af8e8297ef4c3efbcdba"},
    {"Asset_8hpp", "dir_e0c1ede47f21c4a643a1257f949f98e0"},
    {"Assets_8hpp", "dir_e0c1ede47f21c4a643a1257f949f98e0"},
    {"dir_c64386e93f356b78d4612c7b7741707c", "dir_68267d1309a1af8e8297ef4c3efbcdba"},
    {"Audio_8hpp", "dir_c64386e93f356b78d4612c7b7741707c"},
    {"AudioBuffer_8hpp", "dir_c64386e93f356b78d4612c7b7741707c"},
    {"AudioManager_8hpp", "dir_c64386e93f356b78d4612c7b7741707c"},
    {"dir_3419e189e460663ec2c964233bf8dcd6", "dir_68267d1309a1af8e8297ef4c3efbcdba"},
    {"Framebuffer_8hpp", "dir_3419e189e460663ec2c964233bf8dcd6"},
    {"Graphics_8hpp", "dir_3419e189e460663ec2c964233bf8dcd6"},
    {"Handle_8hpp", "dir_3419e189e460663ec2c964233bf8dcd6"},
    {"Renderer_8hpp", "dir_3419e189e460663ec2c964233bf8dcd6"},
    {"Texture_8hpp", "dir_3419e189e460663ec2c964233bf8dcd6"},
    {"Texture2D_8hpp", "dir_3419e189e460663ec2c964233bf8dcd6"},
    {"Texture3D_8hpp", "dir_3419e189e460663ec2c964233bf8dcd6"},
    {"dir_a7363e98f9e0bdd87618633653859815", "dir_68267d1309a1af8e8297ef4c3efbcdba"},
    {"ArrayView_8hpp", "dir_a7363e98f9e0bdd87618633653859815"},
    {"Config_8hpp", "dir_a7363e98f9e0bdd87618633653859815"},
    {"Config_8hpp_1a5da40ad5fb2c39d9a9a711979695fea0", "Config_8hpp"},
    {"Log_8hpp", "dir_a7363e98f9e0bdd87618633653859815"},
    {"Path_8hpp", "dir_a7363e98f9e0bdd87618633653859815"},
    {"Utils_8hpp", "dir_a7363e98f9e0bdd87618633653859815"},
    {"Engine_8hpp", "dir_68267d1309a1af8e8297ef4c3efbcdba"},
    {"Engine_8hpp_1ae7d52a33d99eaf0d779f82301ce5432f", "Engine_8hpp"},
    {"Exception_8hpp", "dir_68267d1309a1af8e8297ef4c3efbcdba"},
};
#else 
static const RelationMap relationMap = {
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
    {"class_engine_1_1_audio_1_1_audio_buffer_1ad6d10d04bef7fa259cdd5029697cf052a34429544f281bc1e40ec5525cff2a060", "class_engine_1_1_audio_1_1_audio_buffer_1ad6d10d04bef7fa259cdd5029697cf052"},
    {"class_engine_1_1_audio_1_1_audio_buffer_1ad6d10d04bef7fa259cdd5029697cf052a6c4baecc05d360b290e07c50131c7fc2", "class_engine_1_1_audio_1_1_audio_buffer_1ad6d10d04bef7fa259cdd5029697cf052"},
    {"class_engine_1_1_audio_1_1_audio_buffer_1ad6d10d04bef7fa259cdd5029697cf052a1b20706298a204bb53fae40ad0c97bbf", "class_engine_1_1_audio_1_1_audio_buffer_1ad6d10d04bef7fa259cdd5029697cf052"},
    {"class_engine_1_1_audio_1_1_audio_buffer_1ad6d10d04bef7fa259cdd5029697cf052aab97c2f5d9a173a78b47802dce7a1806", "class_engine_1_1_audio_1_1_audio_buffer_1ad6d10d04bef7fa259cdd5029697cf052"},
    {"class_engine_1_1_audio_1_1_audio_buffer_1ad6d10d04bef7fa259cdd5029697cf052a2dea92647909a0657c5ca657d92ff0c2", "class_engine_1_1_audio_1_1_audio_buffer_1ad6d10d04bef7fa259cdd5029697cf052"},
    {"class_engine_1_1_audio_1_1_audio_buffer_1ad6d10d04bef7fa259cdd5029697cf052a696b031073e74bf2cb98e5ef201d4aa3", "class_engine_1_1_audio_1_1_audio_buffer_1ad6d10d04bef7fa259cdd5029697cf052"},
    {"class_engine_1_1_audio_1_1_audio_buffer_1ad6d10d04bef7fa259cdd5029697cf052", "class_engine_1_1_audio_1_1_audio_buffer"},
    {"class_engine_1_1_audio_1_1_audio_buffer_1a2c08e837f7eac332b2003c0989047ba1", "class_engine_1_1_audio_1_1_audio_buffer"},
    {"class_engine_1_1_audio_1_1_audio_buffer_1a85edaa7e5c3ae68dabadd5373890591e", "class_engine_1_1_audio_1_1_audio_buffer"},
    {"class_engine_1_1_audio_1_1_audio_buffer_1a8a93ebadfbe0508d87d2605f062b4b7f", "class_engine_1_1_audio_1_1_audio_buffer"},
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
    {"class_engine_1_1_graphics_1_1_texture_1a31280bb4ea2ce7bb099d8291ebb41bf3ac6324250e415ea439a02714b399179a6", "class_engine_1_1_graphics_1_1_texture_1a31280bb4ea2ce7bb099d8291ebb41bf3"},
    {"class_engine_1_1_graphics_1_1_texture_1a31280bb4ea2ce7bb099d8291ebb41bf3aab36754052ec07e2737b63f43cc214f1", "class_engine_1_1_graphics_1_1_texture_1a31280bb4ea2ce7bb099d8291ebb41bf3"},
    {"class_engine_1_1_graphics_1_1_texture_1a31280bb4ea2ce7bb099d8291ebb41bf3ac0182f4ea7c5e9b625c36eae16149ccc", "class_engine_1_1_graphics_1_1_texture_1a31280bb4ea2ce7bb099d8291ebb41bf3"},
    {"class_engine_1_1_graphics_1_1_texture_1a31280bb4ea2ce7bb099d8291ebb41bf3a33ba5e65ee977da3cbdbdca56a054a01", "class_engine_1_1_graphics_1_1_texture_1a31280bb4ea2ce7bb099d8291ebb41bf3"},
    {"class_engine_1_1_graphics_1_1_texture_1a31280bb4ea2ce7bb099d8291ebb41bf3a542b7a8cf84322d8b6d6ce98a6d86d84", "class_engine_1_1_graphics_1_1_texture_1a31280bb4ea2ce7bb099d8291ebb41bf3"},
    {"class_engine_1_1_graphics_1_1_texture_1a31280bb4ea2ce7bb099d8291ebb41bf3aa23cde681dadb7db981dc69ef3b1eb5b", "class_engine_1_1_graphics_1_1_texture_1a31280bb4ea2ce7bb099d8291ebb41bf3"},
    {"class_engine_1_1_graphics_1_1_texture_1a31280bb4ea2ce7bb099d8291ebb41bf3a696b031073e74bf2cb98e5ef201d4aa3", "class_engine_1_1_graphics_1_1_texture_1a31280bb4ea2ce7bb099d8291ebb41bf3"},
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
    {"group___utils", "group___engine"},
    {"group___utils_1ga175f00127b5cd4fc673a52180eb80eb5", "group___utils"},
    {"group___utils_1gac7c70714c30c998f33d7cb5639cec5fc", "group___utils"},
    {"group___utils_1gaef03bfdf7ea06e105903c76f06f22deb", "namespace_engine_1_1_utils"},
    {"dir_68267d1309a1af8e8297ef4c3efbcdba", "index"},
    {"dir_e0c1ede47f21c4a643a1257f949f98e0", "dir_68267d1309a1af8e8297ef4c3efbcdba"},
    {"_asset_8hpp", "dir_e0c1ede47f21c4a643a1257f949f98e0"},
    {"_assets_8hpp", "dir_e0c1ede47f21c4a643a1257f949f98e0"},
    {"dir_c64386e93f356b78d4612c7b7741707c", "dir_68267d1309a1af8e8297ef4c3efbcdba"},
    {"_audio_8hpp", "dir_c64386e93f356b78d4612c7b7741707c"},
    {"_audio_buffer_8hpp", "dir_c64386e93f356b78d4612c7b7741707c"},
    {"_audio_manager_8hpp", "dir_c64386e93f356b78d4612c7b7741707c"},
    {"dir_3419e189e460663ec2c964233bf8dcd6", "dir_68267d1309a1af8e8297ef4c3efbcdba"},
    {"_framebuffer_8hpp", "dir_3419e189e460663ec2c964233bf8dcd6"},
    {"_graphics_8hpp", "dir_3419e189e460663ec2c964233bf8dcd6"},
    {"_handle_8hpp", "dir_3419e189e460663ec2c964233bf8dcd6"},
    {"_renderer_8hpp", "dir_3419e189e460663ec2c964233bf8dcd6"},
    {"_texture_8hpp", "dir_3419e189e460663ec2c964233bf8dcd6"},
    {"_texture2_d_8hpp", "dir_3419e189e460663ec2c964233bf8dcd6"},
    {"_texture3_d_8hpp", "dir_3419e189e460663ec2c964233bf8dcd6"},
    {"dir_a7363e98f9e0bdd87618633653859815", "dir_68267d1309a1af8e8297ef4c3efbcdba"},
    {"_array_view_8hpp", "dir_a7363e98f9e0bdd87618633653859815"},
    {"_config_8hpp", "dir_a7363e98f9e0bdd87618633653859815"},
    {"_config_8hpp_1a5da40ad5fb2c39d9a9a711979695fea0", "_config_8hpp"},
    {"_log_8hpp", "dir_a7363e98f9e0bdd87618633653859815"},
    {"_path_8hpp", "dir_a7363e98f9e0bdd87618633653859815"},
    {"_utils_8hpp", "dir_a7363e98f9e0bdd87618633653859815"},
    {"_engine_8hpp", "dir_68267d1309a1af8e8297ef4c3efbcdba"},
    {"_engine_8hpp_1ae7d52a33d99eaf0d779f82301ce5432f", "_engine_8hpp"},
    {"_exception_8hpp", "dir_68267d1309a1af8e8297ef4c3efbcdba"},
};
#endif

static void recursiveRefidPrint(const Node& node, const int indent = 0) {
    for (const auto& child : node.getChildren()) {
        if (node.getKind() == Kind::DIR || node.getKind() == Kind::FILE) {
            if (child->getKind() != Kind::DIR && child->getKind() != Kind::FILE) {
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

static nlohmann::json jsonFile(const std::string& path) {
    std::ifstream file(path);
    std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return nlohmann::json::parse(str);
}

TEST_CASE("Load everything") {
    Config config;
    config.copyImages = false;
    Doxygen doxygen(config);
    TextPlainPrinter plainPrinter(config, doxygen);
    TextMarkdownPrinter markdownPrinter(config, IMPORT_DIR, doxygen);
    JsonConverter jsonConverter(config, doxygen, plainPrinter, markdownPrinter);

    doxygen.load(IMPORT_DIR);
    doxygen.finalize(plainPrinter, markdownPrinter);

    const auto& index = doxygen.getIndex();
    //recursiveRefidPrint(index);

    SECTION("print child -> parent relation") {
        std::set<std::string> temp;
        traverse(doxygen.getIndex(), [&](const Node* parent, const Node* node) {
            if (temp.find(node->getRefid()) == temp.end()) {
                std::cout << "\"" << node->getRefid() << "\" -> \"" << parent->getRefid() << "\"" << std::endl;
                temp.insert(node->getRefid());
            }
        });
    }

    SECTION("Make sure everyone has a parent") {
        traverse(doxygen.getIndex(), [](const Node* parent, const Node* node) {
            INFO(node->getRefid());
            CHECK(node->getParent() != nullptr);
        });
    }

    SECTION("Make sure everyone does not belong to index except root objects") {
        for (const auto& child : doxygen.getIndex().getChildren()) {
            traverse(*child, [&](const Node* parent, const Node* node) {
                if (!isKindLanguage(parent->getKind())) return;

                INFO(node->getRefid());
                CHECK(node->getParent() != &doxygen.getIndex());
            });
        }
    }

    SECTION("Validate child-parent relation") {
        traverse(doxygen.getIndex(), [&](const Node* parent, const Node* node) {
            if (!isKindLanguage(parent->getKind())) return;

            INFO(node->getRefid());
            const auto refid = relationMap.at(node->getRefid());
            CHECK(refid == parent->getRefid());
        });
    }

    SECTION("Validate child-parent relation for groups only") {
        traverse(doxygen.getIndex(), [&](const Node* parent, const Node* node) {
            if (node->getKind() != Kind::MODULE) return;

            INFO(node->getRefid());
            const auto refid = relationMap.at(node->getRefid());
            CHECK(refid == parent->getRefid());
        });
    }

    SECTION("Validate child-parent relation for files and dirs only") {
        traverse(doxygen.getIndex(), [&](const Node* parent, const Node* node) {
            if (node->getKind() != Kind::FILE && node->getKind() != Kind::DIR) return;

            INFO(node->getRefid());
            const auto refid = relationMap.at(node->getRefid());
            CHECK(refid == parent->getRefid());
        });
    }

    SECTION("Random lookup via find cuntion") {
#if defined(__linux__) || defined(__APPLE__)
        CHECK(index.getRefid() == "index");
        CHECK(index.find("group__Engine")->getRefid() == "group__Engine");
        CHECK(index.find("group__Engine")->findChild("group__Audio"));
        CHECK(index.find("group__Engine")->findChild("group__Utils"));
        CHECK(index.find("group__Audio")->getParent()->getRefid() == "group__Engine");
        CHECK(index.find("group__Utils")->getParent()->getRefid() == "group__Engine");
        CHECK(index.find("group__Utils")->getKind() == Kind::MODULE);
        CHECK(index.find("group__Audio")->getKind() == Kind::MODULE);
        CHECK(index.find("namespaceEngine")->getRefid() == "namespaceEngine");
        CHECK(index.find("namespaceEngine")->findChild("namespaceEngine_1_1Audio"));
        CHECK(index.find("namespaceEngine")->findChild("namespaceEngine_1_1Utils"));
        CHECK(index.find("namespaceEngine_1_1Audio")->getParent()->getRefid() == "namespaceEngine");
        CHECK(index.find("namespaceEngine_1_1Utils")->getParent()->getRefid() == "namespaceEngine");
        CHECK(index.find("namespaceEngine")->getName() == "Engine");
        CHECK(index.find("namespaceEngine_1_1Audio")->getName() == "Engine::Audio");
        CHECK(index.find("namespaceEngine_1_1Utils")->getName() == "Engine::Utils");
        CHECK(index.find("namespaceEngine_1_1Utils_1_1Path")->getName() == "Engine::Utils::Path");
        CHECK(index.find("classEngine_1_1Audio_1_1AudioBuffer_1a2c08e837f7eac332b2003c0989047ba1")->getKind() == Kind::TYPEDEF);
#else
        CHECK(index.getRefid() == "index");
        CHECK(index.find("group___engine")->getRefid() == "group___engine");
        CHECK(index.find("group___engine")->findChild("group___audio"));
        CHECK(index.find("group___engine")->findChild("group___utils"));
        CHECK(index.find("group___audio")->getParent()->getRefid() == "group___engine");
        CHECK(index.find("group___utils")->getParent()->getRefid() == "group___engine");
        CHECK(index.find("group___utils")->getKind() == Kind::MODULE);
        CHECK(index.find("group___audio")->getKind() == Kind::MODULE);
        CHECK(index.find("namespace_engine")->getRefid() == "namespace_engine");
        CHECK(index.find("namespace_engine")->findChild("namespace_engine_1_1_audio"));
        CHECK(index.find("namespace_engine")->findChild("namespace_engine_1_1_utils"));
        CHECK(index.find("namespace_engine_1_1_audio")->getParent()->getRefid() == "namespace_engine");
        CHECK(index.find("namespace_engine_1_1_utils")->getParent()->getRefid() == "namespace_engine");
        CHECK(index.find("namespace_engine")->getName() == "Engine");
        CHECK(index.find("namespace_engine_1_1_audio")->getName() == "Engine::Audio");
        CHECK(index.find("namespace_engine_1_1_utils")->getName() == "Engine::Utils");
        CHECK(index.find("namespace_engine_1_1_utils_1_1_path")->getName() == "Engine::Utils::Path");
        CHECK(index.find("class_engine_1_1_audio_1_1_audio_buffer_1a2c08e837f7eac332b2003c0989047ba1")->getKind() == Kind::TYPEDEF);
#endif
    }

    SECTION("Classes with inheritance must have base and derived classes") {
#if defined(__linux__) || defined(__APPLE__)
        const auto& base = index.find("classEngine_1_1Graphics_1_1Texture2D")->getBaseClasses();
        CHECK(find(base, "classEngine_1_1Graphics_1_1Texture").prot == Visibility::PUBLIC);

        const auto& derived = index.find("classEngine_1_1Graphics_1_1Texture")->getDerivedClasses();
        CHECK(find(derived, "classEngine_1_1Graphics_1_1Texture2D").prot == Visibility::PUBLIC);
#else
        const auto& base = index.find("class_engine_1_1_graphics_1_1_texture2_d")->getBaseClasses();
        CHECK(find(base, "class_engine_1_1_graphics_1_1_texture").prot == Visibility::PUBLIC);

        const auto& derived = index.find("class_engine_1_1_graphics_1_1_texture")->getDerivedClasses();
        CHECK(find(derived, "class_engine_1_1_graphics_1_1_texture2_d").prot == Visibility::PUBLIC);
#endif
    }
    SECTION("Classes must have valid location data") {
#if defined(__linux__) || defined(__APPLE__)
        const auto[data, childrenData] = index.find("classEngine_1_1Audio_1_1AudioManager")->loadData(config, plainPrinter, markdownPrinter, doxygen.getCache());
        const auto& location = data.location;
        CHECK(location.file == "src/Audio/AudioManager.hpp");
        CHECK(location.line == 17);
#else
        const auto [data, childrenData] = index.find("class_engine_1_1_audio_1_1_audio_manager")->loadData(config, plainPrinter, markdownPrinter, doxygen.getCache());
        const auto& location = data.location;
        CHECK(location.file == "src/Audio/AudioManager.hpp");
        CHECK(location.line == 17);
#endif
    }
}
