#include <Doxybook/Config.hpp>
#include <catch2/catch.hpp>
#include <fmt/format.h>
#include <nlohmann/json-schema.hpp>

using namespace Doxybook2;

class SchemaValidator {
public:
    explicit SchemaValidator()
        : validator(std::bind(&SchemaValidator::loader, this, std::placeholders::_1, std::placeholders::_2)) {
    }

    void set(const std::string& root) {
        validator.set_root_schema(nlohmann::json{root});
    }

    void insert(const std::string& name, const std::string& source) {
        children.insert(std::make_pair(name, nlohmann::json{source}));
    }

    void operator()(const nlohmann::json& data) const {
        (void)validator.validate(data);
    }

private:
    void loader(const nlohmann::json_uri& uri, nlohmann::json& json) {
        const auto it = children.find(uri.to_string());
        if (it == children.end()) {
            throw std::runtime_error(fmt::format("No schema named '{}'", uri.to_string()));
        }

        json = it->second;
    }

    nlohmann::json_schema::json_validator validator;
    std::unordered_map<std::string, nlohmann::json> children;
};

TEST_CASE("Schema sanity check", "Json") {
}
