#include <Doxybook/Doxybook.hpp>

using namespace Doxybook2;

Doxybook::Doxybook(const Config& config, const std::filesystem::path& path) : config(config) {
    // Parse entire directory.
    // The "index" is not an index of items. It's the root.
    // Doxygen calls it index so we do as well.
    // So index != definition of "index"
    index = Doxygen::parseIndex(path);
    cache = Doxygen::buildCache(index);

    // Some nodes can reference other references
    Doxygen::resolveReferences(cache, index);

    // This will make sure that each children belongs to the correct parent
    Doxygen::resolveHierarchy(index);

    // This will resolve internal URLs and prints text nodes that will eventually be printed as markdown
    Doxygen::finalize(config, cache, index);
}
