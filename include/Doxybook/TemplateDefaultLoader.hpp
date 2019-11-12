#pragma once
#include "TemplateLoader.hpp"

namespace Doxybook2 {
    class TemplateDefaultLoader: public TemplateLoader {
    public:
        TemplateDefaultLoader();
        virtual ~TemplateDefaultLoader() = default;

        void saveAll(const std::string& path) const;
    };
}
