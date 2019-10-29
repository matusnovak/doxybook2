#include <string>
#include <argagg/argagg.hpp>
#include "Log.hpp"
#include "Generator.hpp"

static std::vector<argagg::definition> defaultArgs{
    {
        "help", {"-h", "--help"},
        "Shows this help message", 0
    },
    {
        "input", {"--input"},
        "REQUIRED! Path to the generated Doxygen XML folder. Must contain index.xml!", 1
    }
};

using namespace Doxydown;

class ConfigArg {
public:
    virtual argagg::definition createDefinition() const = 0;
    virtual void load(const argagg::parser_results& results) const = 0;
};

template<typename T>
class ConfigArgTyped;

template<>
class ConfigArgTyped<std::string>: public ConfigArg {
public:
    explicit ConfigArgTyped(std::string& ref, std::string key, std::string help, const bool optional)
        : ref(ref),
          key(std::move(key)),
          help(std::move(help)),
          defval(ref),
          optional(optional) {
    }

    argagg::definition createDefinition() const override {
        assert(!key.empty());

        return {
            key, {"--" + key},
            !optional ? "REQUIRED! " + help : "OPTIONAL! " + help + (!optional ? "" : " (default: \"" + defval + "\")"),
            1
        };
    }

    void load(const argagg::parser_results& results) const override {
        assert(!key.empty());

        if (optional) ref = results[key].as<std::string>(defval);
        else {
            if (!optional && !results[key]) {
                Log::e("You need to provide --{}!", key);
            }
            ref = results[key].as<std::string>();
        }
    }

    std::string& ref;
    std::string key;
    std::string help;
    std::string defval;
    bool optional;
};

template<>
class ConfigArgTyped<bool> : public ConfigArg {
public:
    explicit ConfigArgTyped(bool& ref, std::string key, std::string help)
        : ref(ref),
          key(std::move(key)),
          help(std::move(help)),
          defval(ref) {
    }

    argagg::definition createDefinition() const override {
        return {
            key, {"--" + key},
            "OPTIONAL! " + help + " (default: " + (defval ? "true" : "false") + ")",
            1
        };
    }

    void load(const argagg::parser_results& results) const override {
        ref = results[key].as<bool>(defval);
    }

    bool& ref;
    std::string key;
    std::string help;
    bool defval;
};

static const std::unordered_set<Node::Kind> INDEX_CLASS_FILTER = {
    Node::Kind::NAMESPACE,
    Node::Kind::CLASS,
    Node::Kind::STRUCT,
    Node::Kind::UNION
};

static const std::unordered_set<Node::Kind> INDEX_NAMESPACES_FILTER = {
    Node::Kind::NAMESPACE
};

static const std::unordered_set<Node::Kind> INDEX_MODULES_FILTER = {
    Node::Kind::MODULE
};

