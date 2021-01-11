#include <Doxybook/DefaultTemplates.hpp>
#include <Doxybook/Doxygen.hpp>
#include <Doxybook/Generator.hpp>
#include <Doxybook/Log.hpp>
#include <Doxybook/Path.hpp>
#include <Doxybook/TextMarkdownPrinter.hpp>
#include <Doxybook/TextPlainPrinter.hpp>
#include <Doxybook/Utils.hpp>
#include <argagg/argagg.hpp>
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

static argagg::parser argparser{{{"help", {"-h", "--help"}, "Shows this help message.", 0},
    {"version", {"-v", "--version"}, "Shows the version.", 0},
    {"quiet", {"-q", "--quiet"}, "Run in quiet mode, no stdout, display only errors and warnings to stderr.", 0},
    {"input", {"-i", "--input"}, "Path to the generated Doxygen XML folder. Must contain index.xml!", 1},
    {"output", {"-o", "--output"}, "Path to the target folder where to generate markdown files.", 1},
    {"json",
        {"-j", "--json"},
        "Generate JSON only, no markdown, into the output path. This will also generate index.json.",
        0},
    {"config", {"-c", "--config"}, "Optional path to a config json file.", 1},
    {"config-data", {"--config-data"}, "Optional json data to override config.", 1},
    {"templates", {"-t", "--templates"}, "Optional path to a folder with templates.", 1},
    {"generate-config", {"--generate-config"}, "Generate config file given a path to the destination json file", 1},
    {"generate-templates", {"--generate-templates"}, "Generate template files given a path to a target folder.", 1},
    {"debug-templates",
        {"-d", "--debug-templates"},
        "Debug templates. This will create JSON for each generated template.",
        0},
    {"summary-input",
        {"--summary-input"},
        "Path to the summary input file. This file must contain \"{{doxygen}}\" string.",
        1},
    {"summary-output",
        {"--summary-output"},
        "Where to generate summary file. This file will be created. Not a directory!",
        1}}};

using namespace Doxybook2;

static const std::string example = "Example usage:\n"
                                   "    doxybook2 --generate-config doxybook.json\n"
                                   "    doxybook2 -i ./doxygen/xml -o ./docs/content -c doxybook.json\n"
                                   "\n";

static const Generator::Filter INDEX_CLASS_FILTER = {Kind::NAMESPACE,
    Kind::CLASS,
    Kind::INTERFACE,
    Kind::STRUCT,
    Kind::UNION};

static const Generator::Filter INDEX_CLASS_FILTER_SKIP = {Kind::NAMESPACE};

static const Generator::Filter INDEX_NAMESPACES_FILTER = {Kind::NAMESPACE};

static const Generator::Filter INDEX_MODULES_FILTER = {Kind::MODULE};

static const Generator::Filter INDEX_FILES_FILTER = {Kind::DIR, Kind::FILE};

static const Generator::Filter LANGUAGE_FILTER =
    {Kind::NAMESPACE, Kind::CLASS, Kind::INTERFACE, Kind::STRUCT, Kind::UNION, Kind::MODULE};

static const Generator::Filter INDEX_PAGES_FILTER = {Kind::PAGE};

static const Generator::Filter INDEX_EXAMPLES_FILTER = {Kind::EXAMPLE};

static void help() {
    std::cerr << example;
    std::cerr << "Options:\n";
    std::cerr << argparser;
}

int main(const int argc, char* argv[]) {
    try {
        Config config;

        const auto args = argparser.parse(argc, argv);

        if (args["quiet"]) {
            Log::setQuietMode(true);
        }

        if (args["help"]) {
            help();
            return EXIT_SUCCESS;
        }

        else if (args["version"]) {
            std::cerr << version;
            return EXIT_SUCCESS;
        }

        else if (args["generate-config"]) {
            saveConfig(config, args["generate-config"].as<std::string>());
            return EXIT_SUCCESS;
        }

        else if (args["generate-templates"]) {
            saveDefaultTemplates(args["generate-templates"].as<std::string>());
            return EXIT_SUCCESS;
        }

        else if (args["output"]) {
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

            std::optional<std::string> templatesPath;
            if (args["templates"]) {
                templatesPath = args["templates"].as<std::string>();
            }

            Generator generator(config, jsonConverter, templatesPath);

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

                    generator.summary(doxygen,
                        args["summary-input"].as<std::string>(),
                        args["summary-output"].as<std::string>(),
                        sections);
                }

                Generator::Filter languageFilder;
                if (shouldGenerate(FolderCategory::CLASSES)) {
                    languageFilder.insert(Kind::CLASS);
                    languageFilder.insert(Kind::STRUCT);
                    languageFilder.insert(Kind::UNION);
                    languageFilder.insert(Kind::INTERFACE);
                }
                if (shouldGenerate(FolderCategory::NAMESPACES)) {
                    languageFilder.insert(Kind::NAMESPACE);
                }
                if (shouldGenerate(FolderCategory::MODULES)) {
                    languageFilder.insert(Kind::MODULE);
                }
                if (!languageFilder.empty()) {
                    generator.print(doxygen, languageFilder, {});
                }

                if (shouldGenerate(FolderCategory::FILES)) {
                    generator.print(doxygen, INDEX_FILES_FILTER, {});
                }
                if (shouldGenerate(FolderCategory::PAGES)) {
                    generator.print(doxygen, INDEX_PAGES_FILTER, {});
                }
                if (shouldGenerate(FolderCategory::EXAMPLES)) {
                    generator.print(doxygen, INDEX_EXAMPLES_FILTER, {});
                }

                if (shouldGenerate(FolderCategory::CLASSES)) {
                    generator.printIndex(doxygen, FolderCategory::CLASSES, INDEX_CLASS_FILTER, {});
                }
                if (shouldGenerate(FolderCategory::NAMESPACES)) {
                    generator.printIndex(doxygen, FolderCategory::NAMESPACES, INDEX_NAMESPACES_FILTER, {});
                }
                if (shouldGenerate(FolderCategory::MODULES)) {
                    generator.printIndex(doxygen, FolderCategory::MODULES, INDEX_MODULES_FILTER, {});
                }
                if (shouldGenerate(FolderCategory::FILES)) {
                    generator.printIndex(doxygen, FolderCategory::FILES, INDEX_FILES_FILTER, {});
                }
                if (shouldGenerate(FolderCategory::PAGES)) {
                    generator.printIndex(doxygen, FolderCategory::PAGES, INDEX_PAGES_FILTER, {});
                }
                if (shouldGenerate(FolderCategory::EXAMPLES)) {
                    generator.printIndex(doxygen, FolderCategory::EXAMPLES, INDEX_EXAMPLES_FILTER, {});
                }
            }
        } else {
            help();
            return EXIT_FAILURE;
        }

        return EXIT_SUCCESS;
    } catch (std::exception& e) {
        Log::e(e.what());
        return EXIT_FAILURE;
    }
}
