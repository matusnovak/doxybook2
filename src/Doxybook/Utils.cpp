#include <Doxybook/Log.hpp>
#include <Doxybook/Utils.hpp>

void Doxybook2::backtrace(const std::exception& e, const size_t level) {
    Log::e("{} {}", std::string(level * 2, ' '), e.what());
    try {
        std::rethrow_if_nested(e);
    } catch (const std::exception& ex) {
        backtrace(ex, level + 1);
    } catch (...) {
    }
}
