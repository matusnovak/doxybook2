#include <string>
#include <argagg/argagg.hpp>
#include "Log.hpp"
#include "Generate.hpp"

static argagg::parser argparser{
    {
        {
            "help", {"-h", "--help"},
            "shows this help message", 0
        },
        {
            "input", {"-i", "--input"},
            "Path to the generated Doxygen XML folder. Must contain index.xml!", 1
        },
        {
            "markdown", {"-m", "--markdown"},
            "Path to a folder where save markdown files. The files will be created, the folder must exist! Example: --markdown project/docs/api",
            1
        },
        {
            "json", {"-j", "--json"},
            "Path to your desired JSON file. This file will be created, the folder must exist! Example: --json /tmp/doxygen.json",
            1
        }
    }
};

int main(const int argc, char* argv[]) {
    try {
        const auto args = argparser.parse(argc, argv);
        if (args["help"]) {
            std::cerr << argparser;
            return EXIT_SUCCESS;
        }

        if (!args["input"]) {
            Doxydown::Log::e("You need to provide input path!");
            std::cerr << argparser;
        }

        Doxydown::Doxygen doxygen(args["input"].as<std::string>());

        if (args["markdown"]) {
            Doxydown::generateMarkdown(doxygen, args["markdown"].as<std::string>());
        }

        if (args["json"]) {
            Doxydown::generateJson(doxygen, args["json"].as<std::string>());
        }

        return EXIT_SUCCESS;
    } catch (std::exception& e) {
        Doxydown::Log::e("{}", e.what());
        return EXIT_FAILURE;
    }
}
