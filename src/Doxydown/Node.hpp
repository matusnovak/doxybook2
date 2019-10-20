#pragma once
#include <string>
#include "Xml.hpp"

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
            CLASS,
            NAMESPACE,
            STRUCT,
            INTERFACE,
            FUNCTION,
            VARIABLE,
            TYPEDEF,
            ENUM,
            ENUMVALUE,
            DIR,
            FILE,
            GROUP
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

        typedef std::list<NodePtr> Children;

        struct DerivedClass {
            NodePtr ptr;
            Visibility prot;
            Virtual virt;
        };

        struct BaseClass {
            NodePtr ptr;
            Visibility prot;
            Virtual virt;
        };

        struct Location {
            std::string file;
            int line{0};
            int column{0};
            std::string bodyFile;
            int bodyStart{0};
            int bodyEnd{0};
        };

        typedef std::list<DerivedClass> DerivedClasses;
        typedef std::list<BaseClass> BaseClasses;

        struct Data {
            std::string brief;
            BaseClasses baseClasses;
            DerivedClasses derivedClasses;
            bool isAbstract{ false };
            Visibility prot{ Visibility::PUBLIC };
            Virtual virt{ Virtual::NON_VIRTUAL };
            bool isStatic{ false };
            Location location;
        };

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

        Kind getKind() const {
            return kind;
        }

        const std::string& getRefid() const {
            return refid;
        }

        const std::string& getName() const {
            return name;
        }

        Node* getParent() const {
            return parent;
        }

        // Use wisely
        void setParent(Node* parent) {
            this->parent = parent;
        }

        const Children& getChildren() const {
            return children;
        }

        Children& getChildren() {
            return children;
        }

        void addChild(NodePtr ptr) {
            children.push_back(std::move(ptr));
        }

        Node* getIndex() {
            if (kind == Kind::INDEX) return this;
            else return parent->getIndex();
        }

        bool isEmpty() const {
            return empty;
        }

        Data& getData() {
            return data;
        }

        const Data& getData() const {
            return data;
        }

        NodePtr find(const std::string& refid) const;

        NodePtr findChild(const std::string& refid) const;

        static Kind strToKind(const std::string& kind);
        static Visibility strToVisibility(const std::string& vis);
        static Virtual strToVirtual(const std::string& virt);
        static bool isKindLanguage(Kind kind);
        static bool isKindFile(Kind kind);
    private:
        void parseBaseInfo(const Xml::Element& element);
        void parseInheritanceInfo(const std::string& inputDir, NodeCacheMap& cache, const Xml::Element& element);

        NodePtr findRecursively(const std::string& refid) const;
        static Xml::Element assertChild(const Xml::Element& xml, const std::string& name);
        static Xml::Element assertChild(const Xml& xml, const std::string& name);

        // Required stuff
        Kind kind{Kind::INDEX};
        const std::string refid;
        std::string name;
        Node* parent{ nullptr };
        Children children;
        bool empty{ true };
        std::string xmlPath;
        Data data;
    };
}
