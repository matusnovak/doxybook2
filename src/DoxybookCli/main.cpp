#include <string>
#include <cassert>
#include <iostream>
#include <argagg/argagg.hpp>
#include <Doxybook/Generator.hpp>
#include <Doxybook/TextMarkdownPrinter.hpp>
#include <Doxybook/TextPlainPrinter.hpp>
#include <Doxybook/TemplateDefaultLoader.hpp>
#include <Doxybook/TemplateFolderLoader.hpp>
#include <Doxybook/Doxygen.hpp>
#include <Doxybook/Log.hpp>
#include <Doxybook/Path.hpp>
#include <Doxybook/Utils.hpp>

static argagg::parser argparser {{
    {
        "help", {"-h", "--help"},
        "Shows this help message", 0
    },
    {
        "input", {"-i", "--input"},
        "Path to the generated Doxygen XML folder. Must contain index.xml!", 1
    },
    {
        "output", {"-o", "--output"},
        "Path to the target folder where to generate markdown files", 1
    },
    {
        "json", {"-j", "--json"},
        "Generate JSON only, no markdown, into the output path. This will also generate index.json.", 0
    },
    {
        "config", {"-c", "--config"},
        "Optional path to a config json file.", 1
    },
    {
        "config-data", {"--config-data"},
        "Optional json data to override config.", 1
    },
    {
        "templates", {"-t", "--templates"},
        "Optional path to a folder with templates.", 1
    },
    {
        "generate-config", {"--generate-config"},
        "Generate config file given a path to the destination json file", 1
    },
    {
        "generate-templates", {"--generate-templates"},
        "Generate template files given a path to a target folder.", 1
    },
    {
        "debug-templates", {"-d", "--debug-templates"},
        "Debug templates. This will create JSON for each generated template.", 0
    },
    {
        "summary-input", {"--summary-input"},
        "Path to the summary input file. This file must contain \"{{doxygen}}\" string.", 1
    },
    {
        "summary-output", {"--summary-output"},
        "Where to generate summary file. This file will be created. Not a directory!", 1
    }
}};

using namespace Doxybook2;

static const Generator::Filter INDEX_CLASS_FILTER = {
    Kind::NAMESPACE,
    Kind::CLASS,
    Kind::INTERFACE,
    Kind::STRUCT,
    Kind::UNION
};

static const Generator::Filter INDEX_CLASS_FILTER_SKIP = {
    Kind::NAMESPACE
};

static const Generator::Filter INDEX_NAMESPACES_FILTER = {
    Kind::NAMESPACE
};

static const Generator::Filter INDEX_MODULES_FILTER = {
    Kind::MODULE
};

static const Generator::Filter INDEX_FILES_FILTER = {
    Kind::DIR,
    Kind::FILE
};

static const Generator::Filter LANGUAGE_FILTER = {
    Kind::NAMESPACE,
    Kind::CLASS,
    Kind::INTERFACE,
    Kind::STRUCT,
    Kind::UNION,
    Kind::MODULE
};

static const Generator::Filter INDEX_PAGES_FILTER = {
    Kind::PAGE
};

static const Generator::Filter INDEX_EXAMPLES_FILTER = {
    Kind::EXAMPLE
};

