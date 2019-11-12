#pragma once
#include "TemplateDefaultLoader.hpp"

namespace Doxybook2 {
    class TemplateFolderLoader : public TemplateDefaultLoader {
    public:
        explicit TemplateFolderLoader(const std::string& path);
        virtual ~TemplateFolderLoader() = default;
    };
}
