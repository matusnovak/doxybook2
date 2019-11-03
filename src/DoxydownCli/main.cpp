#include <string>
#include <cassert>
#include <iostream>
#include <argagg/argagg.hpp>
#include <Doxydown/Generator.hpp>
#include <Doxydown/TextMarkdownPrinter.hpp>
#include <Doxydown/TextPlainPrinter.hpp>
#include <Doxydown/TemplateDefaultLoader.hpp>
#include <Doxydown/TemplateFolderLoader.hpp>
#include <Doxydown/Doxygen.hpp>
#include <Doxydown/Log.hpp>
#include "Doxydown/Path.hpp"
#include "Doxydown/Utils.hpp"

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
        "config", {"-c", "--config"},
        "Optional path to a config json file.", 1
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
        "Generate template files given a path to folder (folder must exist).", 1
    },
    {
        "debug-templates", {"--debug-templates"},
        "Debug templates. This will create JSON for each generated template.", 0
    }
}};

using namespace Doxydown;

static const Generator::Filter INDEX_CLASS_FILTER = {
    Kind::NAMESPACE,
    Kind::CLASS,
    Kind::STRUCT,
    Kind::UNION
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

static const Generator::Filter INDEX_FILES_PAGES = {
    Kind::PAGE
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

            if (args["debug-templates"]) {
                config.debugTemplateJson = true;
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

            static const std::array<Type, 5> ALL_GROUPS = {
                Type::CLASSES,
                Type::NAMESPACES,
                Type::FILES,
                Type::MODULES,
                Type::PAGES
            };
            for (const auto& g : ALL_GROUPS) {
                Utils::createDirectory(Path::join(config.outputDir, typeToFolderName(config, g)));
            }
            Utils::createDirectory(Path::join(config.outputDir, config.imagesFolder));

            doxygen.load(args["input"].as<std::string>());
            doxygen.finalize(plainPrinter, markdownPrinter);

            generator.print(doxygen, LANGUAGE_FILTER);
            generator.print(doxygen, INDEX_FILES_FILTER);
            generator.print(doxygen, INDEX_FILES_PAGES);

            generator.printIndex(doxygen, Type::CLASSES, "Classes", INDEX_CLASS_FILTER);
            generator.printIndex(doxygen, Type::NAMESPACES, "Namespaces", INDEX_NAMESPACES_FILTER);
            generator.printIndex(doxygen, Type::MODULES, "Modules", INDEX_MODULES_FILTER);
            generator.printIndex(doxygen, Type::FILES, "Files", INDEX_FILES_FILTER);
            generator.printIndex(doxygen, Type::PAGES, "Pages", INDEX_FILES_PAGES);
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
