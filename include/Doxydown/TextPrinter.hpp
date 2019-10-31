#pragma once
#include <string>
#include "XmlTextParser.hpp"
#include "Config.hpp"
#include "Node.hpp"

namespace Doxydown {
    class Doxygen;

    class TextPrinter {
    public:
        explicit TextPrinter(const Config& config, const Doxygen& doxygen)
            : config(config),
              doxygen(doxygen) {

        }

        virtual std::string print(const XmlTextParser::Node& node) const = 0;

    protected:
        const Config& config;
        const Doxygen& doxygen;
    };
}
