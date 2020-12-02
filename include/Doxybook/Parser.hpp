#pragma once

#include "Config.hpp"
#include "Node.hpp"
#include "Xml.hpp"

namespace Doxybook2 {
    class Parser {
    public:
        explicit Parser(const std::filesystem::path& path);

        const Node& getIndex() const {
            return index;
        }

    private:
        void load(const std::filesystem::path& path, const std::string& refid);
        NodeSharedPtr parseCompound(const Xml::Element& compound) const;
        Kind parseKind(const Xml::Element& elm) const;
        Visibility parseProt(const Xml::Element& elm) const;
        std::string parseCompoundName(const Xml::Element& elm) const;
        Includes parseIncludes(const Xml::Element& elm) const;
        Params parseTemplateParams(const Xml::Element& elm) const;
        Param parseParam(const Xml::Element& elm) const;

        Node index;
    };
} // namespace Doxybook2
