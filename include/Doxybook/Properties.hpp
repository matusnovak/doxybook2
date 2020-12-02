#pragma once

#include "Enums.hpp"
#include <string>
#include <vector>

namespace Doxybook2 {
    struct Param {
        std::string type;
        std::string name;
        std::string defval;
    };

    typedef std::vector<Param> Params;

    struct Properties {
        std::string type;
        std::string definition;
        std::string argsString;
        std::string initializer;
        bool isAbstract{false};
        bool isStatic{false};
        bool isConst{false};
        bool isExplicit{false};
        bool isStrong{false};
        bool isInline{false};
        bool isDefault{false};
        bool isDeleted{false};
        bool isOverride{false};
        Params params;
        Params templateParams;
        Visibility visibility;
        Virtual virt;
    };
} // namespace Doxybook2
