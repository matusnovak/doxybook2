#include <Doxybook/Doxybook.hpp>

using namespace Doxybook2;

Doxybook::Doxybook(const std::filesystem::path& path) {
    index = Doxygen::parseIndex(path);
    cache = Doxygen::buildCache(index);
    Doxygen::resolveReferences(cache, index);
    Doxygen::resolveHierarchy(index);
}
