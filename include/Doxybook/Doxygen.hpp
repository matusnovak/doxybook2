#pragma once

#include "Config.hpp"
#include "Node.hpp"
#include "Xml.hpp"

namespace Doxybook2 {
namespace Doxygen {
extern NodeSharedPtr parseIndex(const std::filesystem::path& path);
extern NodeSharedPtr parse(const std::filesystem::path& path, const std::string& refid);
extern NodeSharedPtr parseCompound(const Xml::Element& compound);
extern NodeSharedPtr parseMember(const Xml::Element& memberdef);
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
extern std::optional<Location> parseLocation(const Xml::Element& elm);
extern std::optional<BodyLocation> parseBodyLocation(const Xml::Element& elm);
extern bool parseBoolAttr(const Xml::Element& elm, const std::string& key);
extern std::string parseTextBody(const Xml::Element& elm, const std::string& key);
extern std::string parseArgsString(const Xml::Element& elm);
extern std::string parseDefinition(const Xml::Element& elm);
extern TextNode parseTextNode(const Xml::Element& elm, const std::string& key);
extern TextNode parseType(const Xml::Element& elm);
extern TextNode parseInitializer(const Xml::Element& elm);
}; // namespace Doxygen
} // namespace Doxybook2
