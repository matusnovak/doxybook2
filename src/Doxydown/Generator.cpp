#include <fstream>
#include <Doxydown/Generator.hpp>
#include <Doxydown/Doxygen.hpp>
#include <Doxydown/Renderer.hpp>
#include <Doxydown/Path.hpp>
#include <Doxydown/TemplateLoader.hpp>
#include <Doxydown/Exception.hpp>
#include <Doxydown/Utils.hpp>
#include "ExceptionUtils.hpp"


std::string Doxydown::Generator::kindToTemplateName(const Kind kind) {
    using namespace Doxydown;
    switch (kind) {
        case Kind::STRUCT:
            return config.templateKindStruct;
        case Kind::INTERFACE:
            return config.templateKindInterface;
        case Kind::UNION:
            return config.templateKindUnion;
        case Kind::CLASS:
            return config.templateKindClass;
        case Kind::NAMESPACE:
            return config.templateKindNamespace;
        case Kind::MODULE:
            return config.templateKindGroup;
        default: {
            throw EXCEPTION("Unrecognised kind {} please contant the author!", int(kind));
        }
    }
}

Doxydown::Generator::Generator(const Config& config, const JsonConverter& jsonConverter, const TemplateLoader& templateLoader)
    : config(config),
      jsonConverter(jsonConverter),
      renderer(config) {

    for (const auto& pair : templateLoader.getTemplates()) {
        renderer.addTemplate(pair.name, pair.contents);
    }
}

void Doxydown::Generator::printRecursively(const Node& parent) {
    using namespace Doxydown;

    for (const auto& child : parent.getChildren()) {
        if (child->isStructured() || child->getKind() == Kind::MODULE) {
            nlohmann::json data = jsonConverter.getAsJson(*child);

            const auto path = Path::join(
                typeToFolderName(config, child->getType()),
                child->getRefid() + "." + config.fileExt
            );

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
    static const std::array<Type, 4> ALL_GROUPS = {
        Type::CLASSES,
        Type::NAMESPACES,
        Type::DIRORFILE,
        Type::MODULES
    };
    for (const auto& g : ALL_GROUPS) {
        Utils::createDirectory(Path::join(config.outputDir, typeToFolderName(config, g)));
    }

    printRecursively(doxygen.getIndex());
}

void Doxydown::Generator::printIndex(const Doxygen& doxygen, const std::string& name, const Filter& filter) {
    const auto path = indexToPath(name) + "." + config.fileExt;

    nlohmann::json data;
    data["children"] = buildIndexRecursively(doxygen.getIndex(), filter);
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

std::string Doxydown::Generator::indexToPath(const std::string& name) {
    if (name == "classes") {
        return config.indexInFolders ? config.folderClassesName + "/" + config.indexClassesName : config.indexClassesName;
    }
    if (name == "namespaces") {
        return config.indexInFolders ? config.folderNamespacesName + "/" + config.indexNamespacesName : config.indexNamespacesName;
    }
    if (name == "groups") {
        return config.indexInFolders ? config.folderGroupsName + "/" + config.indexGroupsName : config.indexGroupsName;
    }
    if (name == "files") {
        return config.indexInFolders ? config.folderFilesName + "/" + config.indexFilesName : config.indexFilesName;
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
