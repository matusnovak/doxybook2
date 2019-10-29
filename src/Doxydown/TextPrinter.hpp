#pragma once
#include <string>
#include "TextParser.hpp"
#include "Config.hpp"
#include "Node.hpp"

namespace Doxydown {
    class TextPrinter {
    public:
        explicit TextPrinter(const Config& config);

        std::string printMarkdown(const TextParser::Node& node) const;
        std::string printPlain(const TextParser::Node& node) const;

        void setCache(const NodeCacheMap* cache) {
            this->cache = cache;
        }
    private:
        const Config& config;
        const NodeCacheMap* cache{nullptr};
        void printMarkdown(std::stringstream& ss, const TextParser::Node& node) const;
        void printPlain(std::stringstream& ss, const TextParser::Node& node) const;
    };
}
