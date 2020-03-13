#pragma once
#include <string>
#include <unordered_set>
#include "Renderer.hpp"
#include "JsonConverter.hpp"
#include "TemplateLoader.hpp"

namespace Doxybook2 {

    // Declaration of the Generator class
    class Generator {
    public:

        // The "Filter" Type is a std::unordered_set<Kind>
        typedef std::unordered_set<Kind> Filter;

        // Declaration of the SummarySection struct
        struct SummarySection {
            FolderCategory type;
            Filter filter;
            Filter skip;
        };

        // An explicit Generator declaration; requires that the compiler not change the types due to a compiler-based assumption of human error
        explicit Generator(const Config& config,
                           const JsonConverter& jsonConverter,
                           const TemplateLoader& templateLoader);

        // 
        void print(const Doxygen& doxygen, const Filter& filter, const Filter& skip);
        void json(const Doxygen& doxygen, const Filter& filter, const Filter& skip);
        void manifest(const Doxygen& doxygen);
        void printIndex(const Doxygen& doxygen, FolderCategory type, const Filter& filter, const Filter& skip);
        void summary(const Doxygen& doxygen,
                     const std::string& inputFile,
                     const std::string& outputFile,
                     const std::vector<SummarySection>& sections);
    private:
        void printRecursively(const Node& parent, const Filter& filter, const Filter& skip);
        nlohmann::json manifestRecursively(const Node& node);
        void jsonRecursively(const Node& parent, const Filter& filter, const Filter& skip);
        std::string kindToTemplateName(Kind kind);
        nlohmann::json buildIndexRecursively(const Node& node, const Filter& filter, const Filter& skip);
        void summaryRecursive(std::stringstream& ss,
                              int indent,
                              const std::string& folderName,
                              const Node& node,
                              const Filter& filter,
                              const Filter& skip);

        const Config& config;
        const JsonConverter& jsonConverter;
        Renderer renderer;
    };
}
