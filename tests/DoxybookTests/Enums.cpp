#include <Doxybook/Enums.hpp>
#include <catch2/catch.hpp>

using namespace Doxybook2;

TEST_CASE("Throw if invalid enum", "Enums") {
    REQUIRE(toEnumKind("function") == Kind::FUNCTION);
    REQUIRE_THROWS(toEnumKind("unknown"));

    REQUIRE(toEnumType("functions") == Type::FUNCTIONS);
    REQUIRE_THROWS(toEnumType("unknown"));

    REQUIRE(toEnumVirtual("virtual") == Virtual::VIRTUAL);
    REQUIRE_THROWS(toEnumVirtual("unknown"));

    REQUIRE(toEnumVisibility("public") == Visibility::PUBLIC);
    REQUIRE_THROWS(toEnumVisibility("unknown"));
}
