#include "ExceptionUtils.hpp"
#include "tinyxml2/tinyxml2.h"
#include <Doxybook/Config.hpp>
#include <Doxybook/Doxygen.hpp>
#include <Doxybook/Exception.hpp>
#include <Doxybook/Node.hpp>
#include <Doxybook/Path.hpp>
#include <Doxybook/Xml.hpp>
#include <cassert>
#include <set>

#define ASSERT_FIRST_CHILD_ELEMENT(PARENT, NAME)                                                                       \
    const auto NAME = PARENT->FirstChildElement(##NAME);                                                               \
    if (NAME == nullptr)                                                                                               \
        throw DOXYGEN_EXCEPTION("Element {} not found in parent {} file {}", NAME, PARENT->Name(), inputDir);

static bool isKindAllowedLanguage(const std::string& kind) {
    static std::set<std::string> values = {
        "namespace", "class", "struct", "interface", "function", "variable", "typedef", "enum", "union", "slot", "signal"};
    return values.find(kind) != values.end();
}

static bool isKindAllowedGroup(const std::string& kind) {
    return kind == "group";
}

static bool isKindAllowedDirs(const std::string& kind) {
    return kind == "dir" || kind == "file";
}

static bool isKindAllowedPages(const std::string& kind) {
    return kind == "page";
}

static bool isKindAllowedExamples(const std::string& kind) {
    return kind == "example";
}

Doxybook2::Doxygen::Doxygen(const Config& config) : config(config), index(std::make_shared<Node>("index")) {
}

void Doxybook2::Doxygen::load(const std::string& inputDir) {
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

    // Next, pages
    for (const auto& pair : kindRefidMap) {
        if (!isKindAllowedPages(pair.first))
            continue;
        try {
            auto found = cache.find(pair.second);
            if (found == cache.end()) {
                index->children.push_back(Node::parse(cache, inputDir, pair.second, true));
                auto child = index->children.back();
                if (child->parent == nullptr) {
                    child->parent = index.get();
                }
                if (child->refid == "indexpage") {
                    child->refid = config.mainPageName;
                }
            }
        } catch (std::exception& e) {
            WARNING("Failed to parse member {} error: {}", pair.second, e.what());
        }
    }
    cleanup(index);

    // Lastly, examples (we don't need to sort these ones)
    for (const auto& pair : kindRefidMap) {
        if (!isKindAllowedExamples(pair.first))
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

    getIndexCache(cache, index);

    // Update group pointers
    updateGroupPointers(index);
}

void Doxybook2::Doxygen::updateGroupPointers(const NodePtr& node) {
    if (node->kind == Kind::MODULE) {
        for (const auto& child : node->children) {
            child->group = node.get();
        }
    }

    for (const auto& child : node->children) {
        if (child->kind == Kind::MODULE) {
            updateGroupPointers(child);
        }
    }
}

void Doxybook2::Doxygen::finalize(const TextPrinter& plainPrinter, const TextPrinter& markdownPrinter) {
    finalizeRecursively(plainPrinter, markdownPrinter, index);
}

void Doxybook2::Doxygen::finalizeRecursively(const TextPrinter& plainPrinter,
    const TextPrinter& markdownPrinter,
    const NodePtr& node) {

    for (const auto& child : node->children) {
        child->finalize(config, plainPrinter, markdownPrinter, cache);
        finalizeRecursively(plainPrinter, markdownPrinter, child);
    }
}

Doxybook2::Doxygen::KindRefidMap Doxybook2::Doxygen::getIndexKinds(const std::string& inputDir) const {
    const auto indexPath = Path::join(inputDir, "index.xml");
    Xml xml(indexPath);

    std::unordered_multimap<std::string, std::string> map;

    auto root = xml.firstChildElement("doxygenindex");
    if (!root)
        throw EXCEPTION("Unable to find root element in file {}", indexPath);

    auto compound = root.firstChildElement("compound");
    if (!compound)
        throw EXCEPTION("No <compound> element in file {}", indexPath);
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

void Doxybook2::Doxygen::getIndexCache(NodeCacheMap& cache, const NodePtr& parent) const {
    for (const auto& child : parent->children) {
        cache.insert(std::make_pair(child->refid, child));
        getIndexCache(cache, child);
    }
}

Doxybook2::NodePtr Doxybook2::Doxygen::find(const std::string& refid) const {
    try {
        return cache.at(refid);
    } catch (std::exception& e) {
        (void)e;
        throw EXCEPTION("Failed to find node from cache by refid {}", refid);
    }
}
