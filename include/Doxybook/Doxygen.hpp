#pragma once

#include "Config.hpp"
#include "Node.hpp"
#include "Xml.hpp"

namespace Doxybook2 {
class Cache : public std::unordered_map<std::string, NodeSharedPtr> {
public:
    NodeSharedPtr findByRef(const std::string& refid) const {
        const auto it = this->find(refid);
        if (it == this->end()) {
            return nullptr;
        } else {
            return it->second;
        }
    }
};

namespace Doxygen {
extern Cache buildCache(const NodeSharedPtr& index);
extern void resolveReferences(const Cache& cache, const NodeSharedPtr& node);
extern void resolveHierarchy(const NodeSharedPtr& index);
extern std::string makeUrl(const Config& config, const Node& node);
extern void finalize(const Config& config, const Cache& cache, const NodeSharedPtr& index);
extern NodeSharedPtr parseIndex(const std::filesystem::path& path);
extern NodeSharedPtr parse(const std::filesystem::path& path, const std::string& refid);
extern NodeSharedPtr parse(const Xml::Element& root);
extern NodeSharedPtr parseCompound(const Xml::Element& compound);
extern NodeSharedPtr parseMember(const Xml::Element& memberdef);
extern std::vector<NodeSharedPtr> parseEnumValues(const Xml::Element& memberdef);
extern NodeSharedPtr parseEnumValue(const Xml::Element& enumvalue);
extern std::string parseRefid(const Xml::Element& elm);
extern Kind parseKind(const Xml::Element& elm);
extern Visibility parseProt(const Xml::Element& elm);
extern Virtual parseVirt(const Xml::Element& elm);
extern std::string parseCompoundName(const Xml::Element& elm);
extern Includes parseIncludes(const Xml::Element& elm);
extern Params parseTemplateParams(const Xml::Element& elm);
extern void parseProperties(const Xml::Element& elm, Properties& properties);
extern Params parseParams(const Xml::Element& elm);
extern Param parseParam(const Xml::Element& elm);
extern std::string parseName(const Xml::Element& elm);
extern std::string parseTitle(const Xml::Element& elm);
extern std::optional<Location> parseLocation(const Xml::Element& elm);
extern std::optional<BodyLocation> parseBodyLocation(const Xml::Element& elm);
extern NodeRef parseRef(const Xml::Element& elm);
extern std::vector<NodeRef> parseInnerClasses(const Xml::Element& elm);
extern std::vector<NodeRef> parseInnerNamespaces(const Xml::Element& elm);
extern std::vector<NodeRef> parseInnerFiles(const Xml::Element& elm);
extern std::vector<NodeRef> parseInnerDirectories(const Xml::Element& elm);
extern std::vector<NodeRef> parseInnerPages(const Xml::Element& elm);
extern std::vector<NodeRef> parseInnerGroups(const Xml::Element& elm);
extern std::vector<NodeRef> parseBaseCompound(const Xml::Element& elm);
extern std::vector<NodeRef> parseDerivedCompound(const Xml::Element& elm);
extern bool parseBoolAttr(const Xml::Element& elm, const std::string& key);
extern std::string parseTextBody(const Xml::Element& elm, const std::string& key);
extern std::string parseArgsString(const Xml::Element& elm);
extern std::string parseDefinition(const Xml::Element& elm);
extern TextNode parseTextNode(const Xml::Element& elm, const std::string& key);
extern TextNode parseType(const Xml::Element& elm);
extern TextNode parseInitializer(const Xml::Element& elm);
extern TextNode parseBrief(const Xml::Element& elm);
}; // namespace Doxygen
} // namespace Doxybook2
