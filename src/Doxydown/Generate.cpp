#include "Generate.hpp"
#include "Doxygen.hpp"
#include "Renderer.hpp"
#include "Path.hpp"

static const std::string TEMPLATE_MEMBER =
R"({% include "header" %}

{% include "footer" %}
)";

static const std::string TEMPLATE_HEADER =
R"({{title(node.kind)}} {{node.name}}
=========================================
)";

static const std::string TEMPLATE_FOOTER =
R"(Created at {{date("%Y %B %e")}}
)";

static void generateMarkdownMemberRecursive(Doxydown::Renderer& renderer,
                                            const Doxydown::Node& parent,
                                            const std::string& outputDir) {
    using namespace Doxydown;

    for (const auto& child : parent.getChildren()) {
        const auto path = Path::join(outputDir, child->getRefid() + ".md");
        nlohmann::json data;
        data["node"] = JsonConverter::convert(*child);
        renderer.render("member", path, data);
    }
}

void Doxydown::generateMarkdown(Doxygen& doxygen, const std::string& outputDir) {
    Renderer renderer;
    renderer.addTemplate("header", TEMPLATE_HEADER);
    renderer.addTemplate("footer", TEMPLATE_FOOTER);
    renderer.addTemplate("member", TEMPLATE_MEMBER);

    generateMarkdownMemberRecursive(renderer, doxygen.getIndex(), outputDir);
}

void Doxydown::generateJson(Doxygen& doxygen, const std::string& outputDir) {

}

