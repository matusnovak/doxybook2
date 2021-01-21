#include <Doxybook/Config.hpp>
#include <Doxybook/Doxygen.hpp>
#include <Doxybook/Log.hpp>
#include <Doxybook/Utils.hpp>
#include <argagg/argagg.hpp>
#include <cassert>
#include <functional>
#include <iostream>
#include <string>

#define xstr(a) str(a)
#define str(a) #a

#ifdef VERSION
static const std::string VERSION_STR = xstr(VERSION);
#else
static const std::string VERSION_STR = "unknown";
#endif

// clang-format off
static argagg::parser argParser{{
    {
        "help",
        {"-h", "--help"},
        "Shows this help message.",
        0
    },
    {
        "version",
        {"-v", "--version"},
        "Shows the version.",
        0
    },
    {
        "quiet",
        {"-q", "--quiet"},
        "Run in quiet mode, no stdout, display only errors and warnings to stderr.",
        0
    },
    {
        "input",
        {"-i", "--input"},
        "Path to the generated Doxygen XML folder. Must contain index.xml!",
        1
    },
    {
        "output",
        {"-o", "--output"},
        "Path to the target folder where to generate markdown files.",
        1
    },
    {
        "json",
        {"-j", "--json"},
        "Generate JSON only, no markdown, into the output path. This will also generate index.json.",
        0
    },
    {
        "config",
        {"-c", "--config"},
        "Optional path to a config json file.",
        1
    },
    {
        "config-data",
        {"--config-data"}, "Optional json data to override config.",
        1
    },
    {
        "templates",
        {"-t", "--templates"}, "Optional path to a folder with templates.",
        1
    },
    {
        "generate-config",
        {"--generate-config"}, "Generate config file given a path to the destination json file",
        1
    },
    {
        "generate-templates",
        {"--generate-templates"}, "Generate template files given a path to a target folder.",
        1
    },
    {
        "debug-templates",
        {"-d", "--debug-templates"},
        "Debug templates. This will create JSON for each generated template.",
        0
    },
    {
        "summary-input",
        {"--summary-input"},
        "Path to the summary input file. This file must contain \"{{doxygen}}\" string.",
        1
    },
    {
        "summary-output",
        {"--summary-output"},
        "Where to generate summary file. This file will be created. Not a directory!",
        1
    }
}};
// clang-format on

using namespace Doxybook2;

struct State {
    Config config;
    std::filesystem::path inputDir;
    std::filesystem::path outputDir;

    NodeSharedPtr root;
};

struct Command {
    std::string name;
    std::function<void(State&, const argagg::option_results&)> callback;
};

void commandVersion(State& state, const argagg::option_results& arg) {
    (void)state;
    (void)arg;

    std::cerr << VERSION_STR;
}

void commandHelp(State& state, const argagg::option_results& arg) {
    (void)state;
    (void)arg;

    static const std::string EXAMPLE = "Example usage:\n"
                                       "    doxybook2 --generate-config doxybook.json\n"
                                       "    doxybook2 -i ./doxygen/xml -o ./docs/content -c doxybook.json\n"
                                       "\n";

    std::cerr << EXAMPLE;
    std::cerr << "Options:\n";
    std::cerr << argParser;
}

void commandQuiet(State& state, const argagg::option_results& arg) {
    (void)state;
    (void)arg;

    Log::setQuietMode(true);
}

void commandConfig(State& state, const argagg::option_results& arg) {
    const auto path = std::filesystem::path(arg.as<std::string>());
    loadConfig(state.config, path);
}

void commandInputDir(State& state, const argagg::option_results& arg) {
    state.inputDir = std::filesystem::path(arg.as<std::string>());
    state.root = Doxygen::parseIndex(state.inputDir);
}

void commandOutputDir(State& state, const argagg::option_results& arg) {
    state.outputDir = std::filesystem::path(arg.as<std::string>());
}

// clang-format off
static const std::vector<Command> COMMANDS = {
    {"version", commandVersion},
    {"help", commandHelp},
    {"quiet", commandQuiet},
    {"config", commandConfig},
    {"input", commandInputDir},
    {"output", commandOutputDir}
};
// clang-format on

int main(const int argc, char* argv[]) {
    try {
        State state;

        const auto args = argParser.parse(argc, argv);

        for (const auto& cmd : COMMANDS) {
            const auto& arg = args[cmd.name];
            if (arg) {
                cmd.callback(state, arg);
            }
        }

        return EXIT_SUCCESS;
    } catch (const std::exception& e) {
        backtrace(e);
        return EXIT_FAILURE;
    }
}
