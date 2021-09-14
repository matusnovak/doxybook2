#include <Doxybook/DefaultTemplates.hpp>
#include <Doxybook/Doxygen.hpp>
#include <Doxybook/Generator.hpp>
#include <spdlog/spdlog.h>
#include <Doxybook/Path.hpp>
#include <Doxybook/TextMarkdownPrinter.hpp>
#include <Doxybook/TextPlainPrinter.hpp>
#include <Doxybook/Utils.hpp>
#include <cxxopts.hpp>
#include <cassert>
#include <iostream>
#include <string>

#define xstr(a) str(a)
#define str(a) #a

#ifdef VERSION
static const std::string version = xstr(VERSION);
#else
static const std::string version = "unknown";
#endif
 
using namespace Doxybook2;

static const Generator::Filter INDEX_CLASS_FILTER = {Kind::NAMESPACE,
    Kind::CLASS,
    Kind::INTERFACE,
    Kind::STRUCT,
    Kind::UNION,
    Kind::JAVAENUM};

static const Generator::Filter INDEX_CLASS_FILTER_SKIP = {Kind::NAMESPACE};

static const Generator::Filter INDEX_NAMESPACES_FILTER = {Kind::NAMESPACE};

static const Generator::Filter INDEX_MODULES_FILTER = {Kind::MODULE};

static const Generator::Filter INDEX_FILES_FILTER = {Kind::DIR, Kind::FILE};

static const Generator::Filter LANGUAGE_FILTER =
    {Kind::NAMESPACE, Kind::CLASS, Kind::INTERFACE, Kind::STRUCT, Kind::UNION, Kind::MODULE, Kind::JAVAENUM};

static const Generator::Filter INDEX_PAGES_FILTER = {Kind::PAGE};

static const Generator::Filter INDEX_EXAMPLES_FILTER = {Kind::EXAMPLE};