int main(const int argc, char* argv[]) {
    try {
        Config config;

        const auto args = argparser.parse(argc, argv);
        if (args["help"]) {
            std::cerr << argparser;
            return EXIT_SUCCESS;
        }

        if (args["generate-config"]) {
            saveConfig(config, args["generate-config"].as<std::string>());
            return EXIT_SUCCESS;
        }

        if (args["generate-templates"]) {
            saveConfig(config, args["generate-config"].as<std::string>());
            return EXIT_SUCCESS;
        }

        if (args["output"]) {
            if (!args["input"]) {
                std::cerr << "You need to provide input path!" << std::endl;
                std::cerr << argparser;
                return EXIT_FAILURE;
            }

            if (args["config"]) {
                loadConfig(config, args["config"].as<std::string>());
            }

            if (args["config-data"]) {
                loadConfigData(config, args["config-data"].as<std::string>());
            }

            if (args["debug-templates"]) {
                config.debugTemplateJson = true;
            }

            if (args["json"]) {
                config.useFolders = false;
                config.imagesFolder = "";
            }

            config.outputDir = args["output"].as<std::string>();

            Doxygen doxygen(config);
            TextMarkdownPrinter markdownPrinter(config, args["input"].as<std::string>(), doxygen);
            TextPlainPrinter plainPrinter(config, doxygen);
            JsonConverter jsonConverter(config, doxygen, plainPrinter, markdownPrinter);
            std::unique_ptr<TemplateLoader> templateLoader = args["templates"]
                                                                 ? std::make_unique<TemplateFolderLoader>(
                                                                     args["templates"].as<std::string>())
                                                                 : std::make_unique<TemplateDefaultLoader>();
            Generator generator(config, jsonConverter, *templateLoader);

            if (config.useFolders) {
                static const std::array<Type, 6> ALL_GROUPS = {
                    Type::CLASSES,
                    Type::NAMESPACES,
                    Type::FILES,
                    Type::MODULES,
                    Type::PAGES,
                    Type::EXAMPLES
                };
                for (const auto& g : ALL_GROUPS) {
                    Utils::createDirectory(Path::join(config.outputDir, typeToFolderName(config, g)));
                }
                if (!config.imagesFolder.empty()) {
                    Utils::createDirectory(Path::join(config.outputDir, config.imagesFolder));
                }
            }

            Log::i("Loading...");
            doxygen.load(args["input"].as<std::string>());
            Log::i("Finalizing...");
            doxygen.finalize(plainPrinter, markdownPrinter);
            Log::i("Rendering...");

            if (args["json"]) {
                generator.json(doxygen, LANGUAGE_FILTER, {});
                generator.json(doxygen, INDEX_FILES_FILTER, {});
                generator.json(doxygen, INDEX_PAGES_FILTER, {});

                generator.manifest(doxygen);
            } else {
                if (args["summary-input"] && args["summary-output"]) {
                    generator.summary(
                        doxygen, 
                        args["summary-input"].as<std::string>(), 
                        args["summary-output"].as<std::string>(),
                        {
                            {FolderCategory::CLASSES, INDEX_CLASS_FILTER, INDEX_CLASS_FILTER_SKIP},
                            {FolderCategory::NAMESPACES, INDEX_NAMESPACES_FILTER, {}},
                            {FolderCategory::MODULES, INDEX_MODULES_FILTER, {}},
                            {FolderCategory::FILES, INDEX_FILES_FILTER, {}},
                            {FolderCategory::PAGES, INDEX_PAGES_FILTER, {}},
                            {FolderCategory::EXAMPLES, INDEX_EXAMPLES_FILTER, {}}
                        }
                    );
                }

                generator.print(doxygen, LANGUAGE_FILTER, {});
                generator.print(doxygen, INDEX_FILES_FILTER, {});
                generator.print(doxygen, INDEX_PAGES_FILTER, {});
                generator.print(doxygen, INDEX_EXAMPLES_FILTER, {});

                generator.printIndex(doxygen, FolderCategory::CLASSES, INDEX_CLASS_FILTER, {});
                generator.printIndex(doxygen, FolderCategory::NAMESPACES, INDEX_NAMESPACES_FILTER, {});
                generator.printIndex(doxygen, FolderCategory::MODULES, INDEX_MODULES_FILTER, {});
                generator.printIndex(doxygen, FolderCategory::FILES, INDEX_FILES_FILTER, {});
                generator.printIndex(doxygen, FolderCategory::PAGES, INDEX_PAGES_FILTER, {});
                generator.printIndex(doxygen, FolderCategory::EXAMPLES, INDEX_EXAMPLES_FILTER, {});
            }
        } else {
            std::cerr << argparser;
            return EXIT_FAILURE;
        }

        return EXIT_SUCCESS;
    } catch (std::exception& e) {
        Log::e(e.what());
        return EXIT_FAILURE;
    }
}
