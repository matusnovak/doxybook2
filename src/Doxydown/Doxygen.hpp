#pragma once
#include <unordered_map>
#include <string>
#include "TextPrinter.hpp"
#include "Node.hpp"

namespace Doxydown {
    class Doxygen {
    public:
        explicit Doxygen(std::string path);
        ~Doxygen();

        const Node& getIndex() const {
            return index;
        }
    private:
        typedef std::unordered_multimap<std::string, std::string> KindRefidMap;
        typedef std::unordered_map<std::string, Node*> CacheMap;

        KindRefidMap getIndexKinds() const;

        // Creates map<refid, Node*> and processes node recursively
        void getIndexCache(CacheMap& cache, Node* node) const;

        std::string inputDir;
        // The root object that holds everything (index.xml)
        Node index;
    };
}