int main(int argc, char* argv[]) {

    spdlog::set_pattern("%^[%l]%$ %v");

    cxxopts::Options options("Doxybook", "Doxygen XML to Markdown (or JSON)");

    options.add_options()
    ("h, help", "Shows this help message.")
    ("v, version", "Shows the version.")
    ("q, quiet", "Run in quiet mode, no stdout, display only errors and warnings to stderr.", cxxopts::value<bool>()->default_value("false"))
    ("i, input", "Path to the generated Doxygen XML folder. Must contain index.xml!", cxxopts::value<std::string>())
    ("o, output", "Path to the target folder where to generate markdown files.", cxxopts::value<std::string>())
    ("j, json", "Generate JSON only, no markdown, into the output path. This will also generate index.json.")
    ("c, config", "Optional path to a config json file.", cxxopts::value<std::string>())
    ("config-data", "Optional json data to override config.", cxxopts::value<std::string>())
    ("t, templates", "Optional path to a folder with templates.", cxxopts::value<std::string>())
    ("generate-config", "Generate config file given a path to the destination json file", cxxopts::value<std::string>())
    ("generate-templates", "Generate template files given a path to a target folder.", cxxopts::value<std::string>())
    ("d, debug-templates", "Debug templates. This will create JSON for each generated template.")
    ("summary-input", "Path to the summary input file. This file must contain \"{{doxygen}}\" string.", cxxopts::value<std::string>())
    ("summary-output", "Where to generate summary file. This file will be created. Not a directory!", cxxopts::value<std::string>())
    ("example", "Example usage:\n"
                                   "    doxybook2 --generate-config doxybook.json\n"
                                   "    doxybook2 -i ./doxygen/xml -o ./docs/content -c doxybook.json\n"
                                   "\n")
    ;



    try {
        Config config;

        auto args = options.parse(argc, argv);

        if (args["quiet"].as<bool>()) {
            spdlog::set_level(spdlog::level::off);
        }

        if (args["help"].as<bool>()) {
            std::cerr << options.help() << std::endl;
            return EXIT_SUCCESS;
        }

        else if (args["version"].as<bool>()) {
            std::cerr << version;
            return EXIT_SUCCESS;
        }

        else if (args.count("generate-config")) {
            saveConfig(config, args["generate-config"].as<std::string>());
            return EXIT_SUCCESS;
        }

        else if (args.count("generate-templates")) {
            saveDefaultTemplates(args["generate-templates"].as<std::string>());
            return EXIT_SUCCESS;
        }

        else if (args.count("output")) {
            if (!args.count("input")) {
                std::cerr << "You need to provide input path!" << std::endl;
                std::cerr << options.help();
                return EXIT_FAILURE;
            }

            if (args.count("config")) {
                loadConfig(config, args["config"].as<std::string>());
            }

            if (args.count("config-data")) {
                loadConfigData(config, args["config-data"].as<std::string>());
            }

            if (args.count("debug-templates")) {
                config.debugTemplateJson = true;
            }

            if (args.count("json")) {
                config.useFolders = false;
                config.imagesFolder = "";
            }

            config.outputDir = args["output"].as<std::string>();

            Doxygen doxygen(config);
            TextMarkdownPrinter markdownPrinter(config, args["input"].as<std::string>(), doxygen);
            TextPlainPrinter plainPrinter(config, doxygen);
            JsonConverter jsonConverter(config, doxygen, plainPrinter, markdownPrinter);

            std::optional<std::string> templatesPath;
            if (args.count("templates")) {
                templatesPath = args["templates"].as<std::string>();
            }

            Generator generator(config, doxygen, jsonConverter, templatesPath);

            const auto shouldGenerate = [&](const FolderCategory category) {
                return std::find(config.foldersToGenerate.begin(), config.foldersToGenerate.end(), category) !=
                       config.foldersToGenerate.end();
            };

            if (config.useFolders) {
                static const std::array<FolderCategory, 6> ALL_GROUPS = {
                    FolderCategory::CLASSES,
                    FolderCategory::NAMESPACES,
                    FolderCategory::FILES,
                    FolderCategory::MODULES,
                    FolderCategory::PAGES,
                    FolderCategory::EXAMPLES,
                };
                for (const auto& g : ALL_GROUPS) {
                    if (shouldGenerate(g)) {
                        Utils::createDirectory(Path::join(config.outputDir, typeFolderCategoryToFolderName(config, g)));
                    }
                }
                if (!config.imagesFolder.empty()) {
                    Utils::createDirectory(Path::join(config.outputDir, config.imagesFolder));
                }
            }

            spdlog::info("Loading...");
            doxygen.load(args["input"].as<std::string>());
            spdlog::info("Finalizing...");
            doxygen.finalize(plainPrinter, markdownPrinter);
            spdlog::info("Rendering...");

            if (args.count("json")) {
                generator.json(LANGUAGE_FILTER, {});
                generator.json(INDEX_FILES_FILTER, {});
                generator.json(INDEX_PAGES_FILTER, {});

                generator.manifest();
            } else {
                if (args.count("summary-input") && args.count("summary-output")) {
                    std::vector<Generator::SummarySection> sections;
                    if (shouldGenerate(FolderCategory::CLASSES)) {
                        sections.push_back({FolderCategory::CLASSES, INDEX_CLASS_FILTER, INDEX_CLASS_FILTER_SKIP});
                    }
                    if (shouldGenerate(FolderCategory::NAMESPACES)) {
                        sections.push_back({FolderCategory::NAMESPACES, INDEX_NAMESPACES_FILTER, {}});
                    }
                    if (shouldGenerate(FolderCategory::MODULES)) {
                        sections.push_back({FolderCategory::MODULES, INDEX_MODULES_FILTER, {}});
                    }
                    if (shouldGenerate(FolderCategory::FILES)) {
                        sections.push_back({FolderCategory::FILES, INDEX_FILES_FILTER, {}});
                    }
                    if (shouldGenerate(FolderCategory::PAGES)) {
                        sections.push_back({FolderCategory::PAGES, INDEX_PAGES_FILTER, {}});
                    }
                    if (shouldGenerate(FolderCategory::EXAMPLES)) {
                        sections.push_back({FolderCategory::EXAMPLES, INDEX_EXAMPLES_FILTER, {}});
                    }

                    generator.summary(args["summary-input"].as<std::string>(),
                        args["summary-output"].as<std::string>(),
                        sections);
                }

                Generator::Filter languageFilder;
                if (shouldGenerate(FolderCategory::CLASSES)) {
                    languageFilder.insert(Kind::CLASS);
                    languageFilder.insert(Kind::STRUCT);
                    languageFilder.insert(Kind::UNION);
                    languageFilder.insert(Kind::INTERFACE);
                    languageFilder.insert(Kind::JAVAENUM);
                }
                if (shouldGenerate(FolderCategory::NAMESPACES)) {
                    languageFilder.insert(Kind::NAMESPACE);
                }
                if (shouldGenerate(FolderCategory::MODULES)) {
                    languageFilder.insert(Kind::MODULE);
                }
                if (!languageFilder.empty()) {
                    generator.print(languageFilder, {});
                }

                if (shouldGenerate(FolderCategory::FILES)) {
                    generator.print(INDEX_FILES_FILTER, {});
                }
                if (shouldGenerate(FolderCategory::PAGES)) {
                    generator.print(INDEX_PAGES_FILTER, {});
                }
                if (shouldGenerate(FolderCategory::EXAMPLES)) {
                    generator.print(INDEX_EXAMPLES_FILTER, {});
                }

                if (shouldGenerate(FolderCategory::CLASSES)) {
                    generator.printIndex(FolderCategory::CLASSES, INDEX_CLASS_FILTER, {});
                }
                if (shouldGenerate(FolderCategory::NAMESPACES)) {
                    generator.printIndex(FolderCategory::NAMESPACES, INDEX_NAMESPACES_FILTER, {});
                }
                if (shouldGenerate(FolderCategory::MODULES)) {
                    generator.printIndex(FolderCategory::MODULES, INDEX_MODULES_FILTER, {});
                }
                if (shouldGenerate(FolderCategory::FILES)) {
                    generator.printIndex(FolderCategory::FILES, INDEX_FILES_FILTER, {});
                }
                if (shouldGenerate(FolderCategory::PAGES)) {
                    generator.printIndex(FolderCategory::PAGES, INDEX_PAGES_FILTER, {});
                }
                if (shouldGenerate(FolderCategory::EXAMPLES)) {
                    generator.printIndex(FolderCategory::EXAMPLES, INDEX_EXAMPLES_FILTER, {});
                }
            }
        } else {
            std::cerr << options.help() << std::endl;
            return EXIT_FAILURE;
        }

        return EXIT_SUCCESS;
    } catch (std::exception& e) {
        spdlog::error(e.what());
        return EXIT_FAILURE;
    }
}
