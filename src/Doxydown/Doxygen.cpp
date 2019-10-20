#include <set>
#include <tinyxml2.h>
#include "Doxygen.hpp"
#include "Path.hpp"
#include "Exception.hpp"
#include "Log.hpp"
#include "Xml.hpp"
#include "Node.hpp"

#define ASSERT_FIRST_CHILD_ELEMENT(PARENT, NAME) \
    const auto NAME = PARENT->FirstChildElement(##NAME);\
    if (NAME == nullptr) throw DOXYGEN_EXCEPTION("Element {} not found in parent {} file {}", NAME, PARENT->Name(), inputDir);

static bool isKindAllowedLanguage(const std::string& kind) {
    static std::set<std::string> values = {
        "namespace",
        "class",
        "struct",
        "interface",
        "function",
        "variable",
        "typedef",
        "enum"
    };
    return values.find(kind) != values.end();
}

static bool isKindAllowedGroup(const std::string& kind) {
    return kind == "group";
}

static bool isKindAllowedDirs(const std::string& kind) {
    return kind == "dir" || kind == "file";
}

Doxydown::Doxygen::Doxygen(std::string path)
    : inputDir(std::move(path)),
      index("index") {

    // Initial cache which will be discarded after the step below
    NodeCacheMap cache;

    // Remove entires from index which parent has been updated
    const auto cleanup = [](Node& node) {
        auto it = node.getChildren().begin();
        while (it != node.getChildren().end()) {
            if (it->get()->getParent() != &node) {
                node.getChildren().erase(it++);
            } else {
                ++it;
            }
        }
    };

    // Load basic information about all nodes.
    // This includes refid, brief, and list of members.
    // This won't load detailed documentation or other data! (we will do that later)
    const auto kindRefidMap = getIndexKinds();
    for (const auto& pair : kindRefidMap) {
        if (!isKindAllowedLanguage(pair.first))
            continue;
        try {
            auto found = cache.find(pair.second);
            if (found == cache.end()) {
                index.addChild(Node::parse(cache, inputDir, pair.second, false));
                auto child = index.getChildren().back();
                if (child->getParent() == nullptr) {
                    child->setParent(&index);
                }
            }
        } catch (std::exception& e) {
            WARNING("Failed to parse member {} error: {}", pair.second, e.what());
        }
    }

    cleanup(index);

    // Next, load all groups
    for (const auto& pair : kindRefidMap) {
        if (!isKindAllowedGroup(pair.first))
            continue;
        try {
            auto found = cache.find(pair.second);
            if (found == cache.end()) {
                index.addChild(Node::parse(cache, inputDir, pair.second, true));
                auto child = index.getChildren().back();
                if (child->getParent() == nullptr) {
                    child->setParent(&index);
                }
            }
        } catch (std::exception& e) {
            WARNING("Failed to parse member {} error: {}", pair.second, e.what());
        }
    }
    cleanup(index);

    // Next, load all directories and files
    for (const auto& pair : kindRefidMap) {
        if (!isKindAllowedDirs(pair.first))
            continue;
        try {
            auto found = cache.find(pair.second);
            if (found == cache.end()) {
                index.addChild(Node::parse(cache, inputDir, pair.second, true));
                auto child = index.getChildren().back();
                if (child->getParent() == nullptr) {
                    child->setParent(&index);
                }
            }
        } catch (std::exception& e) {
            WARNING("Failed to parse member {} error: {}", pair.second, e.what());
        }
    }
    cleanup(index);
}

Doxydown::Doxygen::~Doxygen() {
}

Doxydown::Doxygen::KindRefidMap Doxydown::Doxygen::getIndexKinds() const {
    const auto indexPath = Path::join(inputDir, "index.xml");
    Xml xml(indexPath);

    std::unordered_multimap<std::string, std::string> map;

    auto root = xml.firstChildElement("doxygenindex");
    if (!root) throw DOXYGEN_EXCEPTION("Unable to find root element in file {}", indexPath);

    auto compound = root.firstChildElement("compound");
    if (!compound) throw DOXYGEN_EXCEPTION("No <compound> element in file {}", indexPath);
    while (compound) {
        try {
            const auto kind = compound.getAttr("kind");
            const auto refid = compound.getAttr("refid");
            map.insert(std::make_pair(kind, refid));

        } catch (std::exception& e) {
            WARNING("compound error {}", e.what());
        }
        compound = compound.nextSiblingElement("compound");
    }

    return map;
}

void Doxydown::Doxygen::getIndexCache(CacheMap& cache, Node* parent) const {
    for (const auto& child : parent->getChildren()) {
        cache.insert(std::make_pair(child->getRefid(), child.get()));
        getIndexCache(cache, child.get());
    }
}
