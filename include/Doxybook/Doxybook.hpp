#pragma once

#include "Doxygen.hpp"

namespace Doxybook2 {
class Doxybook {
public:
    explicit Doxybook(const std::filesystem::path& path);
    ~Doxybook() = default;

    const NodeSharedPtr& getIndex() const {
        return index;
    }

private:
    NodeSharedPtr index;
    Cache cache;
};
} // namespace Doxybook2
