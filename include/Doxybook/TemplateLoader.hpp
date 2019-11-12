#pragma once
#include <vector>
#include "Template.hpp"

namespace Doxybook2 {
    class TemplateLoader {
    public:
        TemplateLoader() = default;
        virtual ~TemplateLoader() = default;

        const std::vector<Template>& getTemplates() const {
            return templates;
        }
    protected:
        std::vector<Template> templates;
    };    
}
