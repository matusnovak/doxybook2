#pragma once
#include <unordered_map>
#include <string>
#include "Node.hpp"

namespace Doxybook2 {
    // Declaration of the TextPrinter class
    class TextPrinter;

    // Declaration of the Doxygen class
    class Doxygen {
    public:
        // Constructor for the Doxygen class
        // The "explicit" function specifier reqiures that the compiler never attempt to automatically correct a human mistake
        // that may occur when the human writes the code that calls a construction of a class instance
        explicit Doxygen(const Config& config);

        // The destructor
        // The "virtual" function specifier indicates that classes derives from the Doxybook2 class can 
        // provide a different destructor
        virtual ~Doxygen() = default;

        // The load function loads data for a node
        void load(const std::string& inputDir);

        // The finalize function (cleans up data in a node recursively?)
        void finalize(const TextPrinter& plainPrinter, const TextPrinter& markdownPrinter);

        // Get the index
        const Node& getIndex() const {
            return *index;
        }

        // Find a NodePtr object based on a provided refid
        NodePtr find(const std::string& refid) const;

        // Get the NodeCacheMap cache object of this class
        const NodeCacheMap& getCache() const {
            return cache;
        }
    private:
        // A KindRefidMap is an alias to the std::unordered_multimap<std::string, std::string> type
        typedef std::unordered_multimap<std::string, std::string> KindRefidMap;

        // Get the index kinds and return a KindRefidMap
        KindRefidMap getIndexKinds(const std::string& inputDir) const;

        // Get the index's cache
        void getIndexCache(NodeCacheMap& cache, const NodePtr& node) const;

        // Finalize each child of the provided node recursively
        void finalizeRecursively(const TextPrinter& plainPrinter,
                                 const TextPrinter& markdownPrinter,
                                 const NodePtr& node);

        // Update group pointers
        void updateGroupPointers(const NodePtr& node);

        // Declaration of a Config object
        const Config& config;

        // The root object that holds everything (index.xml)
        NodePtr index;

        NodeCacheMap cache;
    };
}
