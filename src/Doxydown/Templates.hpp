#pragma once
#include <vector>
#include <string>

namespace Doxydown {
    namespace Templates {
        struct RawTemplate {
            const std::string key;
            const std::string content;
        };

        extern std::vector<RawTemplate> allTemplates;
    }
}
