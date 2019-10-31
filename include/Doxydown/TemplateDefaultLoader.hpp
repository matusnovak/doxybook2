#pragma once
#include "TemplateLoader.hpp"

namespace Doxydown {
    class TemplateDefaultLoader: public TemplateLoader {
    public:
        TemplateDefaultLoader();
        virtual ~TemplateDefaultLoader() = default;

        void saveAll(const std::string& path) const;
    };
}
