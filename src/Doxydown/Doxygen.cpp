#include <set>
#include <tinyxml2.h>
#include <Doxydown/Doxygen.hpp>
#include <Doxydown/Path.hpp>
#include <Doxydown/Exception.hpp>
#include <Doxydown/Xml.hpp>
#include <Doxydown/Node.hpp>
#include "ExceptionUtils.hpp"

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

Doxydown::Doxygen::Doxygen()
    : index(std::make_shared<Node>("index")) {
}

void Doxydown::Doxygen::load(const std::string& inputDir) {
    // Remove entires from index which parent has been updated
    const auto cleanup = [](const NodePtr& node) {
        auto it = node->children.begin();
        while (it != node->children.end()) {
            if (it->get()->parent != node.get()) {
                node->children.erase(it++);
            } else {
                ++it;
            }
        }
    };

    // Load basic information about all nodes.
    // This includes refid, brief, and list of members.
    // This won't load detailed documentation or other data! (we will do that later)
    const auto kindRefidMap = getIndexKinds(inputDir);
    for (const auto& pair : kindRefidMap) {
        if (!isKindAllowedLanguage(pair.first))
            continue;
        try {
            auto found = cache.find(pair.second);
            if (found == cache.end()) {
                index->children.push_back(Node::parse(cache, inputDir, pair.second, false));
                auto child = index->children.back();
                if (child->parent == nullptr) {
                    child->parent = index.get();
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
                index->children.push_back(Node::parse(cache, inputDir, pair.second, true));
                auto child = index->children.back();
                if (child->parent == nullptr) {
                    child->parent = index.get();
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
                index->children.push_back(Node::parse(cache, inputDir, pair.second, true));
                auto child = index->children.back();
                if (child->parent == nullptr) {
                    child->parent = index.get();
                }
            }
        } catch (std::exception& e) {
            WARNING("Failed to parse member {} error: {}", pair.second, e.what());
        }
    }
    cleanup(index);

    getIndexCache(cache, index);
}

void Doxydown::Doxygen::finalize(const Config& config,
                                 const TextPrinter& plainPrinter,
                                 const TextPrinter& markdownPrinter) {
    finalizeRecursively(config, plainPrinter, markdownPrinter, index);
}

void Doxydown::Doxygen::finalizeRecursively(const Config& config,
                                            const TextPrinter& plainPrinter,
                                            const TextPrinter& markdownPrinter,
                                            const NodePtr& node) {

    for (const auto& child : node->children) {
        child->finalize(config, plainPrinter, markdownPrinter, cache);
        finalizeRecursively(config, plainPrinter, markdownPrinter, child);
    }
}

Doxydown::Doxygen::KindRefidMap Doxydown::Doxygen::getIndexKinds(const std::string& inputDir) const {
    const auto indexPath = Path::join(inputDir, "index.xml");
    Xml xml(indexPath);

    std::unordered_multimap<std::string, std::string> map;

    auto root = xml.firstChildElement("doxygenindex");
    if (!root) throw EXCEPTION("Unable to find root element in file {}", indexPath);

    auto compound = root.firstChildElement("compound");
    if (!compound) throw EXCEPTION("No <compound> element in file {}", indexPath);
    while (compound) {
        try {
            const auto kind = compound.getAttr("kind");
            const auto refid = compound.getAttr("refid");
            assert(!refid.empty());
            map.insert(std::make_pair(kind, refid));

        } catch (std::exception& e) {
            WARNING("compound error {}", e.what());
        }
        compound = compound.nextSiblingElement("compound");
    }

    return map;
}

void Doxydown::Doxygen::getIndexCache(NodeCacheMap& cache, const NodePtr& parent) const {
    for (const auto& child : parent->children) {
        cache.insert(std::make_pair(child->refid, child));
        getIndexCache(cache, child);
    }
}

Doxydown::NodePtr Doxydown::Doxygen::find(const std::string& refid) const {
    try {
        return cache.at(refid);
    } catch (std::exception& e) {
        (void)e;
        throw EXCEPTION("Failed to find node from cache by refid {}", refid);
    }
}
