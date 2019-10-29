#pragma once
#include <string>
#include <memory>
#include <list>
#include <vector>
#include <unordered_map>
#include "Xml.hpp"
#include "Config.hpp"

namespace Doxydown {
    class TextPrinter;
    class Xml;
    class Node;

    typedef std::shared_ptr<Node> NodePtr;
    typedef std::unordered_map<std::string, NodePtr> NodeCacheMap;

    class Node {
    public:
        enum class Kind {
            INDEX,
            DEFINE,
            CLASS,
            NAMESPACE,
            STRUCT,
            INTERFACE,
            FUNCTION,
            VARIABLE,
            TYPEDEF,
            FRIEND,
            ENUM,
            ENUMVALUE,
            UNION,
            DIR,
            FILE,
            MODULE
        };

        enum class Visibility {
            PUBLIC,
            PROTECTED,
            PRIVATE
        };

        enum class Virtual {
            NON_VIRTUAL,
            VIRTUAL,
            PURE_VIRTUAL
        };

        enum class Type {
            NONE,
            DEFINES,
            FUNCTIONS,
            NAMESPACES,
            CLASSES,
            ATTRIBUTES,
            TYPES,
            DIRORFILE,
            MODULES,
            FRIENDS
        };

        typedef std::list<NodePtr> Children;

        struct ClassReference {
            std::string name;
            std::string refid;
            Visibility prot;
            Virtual virt;
            std::string url;
        };

        struct Location {
            std::string file;
            int line{0};
            int column{0};
            std::string bodyFile;
            int bodyStart{0};
            int bodyEnd{0};
        };

        struct Param {
            std::string type;
            std::string typePlain;
            std::string name;
            std::string defval;
            std::string defvalPlain;
        };

        struct DetailsSection {
            std::string type;
            std::string text;
        };

        typedef std::vector<ClassReference> ClassReferences;
        typedef std::vector<DetailsSection> DetailsSections;
        typedef std::vector<Param> Params;

        struct Data {
            ClassReferences baseClasses;
            std::string definition;
            std::string argsString;
            std::string initializer;
            ClassReferences derivedClasses;
            Visibility visibility{ Visibility::PUBLIC };
            Virtual virt{ Virtual::NON_VIRTUAL };
            bool isAbstract{ false };
            bool isStatic{ false };
            bool isConst{ false };
            bool isExplicit{ false };
            bool isStrong{ false };
            bool isInline{ false };
            bool isDefault{ false };
            bool isDeleted{ false };
            Location location;
            std::string details;
            std::string inbody;
            std::string includes;
            std::string type;
            std::string typePlain;
            Params params;
            DetailsSections detailsSections;
        };

        typedef std::unordered_map<const Node*, Data> ChildrenData;

        // Parse root xml objects (classes, structs, etc)
        static NodePtr parse(NodeCacheMap& cache,
                             const std::string& inputDir,
                             const std::string& refid,
                             bool isGroupOrFile);

        static NodePtr parse(NodeCacheMap& cache,
                             const std::string& inputDir,
                             const NodePtr& ptr,
                             bool isGroupOrFile);

        // Parse member xml objects (functions, enums, etc)
        static NodePtr parse(Xml::Element& memberdef,
                             const std::string& refid);

        explicit Node(const std::string& refid);
        ~Node();

        NodePtr find(const std::string& refid) const;

        NodePtr findChild(const std::string& refid) const;

        bool isStructured() const {
            return isKindStructured(kind);
        }

        bool isLanguage() const {
            return isKindLanguage(kind);
        }

        bool isFileOrDir() const {
            return isKindFile(kind);
        }

        static Kind strToKind(const std::string& kind);
        static Visibility strToVisibility(const std::string& vis);
        static Virtual strToVirtual(const std::string& virt);
        static bool isKindLanguage(Kind kind);
        static bool isKindStructured(Kind kind);
        static bool isKindFile(Kind kind);
        static std::string kindToStr(Kind kind);
        static std::string visibilityToStr(Visibility prot);
        static std::string virtualToStr(Virtual virt);
        static std::string typeToStr(Type grouping);
        static const std::string& typeToFolderName(const Config& config, Type type);

        Kind getKind() const {
            return kind;
        }

        Type getType() const {
            return type;
        }

        const std::string& getRefid() const {
            return refid;
        }

        const std::string& getName() const {
            return name;
        }

        const Node* getParent() const {
            return parent;
        }

        bool isEmpty() const {
            return empty;
        }

        const Children& getChildren() const {
            return children;
        }

        const std::string& getXmlPath() const {
            return xmlPath;
        }

        const std::string& getBrief() const {
            return brief;
        }

        const std::string& getSummary() const {
            return summary;
        }

        const ClassReferences& getBaseClasses() const {
            return baseClasses;
        }

        const ClassReferences& getDerivedClasses() const {
            return derivedClasses;
        }

        const std::string& getUrl() const {
            return url;
        }

        const std::string& getAnchor() const {
            return anchor;
        }

        void finalize(const Config& config, const TextPrinter& printer, const NodeCacheMap& cache);
        std::tuple<Data, ChildrenData> loadData(const Config& config, const TextPrinter& printer) const;

        friend class Doxygen;
    private:
        class Temp;
        Data loadData(const Config& config, const TextPrinter& printer, const Xml::Element& element) const;

        std::unique_ptr<Temp> temp;
        Kind kind{Kind::INDEX};
        Type type{ Type::NONE};
        const std::string refid;
        std::string name;
        std::string brief;
        std::string summary;
        Node* parent{ nullptr };
        Children children;
        bool empty{ true };
        std::string xmlPath;
        ClassReferences baseClasses;
        ClassReferences derivedClasses;
        std::string url;
        std::string anchor;

        void parseBaseInfo(const Xml::Element& element);
        void parseInheritanceInfo(const Xml::Element& element);
        NodePtr findRecursively(const std::string& refid) const;
        static Xml::Element assertChild(const Xml::Element& xml, const std::string& name);
        static Xml::Element assertChild(const Xml& xml, const std::string& name);
    };
}
