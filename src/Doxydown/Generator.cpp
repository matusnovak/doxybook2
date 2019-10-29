#ifdef _WIN32
#define NOMINMAX
#include <Windows.h>
#else
#include <sys/stat.h>
#endif

#include <fstream>
#include "Generator.hpp"
#include "Doxygen.hpp"
#include "Renderer.hpp"
#include "Path.hpp"
#include "Templates.hpp"
#include "Exception.hpp"
#include "TextUtils.hpp"

static void createDirectory(const std::string& path) {
#ifdef _WIN32
    if (!CreateDirectoryA(path.c_str(), NULL) && ERROR_ALREADY_EXISTS != GetLastError()) {
        throw EXCEPTION("Failed to create directory {} error {}", path, int(GetLastError()));
    }
#else
    const auto err = mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    if (err != 0 && err != EEXIST) {
        throw EXCEPTION("Failed to create directory {} error {}", path, err);
    }
#endif
}

std::string Doxydown::Generator::kindToTemplateName(const Node::Kind kind) {
    using namespace Doxydown;
    switch (kind) {
        case Node::Kind::STRUCT:
            return config.templateKindStruct;
        case Node::Kind::INTERFACE:
            return config.templateKindInterface;
        case Node::Kind::UNION:
            return config.templateKindUnion;
        case Node::Kind::CLASS:
            return config.templateKindClass;
        case Node::Kind::NAMESPACE:
            return config.templateKindNamespace;
        case Node::Kind::MODULE:
            return config.templateKindGroup;
        default: {
            throw EXCEPTION("Unrecognised kind {} please contant the author!", int(kind));
        }
    }
}

Doxydown::Generator::Generator(const Config& config, const JsonConverter& jsonConverter)
    : config(config),
      jsonConverter(jsonConverter),
      renderer(config) {

    for (const auto& pair : Templates::allTemplates) {
        renderer.addTemplate(pair.key, pair.content);
    }
}

void Doxydown::Generator::printRecursively(const Node& parent) {
    using namespace Doxydown;

    for (const auto& child : parent.getChildren()) {
        if (child->isStructured() || child->getKind() == Node::Kind::MODULE) {
            nlohmann::json data = jsonConverter.getAsJson(*child);

            const auto path = Path::join(
                Node::typeToFolderName(config, child->getType()),
                child->getRefid() + ".md"
            );

            dump(path + ".json", data);

            renderer.render(
                kindToTemplateName(child->getKind()),
                path,
                data
            );
            printRecursively(*child);
        }
    }
}

void Doxydown::Generator::print(const Doxygen& doxygen) {
    static const std::array<Node::Type, 4> ALL_GROUPS = {
        Node::Type::CLASSES,
        Node::Type::NAMESPACES,
        Node::Type::DIRORFILE,
        Node::Type::MODULES
    };
    for (const auto& g : ALL_GROUPS) {
        createDirectory(Path::join(config.outputDir, Node::typeToFolderName(config, g)));
    }

    printRecursively(doxygen.getIndex());
}

void Doxydown::Generator::printIndex(const Doxygen& doxygen, const std::string& name, const Filter& filter) {
    const auto path = indexToPath(name);

    nlohmann::json data;
    data["children"] = buildIndexRecursively(doxygen.getIndex(), filter);
    dump(path + ".json", data);
    renderer.render(indexToTemplateName(name), path, data);
}

nlohmann::json Doxydown::Generator::buildIndexRecursively(const Node& node, const Filter& filter) {
    auto json = nlohmann::json::array();
    std::vector<const Node*> sorted;
    sorted.reserve(node.getChildren().size());

    for (const auto& child : node.getChildren()) {
        if (filter.find(child->getKind()) != filter.end()) {
            sorted.push_back(child.get());
        }
    }

    std::sort(sorted.begin(), sorted.end(), [](const Node* a, const Node* b) -> bool {
        return a->getName() < b->getName();
    });

    for (const auto& child : sorted) {
        auto data = jsonConverter.convert(*child);

        auto test = buildIndexRecursively(*child, filter);
        if (!test.empty()) {
            data["children"] = std::move(test);
        }

        json.push_back(std::move(data));
    }

    return json;
}

void Doxydown::Generator::dump(const std::string& path, const nlohmann::json& data) {
    if (!config.debugTemplateJson) return;

    std::ofstream file(Path::join(
        config.outputDir,
        path
    ));
    file << data.dump(2);
}

std::string Doxydown::Generator::indexToPath(const std::string& name) {
    const auto filename = config.indexFileName.empty() ? "index_" + name + ".md" : config.indexFileName + ".md";
    if (name == "classes") {
        return config.indexInFolders ? config.folderClassesName + "/" + filename : filename;
    }
    if (name == "namespaces") {
        return config.indexInFolders ? config.folderNamespacesName + "/" + filename : filename;
    }
    if (name == "groups") {
        return config.indexInFolders ? config.folderGroupsName + "/" + filename : filename;
    }
    if (name == "files") {
        return config.indexInFolders ? config.folderFilesName + "/" + filename : filename;
    }
    throw EXCEPTION("Index {} not recognised please contact the author", name);
}

std::string Doxydown::Generator::indexToTemplateName(const std::string& name) {
    if (name == "classes") {
        return config.templateIndexClasses;
    }
    if (name == "namespaces") {
        return config.templateIndexNamespaces;
    }
    if (name == "groups") {
        return config.templateIndexGroups;
    }
    if (name == "files") {
        return config.templateIndexFiles;
    }
    throw EXCEPTION("Index {} not recognised please contact the author", name);
}
