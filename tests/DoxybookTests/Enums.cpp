#include <Doxybook/Config.hpp>
#include <Doxybook/Enums.hpp>
#include <catch2/catch.hpp>

using namespace Doxybook2;

TEST_CASE("Throw if invalid enum", "Enums") {
    REQUIRE(toEnumKind("function") == Kind::FUNCTION);
    REQUIRE_THROWS(toEnumKind("unknown"));
    REQUIRE(toStr(Kind::FUNCTION) == "function");

    REQUIRE(toEnumType("functions") == Type::FUNCTIONS);
    REQUIRE_THROWS(toEnumType("unknown"));
    REQUIRE(toStr(Type::FUNCTIONS) == "functions");

    REQUIRE(toEnumVirtual("virtual") == Virtual::VIRTUAL);
    REQUIRE_THROWS(toEnumVirtual("unknown"));
    REQUIRE(toStr(Virtual::VIRTUAL) == "virtual");

    REQUIRE(toEnumVisibility("public") == Visibility::PUBLIC);
    REQUIRE_THROWS(toEnumVisibility("unknown"));
    REQUIRE(toStr(Visibility::PUBLIC) == "public");
}

TEST_CASE("Check if enum kind is xyz", "Enums") {
    REQUIRE(isKindFile(Kind::CLASS) == false);
    REQUIRE(isKindFile(Kind::FILE) == true);
    REQUIRE(isKindFile(Kind::FUNCTION) == false);

    REQUIRE(isKindStructured(Kind::CLASS) == true);
    REQUIRE(isKindStructured(Kind::FILE) == true);
    REQUIRE(isKindStructured(Kind::FUNCTION) == false);

    REQUIRE(isKindLanguage(Kind::CLASS) == true);
    REQUIRE(isKindLanguage(Kind::FILE) == false);
    REQUIRE(isKindLanguage(Kind::FUNCTION) == true);
}

TEST_CASE("Enums with config", "Enums") {
    Config config{};

    REQUIRE(typeToFolderName(config, Type::MODULES) == config.folderGroupsName);

    REQUIRE(typeToIndexName(config, FolderCategory::MODULES) == config.indexGroupsName);

    REQUIRE(typeToIndexTemplate(config, FolderCategory::MODULES) == config.templateIndexGroups);

    REQUIRE(typeToIndexTitle(config, FolderCategory::MODULES) == config.indexGroupsTitle);
}