int main(const int argc, char* argv[]) {
    std::vector<ConfigArg*> configArgs;
    try {
        Config config;

        configArgs = {
            new ConfigArgTyped<std::string>(config.outputDir, 
                "output", 
                "Where to store the generated files, the folder must exist!", 
                false),
            new ConfigArgTyped<std::string>(config.baseUrl,
                "base-url", 
                "What is the base url to append all links with?",
                true),
            new ConfigArgTyped<std::string>(config.fileExt,
                "file-ext", 
                "The file extension to use for the generated files", 
                true),
            new ConfigArgTyped<std::string>(config.linkSuffix,
                "link-suffix", 
                "What suffix to append all markdown links with? "
                "If you are using Hugo with Learn template, you might want to use / instead of .md", 
                true),
            new ConfigArgTyped<bool>(config.indexInFolders, 
                "index-in-folders", 
                "Should the index files be generated in the "
                "destination ""folder or in their respective folders? For example in Classes/index.md ?"),
            new ConfigArgTyped<std::string>(config.indexFileName,
                "index-filename", 
                "What should the index files be named? "
                "When using Hugo with Learn template this might be needed and set to _index.md "
                "with index-in-folders=true", 
                true),
            new ConfigArgTyped<std::string>(config.folderClassesName,
                "folder-classes", 
                "Name of the folder to create for classes",
                true),
            new ConfigArgTyped<std::string>(config.folderNamespacesName,
                "folder-namespaces", 
                "Name of the folder to create for namespaces", 
                true),
            new ConfigArgTyped<std::string>(config.folderGroupsName,
                "folder-groups", 
                "Name of the folder to create for groups (alias modules)", 
                true),
            new ConfigArgTyped<std::string>(config.folderFilesName,
                "folder-files", 
                "Name of the folder to create for files", 
                true),
            new ConfigArgTyped<std::string>(config.folderRelatedPagesName,
                "folder-related", 
                "Name of the folder to create for related pages", 
                true),
            new ConfigArgTyped<std::string>(config.templateKindClass,
                "template-kind-class",
                "Name of the template to use for classes", 
                true),
            new ConfigArgTyped<std::string>(config.templateKindNamespace,
                "template-kind-namespace", 
                "Name of the template to use for namespaces", 
                true),
            new ConfigArgTyped<std::string>(config.templateKindInterface,
                "template-kind-interface", 
                "Name of the template to use for interfaces", 
                true),
            new ConfigArgTyped<std::string>(config.templateKindStruct,
                "template-kind-struct", 
                "Name of the template to use for structs", 
                true),
            new ConfigArgTyped<std::string>(config.templateKindGroup,
                "template-kind-group", 
                "Name of the template to use for group (alias modules)", 
                true),
            new ConfigArgTyped<std::string>(config.templateKindDir,
                "template-kind-dir", 
                "Name of the template to use for directories", 
                true),
            new ConfigArgTyped<std::string>(config.templateKindFile,
                "template-kind-file", 
                "Name of the template to use for files", 
                true),
            new ConfigArgTyped<std::string>(config.templateKindUnion,
                "template-kind-union", 
                "Name of the template to use for unions",
                true),
            new ConfigArgTyped<std::string>(config.templateIndexClasses,
                "template-index-classes", 
                "Name of the template to use for class index", 
                true),
            new ConfigArgTyped<std::string>(config.templateIndexNamespaces,
                "template-index-namespaces", 
                "Name of the template to use for namespace index",
                true),
            new ConfigArgTyped<std::string>(config.templateIndexGroups,
                "template-index-groups", 
                "Name of the template to use for groups (alias modules) index", 
                true),
            new ConfigArgTyped<std::string>(config.templateIndexFiles, 
                "template-index-files", 
                "Name of the template to use for files index",
                true),
        };

        argagg::parser argparser;
        for (const auto& a : defaultArgs) argparser.definitions.push_back(a);
        for (const auto& a : configArgs) argparser.definitions.push_back(a->createDefinition());
        
        const auto args = argparser.parse(argc, argv);
        if (args["help"]) {
            std::cerr << argparser;
            return EXIT_SUCCESS;
        }

        if (!args["input"]) {
            Log::e("You need to provide input path!");
            std::cerr << argparser;
        }

        for (const auto& a : configArgs) a->load(args);

        if (args["output"]) {
            TextPrinter printer(config);
            JsonConverter jsonConverter(config, printer);
            Generator generator(config, jsonConverter);
            Doxygen doxygen(args["input"].as<std::string>());

            doxygen.load();
            printer.setCache(&doxygen.getCache());
            doxygen.finalize(config, printer);

            generator.print(doxygen);
            generator.printIndex(doxygen, "classes", INDEX_CLASS_FILTER);
            generator.printIndex(doxygen, "namespaces", INDEX_NAMESPACES_FILTER);
            generator.printIndex(doxygen, "groups", INDEX_MODULES_FILTER);
        }

        for (auto& c : configArgs) delete c;
        return EXIT_SUCCESS;
    } catch (std::exception& e) {
        Log::e("{}", e.what());

        for (auto& c : configArgs) delete c;
        return EXIT_FAILURE;
    }
}
