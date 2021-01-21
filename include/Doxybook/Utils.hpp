#pragma once

#include <stdexcept>

namespace Doxybook2 {
extern void backtrace(const std::exception& e, const size_t level = 0);
}
