#pragma once

#include "Enums.hpp"
#include "Text.hpp"
#include <string>
#include <vector>

namespace Doxybook2 {
struct Param {
    TextNode type;
    std::string name;
    TextNode defval;
};

typedef std::vector<Param> Params;

struct Properties {
    TextNode type;
    std::string definition;
    std::string argsString;
    TextNode initializer;
    bool isAbstract{false};
    bool isStatic{false};
    bool isConst{false};
    bool isExplicit{false};
    bool isStrong{false};
    bool isInline{false};
    bool isDefault{false};
    bool isDeleted{false};
    bool isOverride{false};
    bool isFinal{false};
    bool isSealed{false};
    Params params;
    Params templateParams;
    Visibility visibility;
    Virtual virt;
};
} // namespace Doxybook2
