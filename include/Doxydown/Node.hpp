#pragma once
#include <string>
#include <memory>
#include <list>
#include <vector>
#include <unordered_map>
#include "Xml.hpp"
#include "Enums.hpp"

namespace Doxydown {
    class TextPrinter;
    class Node;
    struct Config;

    typedef std::shared_ptr<Node> NodePtr;
    typedef std::unordered_map<std::string, NodePtr> NodeCacheMap;

    class Node {
    public:
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

        Visibility getVisibility() const {
            return visibility;
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

        void finalize(const Config& config,
                      const TextPrinter& plainPrinter,
                      const TextPrinter& markdownPrinter,
                      const NodeCacheMap& cache);
        typedef std::tuple<Data, ChildrenData> LoadDataResult;
        LoadDataResult loadData(const Config& config,
                                const TextPrinter& plainPrinter,
                                const TextPrinter& markdownPrinter) const;

        friend class Doxygen;
    private:
        class Temp;
        Data loadData(const Config& config,
                      const TextPrinter& plainPrinter,
                      const TextPrinter& markdownPrinter,
                      const Xml::Element& element) const;

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
        Visibility visibility{ Visibility::PUBLIC };
        std::string url;
        std::string anchor;

        void parseBaseInfo(const Xml::Element& element);
        void parseInheritanceInfo(const Xml::Element& element);
        NodePtr findRecursively(const std::string& refid) const;
        static Xml::Element assertChild(const Xml::Element& xml, const std::string& name);
        static Xml::Element assertChild(const Xml& xml, const std::string& name);
    };
}
