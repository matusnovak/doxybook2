#pragma once
#include <unordered_map>
#include <string>
#include "TextPrinter.hpp"
#include "Node.hpp"

namespace Doxydown {
    class Doxygen {
    public:
        explicit Doxygen(std::string path);

        void load();
        void finalize(const Config& config, const TextPrinter& printer);

        const Node& getIndex() const {
            return *index;
        }

        NodePtr find(const std::string& refid) const;

        const NodeCacheMap& getCache() const {
            return cache;
        }
    private:
        typedef std::unordered_multimap<std::string, std::string> KindRefidMap;

        KindRefidMap getIndexKinds() const;
        void getIndexCache(NodeCacheMap& cache, const NodePtr& node) const;
        void finalizeRecursively(const Config& config, const TextPrinter& printer, const NodePtr& node);

        // The root object that holds everything (index.xml)
        NodePtr index;
        NodeCacheMap cache;
        std::string inputDir;
    };
}
