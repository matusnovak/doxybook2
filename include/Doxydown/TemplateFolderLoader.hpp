#pragma once
#include "TemplateDefaultLoader.hpp"

namespace Doxydown {
    class TemplateFolderLoader : public TemplateDefaultLoader {
    public:
        explicit TemplateFolderLoader(const std::string& path);
        virtual ~TemplateFolderLoader() = default;
    };
}
