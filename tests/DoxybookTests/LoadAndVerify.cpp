#define CATCH_CONFIG_MAIN
#include <Doxybook/Doxygen.hpp>
#include <Doxybook/Exception.hpp>
#include <Doxybook/JsonConverter.hpp>
#include <Doxybook/TextMarkdownPrinter.hpp>
#include <Doxybook/TextPlainPrinter.hpp>
#include <ExceptionUtils.hpp>
#include <catch2/catch.hpp>
#include <fmt/format.h>
#include <nlohmann/json.hpp>

using namespace Doxybook2;

static void recursiveRefidPrint(const Node& node, const int indent = 0) {
    for (const auto& child : node.getChildren()) {
        if (node.getKind() == Kind::DIR || node.getKind() == Kind::FILE) {
            if (child->getKind() != Kind::DIR && child->getKind() != Kind::FILE) {
                continue;
            }
        }

        std::cout << std::string(indent, ' ') << child->getRefid() << " " << child->getName() << " ("
                  << (child->getParent() ? child->getParent()->getRefid() : "") << ")" << std::endl;
        recursiveRefidPrint(*child, indent + 2);
    }
};

template <typename K, typename V> static bool contains(const std::unordered_map<K, V>& map, const K& key) {
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
    // recursiveRefidPrint(index);

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
                if (!isKindLanguage(parent->getKind()))
                    return;

                INFO(node->getRefid());
                CHECK(node->getParent() != &doxygen.getIndex());
            });
        }
    }

    SECTION("Random lookup via find function") {
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
        const auto [data, childrenData] = index.find("classEngine_1_1Audio_1_1AudioManager")
                                              ->loadData(config, plainPrinter, markdownPrinter, doxygen.getCache());
        const auto& location = data.location;
        CHECK(location.file == "src/Audio/AudioManager.hpp");
        CHECK(location.line == 17);
#else
        const auto [data, childrenData] = index.find("class_engine_1_1_audio_1_1_audio_manager")
                                              ->loadData(config, plainPrinter, markdownPrinter, doxygen.getCache());
        const auto& location = data.location;
        CHECK(location.file == "src/Audio/AudioManager.hpp");
        CHECK(location.line == 17);
#endif
    }
}
