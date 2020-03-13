#pragma once
#include <nlohmann/json.hpp>
#include "TextPrinter.hpp"
#include "Node.hpp"
#include "Config.hpp"

namespace Doxybook2 {
    // Declaration of the JsonConverter class
    class JsonConverter {
    public:
        // An explicit default constructor
        // The explicit function specifier informs the compiler to only accept the indicated parameters
        // (As opposed to using the normal allowance that is given to a compiler to correct for an obvious accidental mistake)
        explicit JsonConverter(const Config& config,
                               const Doxygen& doxygen,
                               const TextPrinter& plainPrinter,
                               const TextPrinter& markdownPrinter);

        // Convert the provided data to a json value, as defined in the nlohmann-json library
        nlohmann::json convert(const std::vector<std::string>& vec) const;

        // Convert the provided data to a json value, as defined in the nlohmann-json library
        nlohmann::json convert(const Node::ClassReference& klasse) const;

        // Convert the provided data to a json value, as defined in the nlohmann-json library
        nlohmann::json convert(const Node::ClassReferences& klasses) const;

        // Convert the provided data to a json value, as defined in the nlohmann-json library
        nlohmann::json convert(const Node::Location& location) const;

        // Convert the provided data to a json value, as defined in the nlohmann-json library
        nlohmann::json convert(const Node::Param& param) const;

        // Convert the provided data to a json value, as defined in the nlohmann-json library
        nlohmann::json convert(const Node::ParameterListItem& parameterItem) const;

        // Convert the provided data to a json value, as defined in the nlohmann-json library
        nlohmann::json convert(const Node::ParameterList& parameterList) const;

        // Convert the provided data to a json value, as defined in the nlohmann-json library
        nlohmann::json convert(const Node& node) const;

        // Convert the provided data to a json value, as defined in the nlohmann-json library
        nlohmann::json convert(const Node& node, const Node::Data& data) const;

        // Return the provided node object as a json object
        nlohmann::json getAsJson(const Node& node) const;

    private:
        const Config& config;
        const Doxygen& doxygen;
        const TextPrinter& plainPrinter;
        const TextPrinter& markdownPrinter;
    };
}
