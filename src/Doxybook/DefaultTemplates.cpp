#include "ExceptionUtils.hpp"
#include <Doxybook/DefaultTemplates.hpp>
#include <Doxybook/Utils.hpp>
#include <fstream>
#include <spdlog/spdlog.h>

// clang-format off
static const std::vector<std::string> ALL_VISIBILITIES = {
    "public", "protected"
};
// clang-format on

static const std::string TEMPLATE_META = R"()";

static const std::string TEMPLATE_HEADER =
    R"(---
{% if exists("title") -%}
title: {{title}}
{% else if exists("name") -%}
title: {{name}}
{% endif -%}
{% if exists("summary") -%}
summary: {{summary}}
{% endif -%}
{% include "meta" %}
---

{% if exists("title") -%}
# {{title}}
{% else if exists("kind") and kind != "page" -%}
# {{name}} {{title(kind)}} Reference
{% endif %}
)";

static const std::string TEMPLATE_BREADCRUMBS = R"({% if exists("moduleBreadcrumbs") -%}
**Module:** {%- for module in moduleBreadcrumbs -%}
 **[{{module.title}}]({{module.url}})**{% if not loop.is_last %} **/** {% endif -%}
{% endfor %}

{% endif -%})";

static const std::string TEMPLATE_FOOTER =
    R"(-------------------------------

Updated on {{date("%F at %H:%M:%S %z")}})";

static const std::string TEMPLATE_DETAILS =
    R"({% if exists("brief") %}{{brief}}
{% endif -%}

{% if exists("paramList") %}
**Parameters**: 

{% for param in paramList %}  * **{{param.name}}** {{param.text}}
{% endfor %}
{% endif -%}

{% if exists("returnsList") %}
**Returns**: 

{% for param in returnsList %}  * **{{param.name}}** {{param.text}}
{% endfor %}
{% endif -%}

{% if exists("exceptionsList") %}
**Exceptions**: 

{% for param in exceptionsList %}  * **{{param.name}}** {{param.text}}
{% endfor %}
{% endif -%}

{% if exists("templateParamsList") %}
**Template Parameters**: 

{% for param in templateParamsList %}  * **{{param.name}}** {{param.text}}
{% endfor %}
{% endif -%}

{% if exists("deprecated") %}
**Deprecated**: 

{{deprecated}}
{% endif -%}

{% if exists("see") %}
**See**: {% if length(see) == 1 %}{{first(see)}}{% else %}

{% for item in see %}  * {{item}}
{% endfor %}{% endif %}
{% endif -%}

{% if exists("returns") %}
**Return**: {% if length(returns) == 1 %}{{first(returns)}}{% else %}

{% for item in returns %}  * {{item}}
{% endfor %}{% endif %}
{% endif -%}

{% if exists("authors") %}
**Author**: {% if length(authors) == 1 %}{{first(authors)}}{% else %}

{% for item in authors %}  * {{item}}
{% endfor %}{% endif %}
{% endif -%}

{% if exists("version") %}
**Version**: {% if length(version) == 1 %}{{first(version)}}{% else %}

{% for item in version %}  * {{item}}
{% endfor %}{% endif %}
{% endif -%}

{% if exists("since") %}
**Since**: {% if length(since) == 1 %}{{first(since)}}{% else %}

{% for item in since %}  * {{item}}
{% endfor %}{% endif %}
{% endif -%}

{% if exists("date") %}
**Date**: {% if length(date) == 1 %}{{first(date)}}{% else %}

{% for item in date %}  * {{item}}
{% endfor %}{% endif %}
{% endif -%}

{% if exists("note") %}
**Note**: {% if length(note) == 1 %}{{first(note)}}{% else %}

{% for item in note %}  * {{item}}
{% endfor %}{% endif %}
{% endif -%}

{% if exists("bugs") %}
**Bug**: {% if length(bugs) == 1 %}{{first(bugs)}}{% else %}

{% for item in bugs %}  * {{item}}
{% endfor %}{% endif %}
{% endif -%}

{% if exists("tests") %}
**Test**: {% if length(tests) == 1 %}{{first(tests)}}{% else %}

{% for item in tests %}  * {{item}}
{% endfor %}{% endif %}
{% endif -%}

{% if exists("todos") %}
**Todo**: {% if length(todos) == 1 %}{{first(todos)}}{% else %}

{% for item in todos %}  * {{item}}
{% endfor %}{% endif %}
{% endif -%}

{% if exists("warning") %}
**Warning**: {% if length(warning) == 1 %}{{first(warning)}}{% else %}

{% for item in warning %}  * {{item}}
{% endfor %}{% endif %}
{% endif -%}

{% if exists("pre") %}
**Precondition**: {% if length(pre) == 1 %}{{first(pre)}}{% else %}

{% for item in pre %}  * {{item}}
{% endfor %}{% endif %}
{% endif -%}

{% if exists("post") %}
**Postcondition**: {% if length(post) == 1 %}{{first(post)}}{% else %}

{% for item in post %}  * {{item}}
{% endfor %}{% endif %}
{% endif -%}

{% if exists("copyright") %}
**Copyright**: {% if length(copyright) == 1 %}{{first(copyright)}}{% else %}

{% for item in copyright %}  * {{item}}
{% endfor %}{% endif %}
{% endif -%}

{% if exists("invariant") %}
**Invariant**: {% if length(invariant) == 1 %}{{first(invariant)}}{% else %}

{% for item in invariant %}  * {{item}}
{% endfor %}{% endif %}
{% endif -%}

{% if exists("remark") %}
**Remark**: {% if length(remark) == 1 %}{{first(remark)}}{% else %}

{% for item in remark %}  * {{item}}
{% endfor %}{% endif %}
{% endif -%}

{% if exists("attention") %}
**Attention**: {% if length(attention) == 1 %}{{first(attention)}}{% else %}

{% for item in attention %}  * {{item}}
{% endfor %}{% endif %}
{% endif -%}

{% if exists("par") %}
**Par**: {% if length(par) == 1 %}{{first(par)}}{% else %}

{% for item in par %}  * {{item}}
{% endfor %}{% endif %}
{% endif -%}

{% if exists("rcs") %}
**Rcs**: {% if length(rcs) == 1 %}{{first(rcs)}}{% else %}

{% for item in rcs %}  * {{item}}
{% endfor %}{% endif %}
{% endif -%}

{% if exists("reimplements") %}
**Reimplements**: [{{reimplements.fullname}}]({{reimplements.url}})

{% endif -%}

{% if exists("reimplementedBy") %}
**Reimplemented by**: {% for impl in reimplementedBy %}[{{impl.fullname}}]({{impl.url}}){% if not loop.is_last %}, {% endif %}{% endfor %}

{% endif -%}

{% if exists("details") %}
{{details}}

{% endif -%}

{% if exists("inbody") %}
{{inbody}}

{% endif -%})";

static std::string createTableIfInherited(const std::string& visibility,
    const std::string& title,
    const std::string& key,
    const bool inherited) {
    std::stringstream ss;

    if (inherited) {
        ss << "{%- if existsIn(base, \"" << key << "\") -%}\n";
        ss << "**" << title << " inherited from [{{base.name}}]({{base.url}})**\n";
    } else {
        ss << "{%- if exists(\"" << key << "\") %}";
        ss << "## " << title << "\n";
    }

    return ss.str();
}

static std::string createTableForNamespaceLike(const std::string& visibility,
    const std::string& title,
    const std::string& key,
    const bool inherited) {
    std::stringstream ss;

    ss << createTableIfInherited(visibility, title, key, inherited);

    ss << "\n";
    ss << "| Name           |\n";
    ss << "| -------------- |\n";

    ss << "{% for child in " << (inherited ? "base." : "") << key << " -%}\n";

    ss << "| **[{{child.name}}]({{child.url}})** ";
    ss << "{% if existsIn(child, \"brief\") %}<br>{{child.brief}}{% endif %} |\n";

    ss << "{% endfor %}\n{% endif -%}\n";

    return ss.str();
}

static std::string createTableForClassLike(const std::string& visibility,
    const std::string& title,
    const std::string& key,
    const bool inherited) {
    std::stringstream ss;

    ss << createTableIfInherited(visibility, title, key, inherited);

    ss << "\n";
    ss << "|                | Name           |\n";
    ss << "| -------------- | -------------- |\n";

    ss << "{% for child in " << (inherited ? "base." : "") << key << " -%}\n";

    ss << "| {{child.kind}} | ";
    ss << "**[{{child.name}}]({{child.url}})** ";
    ss << "{% if existsIn(child, \"brief\") %}<br>{{child.brief}}{% endif %} |\n";

    ss << "{% endfor %}\n{% endif -%}\n";

    return ss.str();
}

static std::string createTableForClassStripLike(const std::string& visibility,
    const std::string& title,
    const std::string& key,
    const bool inherited) {
    std::stringstream ss;

    ss << createTableIfInherited(visibility, title, key, inherited);

    ss << "\n";
    ss << "|                | Name           |\n";
    ss << "| -------------- | -------------- |\n";

    ss << "{% for child in " << (inherited ? "base." : "") << key << " -%}\n";

    ss << "| {{child.kind}} | ";
    ss << "**[{{last(stripNamespace(child.name))}}]({{child.url}})** ";
    ss << "{% if existsIn(child, \"brief\") %}<br>{{child.brief}}{% endif %} |\n";

    ss << "{% endfor %}\n{% endif -%}\n";

    return ss.str();
}

static std::string createTableForTypeLike(const std::string& visibility,
    const std::string& title,
    const std::string& key,
    const bool inherited) {
    std::stringstream ss;

    ss << createTableIfInherited(visibility, title, key, inherited);

    ss << "\n";
    ss << "|                | Name           |\n";
    ss << "| -------------- | -------------- |\n";

    ss << "{% for child in " << (inherited ? "base." : "") << key << " -%}\n";

    ss << "| {% if existsIn(child, \"templateParams\") -%}\n";
    ss << "template <";
    ss << "{% for param in child.templateParams -%}\n";
    ss << "{{param.typePlain}} {{param.name}}";
    ss << "{% if existsIn(param, \"defvalPlain\") %} ={{param.defvalPlain}}{% endif -%}\n";
    ss << "{% if not loop.is_last %},{% endif -%}\n";
    ss << "{% endfor %}\\> <br>{% endif -%}\n";

    ss << "{{child.kind}}{% if child.kind == \"enum\" and child.strong %} class{% endif %}";
    ss << "{% if existsIn(child, \"type\") %} {{child.type}} {% endif -%}";

    ss << "| **[{{child.name}}]({{child.url}})** ";
    ss << "{% if child.kind == \"enum\" %}{ ";
    ss << "{% for enumvalue in child.enumvalues -%}\n";
    ss << "{{enumvalue.name}}";
    ss << "{% if existsIn(enumvalue, \"initializer\") %} {{enumvalue.initializer}}{% endif -%}\n";
    ss << "{% if not loop.is_last %}, {% endif %}{% endfor -%}\n }";
    ss << "{% endif -%}\n";
    ss << "{% if existsIn(child, \"brief\") %}<br>{{child.brief}}{% endif %} |\n";

    ss << "{% endfor %}\n{% endif -%}\n";

    return ss.str();
}

static std::string createTableForAttributeLike(const std::string& visibility,
    const std::string& title,
    const std::string& key,
    const bool inherited) {
    std::stringstream ss;

    ss << createTableIfInherited(visibility, title, key, inherited);

    ss << "\n";
    ss << "|                | Name           |\n";
    ss << "| -------------- | -------------- |\n";

    ss << "{% for child in " << (inherited ? "base." : "") << key << " -%}\n";
    ss << "| {% if existsIn(child, \"type\") %}{{child.type}} {% endif -%}\n";
    ss << "| **[{{child.name}}]({{child.url}})**";
    ss << " {% if existsIn(child, \"brief\") %}<br>{{child.brief}}{% endif %} |\n";
    ss << "{% endfor %}\n{% endif -%}\n";

    return ss.str();
}

static std::string createTableForJavaEnumConstants(const std::string& title,
    const std::string& key) {
    std::stringstream ss;
    ss << "{%- if exists(\"" << key << "\") %}";
    ss << "## " << title << "\n";
    ss << "\n";
    ss << "| Enum constants | Description    |\n";
    ss << "| -------------- | -------------- |\n";
    ss << "{% for child in " << key << " -%}\n";
    ss << "| **[{{child.name}}]({{child.url}})**";
    ss << "| {% if existsIn(child, \"brief\") %}{{child.brief}}{% endif %} |\n";
    ss << "{% endfor %}\n{% endif -%}\n";

    return ss.str();
}


static std::string createTableForFriendLike(const std::string& title, const std::string& key, const bool inherited) {
    std::stringstream ss;

    if (inherited) {
        ss << "{% if existsIn(base, \"" << key << "\") %}";
        ss << "**" << title;
        ss << " inherited from [{{base.name}}]({{base.url}})**\n";
    } else {
        ss << "{% if exists(\"" << key << "\") %}";
        ss << "## " << title << "\n";
    }

    ss << "\n";
    ss << "|                | Name           |\n";
    ss << "| -------------- | -------------- |\n";

    ss << "{% for child in " << (inherited ? "base." : "") << key << " -%}";

    ss << "| {% if existsIn(child, \"type\") %}{{child.type}} {% endif -%}\n";
    ss << "| **[{{child.name}}]({{child.url}})**";
    ss << "{% if child.type != \"class\" and child.type != \"struct\" -%}\n";
    ss << "({% for param in child.params -%}\n";
    ss << "{{param.type}} {{param.name}}";
    ss << "{% if existsIn(param, \"defval\") %} ={{param.defval}}{% endif -%}\n";
    ss << "{% if not loop.is_last %}, {% endif -%}\n";
    ss << "{% endfor %})";
    ss << "{% if child.const %} const{% endif -%}\n";
    ss << "{% endif %} ";
    ss << "{% if existsIn(child, \"brief\") %}<br>{{child.brief}}";
    ss << "{% endif %} |\n";

    ss << "{% endfor %}\n{% endif -%}\n";

    return ss.str();
}

static std::string createTableForFunctionLike(const std::string& visibility,
    const std::string& title,
    const std::string& key,
    const bool inherited) {
    std::stringstream ss;

    ss << createTableIfInherited(visibility, title, key, inherited);

    ss << "\n";
    ss << "|                | Name           |\n";
    ss << "| -------------- | -------------- |\n";

    ss << "{% for child in " << (inherited ? "base." : "") << key << " -%}\n";

    ss << "| {% if existsIn(child, \"templateParams\") -%}\n";
    ss << "template <";
    ss << "{% for param in child.templateParams -%}\n";
    ss << "{{param.typePlain}} {{param.name}}";
    ss << "{% if existsIn(param, \"defvalPlain\") %} ={{param.defvalPlain}}{% endif -%}\n";
    ss << "{% if not loop.is_last %},{% endif -%}\n";
    ss << "{% endfor %}\\> <br>{% endif -%}\n";

    ss << "{% if child.virtual %}virtual {% endif -%}\n";
    ss << "{% if existsIn(child, \"type\") %}{{child.type}} {% endif -%}\n";
    ss << "| **[{{child.name}}]({{child.url}})**";

    ss << "({% for param in child.params -%}\n";
    ss << "{{param.type}} {{param.name}}";
    ss << "{% if existsIn(param, \"defval\") %} ={{param.defval}}{% endif -%}\n";
    ss << "{% if not loop.is_last %}, {% endif -%}\n";
    ss << "{% endfor %})";

    ss << "{% if child.const %} const{% endif -%}\n";
    ss << "{% if child.override %} override{% endif -%}\n";
    ss << "{% if child.default %} =default{% endif -%}\n";
    ss << "{% if child.deleted %} =delete{% endif -%}\n";
    ss << "{% if child.pureVirtual %} =0{% endif -%}\n";

    ss << " {% if existsIn(child, \"brief\") %}<br>{{child.brief}}{% endif %} |\n";
    ss << "{% endfor %}\n{% endif -%}\n";

    return ss.str();
}

static std::string createTableForDefineLike(const std::string& visibility,
    const std::string& title,
    const std::string& key,
    const bool inherited) {
    std::stringstream ss;

    ss << createTableIfInherited(visibility, title, key, inherited);

    ss << "\n";
    ss << "|                | Name           |\n";
    ss << "| -------------- | -------------- |\n";

    ss << "{% for child in " << (inherited ? "base." : "") << key << " -%}\n";
    ss << "| {% if existsIn(child, \"type\") %}{{child.type}}{% endif %} | ";
    ss << "**[{{child.name}}]({{child.url}})**";
    ss << "{% if existsIn(child, \"params\") %}";
    ss << "({% for param in child.params %}";
    ss << "{{param.name}}";
    ss << "{% if existsIn(param, \"defval\") %} ={{param.defval}}{% endif %}";
    ss << "{% if not loop.is_last %}, {% endif %}";
    ss << "{% endfor %}){% endif %} ";
    ss << "{% if existsIn(child, \"brief\") %}<br>{{child.brief}}{% endif %} |\n";

    ss << "{% endfor %}\n{% endif -%}\n";

    return ss.str();
}

template <typename Fn>
static std::string
createForVisibilities(Fn& fn, const std::string& title, const std::string& key, const bool inherited) {
    std::stringstream ss;
    for (const auto& visibility : ALL_VISIBILITIES) {
        ss << fn(visibility,
            Doxybook2::Utils::title(visibility) + " " + title,
            visibility + Doxybook2::Utils::title(key),
            inherited);
    }
    return ss.str();
}

static std::string createBaseTable() {
    std::stringstream ss;
    ss << "{% for base in baseClasses -%}\n";
    ss << createForVisibilities(createTableForClassStripLike, "Classes", "classes", true);
    ss << createForVisibilities(createTableForTypeLike, "Types", "types", true);
    ss << createForVisibilities(createTableForFunctionLike, "Slots", "slots", true);
    ss << createForVisibilities(createTableForFunctionLike, "Signals", "signals", true);
    ss << createForVisibilities(createTableForFunctionLike, "Events", "events", true);
    ss << createForVisibilities(createTableForFunctionLike, "Functions", "functions", true);
    ss << createForVisibilities(createTableForAttributeLike, "Properties", "properties", true);
    ss << createForVisibilities(createTableForAttributeLike, "Attributes", "attributes", true);
    ss << createTableForFriendLike("Friends", "friends", true);
    ss << "{% endfor -%}";

    return ss.str();
}

static std::string createMemberTable() {
    std::stringstream ss;

    ss << createForVisibilities(createTableForClassStripLike, "Classes", "classes", false);
    ss << createForVisibilities(createTableForTypeLike, "Types", "types", false);
    ss << createForVisibilities(createTableForFunctionLike, "Slots", "slots", false);
    ss << createForVisibilities(createTableForFunctionLike, "Signals", "signals", false);
    ss << createForVisibilities(createTableForFunctionLike, "Events", "events", false);
    ss << createTableForJavaEnumConstants("Enum Constants", "publicJavaenumconstants");
    ss << createForVisibilities(createTableForFunctionLike, "Functions", "functions", false);
    ss << createForVisibilities(createTableForAttributeLike, "Properties", "properties", false);
    ss << createForVisibilities(createTableForAttributeLike, "Attributes", "attributes", false);
    ss << createTableForFriendLike("Friends", "friends", false);

    return ss.str();
}

static std::string createNonMemberTable() {
    std::stringstream ss;

    ss << R"({% if exists("groups") %}## Modules

| Name           |
| -------------- |
{% for child in groups -%}
| **[{{child.title}}]({{child.url}})** {% if existsIn(child, "brief") %}<br>{{child.brief}}{% endif %} |
{%- endfor %}
{% endif -%})";
    ss << "\n\n";

    ss << R"({% if exists("dirs") %}## Directories

| Name           |
| -------------- |
{% for child in dirs -%}
| **[{{child.title}}]({{child.url}})** {% if existsIn(child, "brief") %}<br>{{child.brief}}{% endif %} |
{%- endfor %}
{% endif -%})";
    ss << "\n\n";

    ss << R"({% if exists("files") %}## Files

| Name           |
| -------------- |
{% for child in files -%}
| **[{{child.title}}]({{child.url}})** {% if existsIn(child, "brief") %}<br>{{child.brief}}{% endif %} |
{%- endfor %}
{% endif -%})";
    ss << "\n\n";

    ss << createTableForNamespaceLike("public", R"({% if language == "java" %}Packages{% else %}Namespaces{% endif %})", "namespaces", false);
    ss << createTableForClassLike("public", "Classes", "publicClasses", false);
    ss << createTableForTypeLike("public", "Types", "publicTypes", false);
    ss << createTableForFunctionLike("public", "Slots", "publicSlots", false);
    ss << createTableForFunctionLike("public", "Signals", "publicSignals", false);
    ss << createTableForFunctionLike("public", "Functions", "publicFunctions", false);
    ss << createTableForAttributeLike("public", "Attributes", "publicAttributes", false);
    ss << createTableForDefineLike("public", "Defines", "defines", false);

    return ss.str();
}

static const std::string TEMPLATE_CLASS_MEMBERS_INHERITED_TABLES = createBaseTable();
static const std::string TEMPLATE_CLASS_MEMBERS_TABLES = createMemberTable();
static const std::string TEMPLATE_NONCLASS_MEMBERS_TABLES = createNonMemberTable();

static const std::string TEMPLATE_MEMBER_DETAILS =
    R"({% if kind in ["function", "slot", "signal", "event"] -%}
```{{language}}
{% if exists("templateParams") -%}
template <{% for param in templateParams %}{{param.typePlain}} {{param.name}}{% if existsIn(param, "defvalPlain") %} ={{param.defvalPlain}}{% endif -%}
{% if not loop.is_last %},
{% endif %}{% endfor %}>
{% endif -%}

{% if language == "java" %}{{visibility}} {% endif -%}
{% if static %}static {% endif -%}
{% if inline and language != "csharp" and language != "java" %}inline {% endif -%}
{% if explicit %}explicit {% endif -%}
{% if virtual %}virtual {% endif -%}

{% if exists("typePlain") %}{{typePlain}} {% endif %}{{name}}{% if length(params) > 0 -%}
(
{% for param in params %}    {{param.typePlain}} {{param.name}}{% if existsIn(param, "defvalPlain") %} ={{param.defvalPlain}}{% endif -%}
{% if not loop.is_last %},{% endif %}
{% endfor -%}
){% else -%}
(){% endif -%}

{% if const %} const{% endif -%}
{% if override %} override{% endif -%}
{% if default %} =default{% endif -%}
{% if deleted %} =delete{% endif -%}
{% if pureVirtual %} =0{% endif %}
```{% endif -%}

{% if kind == "enum" -%}
| Enumerator | Value | Description |
| ---------- | ----- | ----------- |
{% for enumvalue in enumvalues %}| {{enumvalue.name}} | {% if existsIn(enumvalue, "initializer") -%}
{{replace(enumvalue.initializer, "= ", "")}}{% endif -%}
| {% if existsIn(enumvalue, "brief") %}{{enumvalue.brief}}{% endif %} {% if existsIn(enumvalue, "details") %}{{enumvalue.details}}{% endif %} |
{% endfor %}
{% endif -%}

{% if kind in ["variable", "property", "enum constant"] -%}
```{{language}}
{% if static %}static {% endif -%}
{% if exists("typePlain") %}{{typePlain}} {% endif -%}{{name}}{% if exists("initializer") %} {{initializer}}{% endif %};
```{% endif -%}

{% if kind == "typedef" -%}
```{{language}}
{{definition}};
```{% endif -%}

{% if kind == "using" -%}
```{{language}}
{% if exists("templateParams") -%}
template <{% for param in templateParams %}{{param.typePlain}} {{param.name}}{% if existsIn(param, "defvalPlain") %} ={{param.defvalPlain}}{% endif -%}
{% if not loop.is_last %},
{% endif %}{% endfor %}>
{% endif -%}
{{definition}};
```{% endif -%}

{% if kind == "friend" -%}
```{{language}}
friend {% if exists("typePlain") %}{{typePlain}} {% endif -%}
{{name}}{% if exists("params") %}{% endif -%}
{% if length(params) > 0 -%}
(
{% for param in params %}    {{param.typePlain}} {{param.name}}{% if existsIn(param, "defvalPlain") %} ={{param.defvalPlain}}{% endif -%}
{% if not loop.is_last %},
{% endif %}
{% endfor -%}
){% else if typePlain != "class" -%}
(){% endif %};
```{% endif -%}

{% if kind == "define" -%}
```{{language}}
#define {{name}}{% if exists("params") -%}
(
{% for param in params %}    {{param.name}}{% if existsIn(param, "defvalPlain") %} ={{param.defvalPlain}}{% endif -%}
{% if not loop.is_last %},
{% endif -%}
{% endfor %}
)
{% else %} {% endif -%}
{% if exists("initializer") %}{{initializer}}{% endif %}
```{% endif %}

{% include "details" -%})";

static const std::string TEMPLATE_NONCLASS_MEMBERS_DETAILS =
    R"({% if exists("publicTypes") %}## Types Documentation

{% for child in publicTypes %}### {{child.kind}} {{child.name}}

{{ render("member_details", child) }}
{% endfor %}{% endif %}
{% if exists("publicFunctions") %}## Functions Documentation

{% for child in publicFunctions %}### {{child.kind}} {{child.name}}

{{ render("member_details", child) }}
{% endfor %}{% endif %}
{% if exists("publicAttributes") %}## Attributes Documentation

{% for child in publicAttributes %}### {{child.kind}} {{child.name}}

{{ render("member_details", child) }}
{% endfor %}{% endif %}
{% if exists("defines") %}## Macros Documentation

{% for child in defines %}### {{child.kind}} {{child.name}}

{{ render("member_details", child) }}
{% endfor %}{% endif %})";

static const std::string TEMPLATE_CLASS_MEMBERS_DETAILS =
    R"({% if exists("publicTypes") %}## Public Types Documentation

{% for child in publicTypes %}### {{child.kind}} {{child.name}}

{{ render("member_details", child) }}
{% endfor %}{% endif -%}

{% if exists("protectedTypes") %}## Protected Types Documentation

{% for child in protectedTypes %}### {{child.kind}} {{child.name}}

{{ render("member_details", child) }}
{% endfor %}{% endif -%}

{% if exists("publicSlots") %}## Public Slots Documentation

{% for child in publicSlots %}### {{child.kind}} {{child.name}}

{{ render("member_details", child) }}
{% endfor %}{% endif -%}

{% if exists("protectedSlots") %}## Protected Slots Documentation

{% for child in protectedSlots %}### {{child.kind}} {{child.name}}

{{ render("member_details", child) }}
{% endfor %}{% endif -%}

{% if exists("publicSignals") %}## Public Signals Documentation

{% for child in publicSignals %}### {{child.kind}} {{child.name}}

{{ render("member_details", child) }}
{% endfor %}{% endif -%}

{% if exists("protectedSignals") %}## Protected Signals Documentation

{% for child in protectedSignals %}### {{child.kind}} {{child.name}}

{{ render("member_details", child) }}
{% endfor %}{% endif -%}

{% if exists("publicEvents") %}## Public Events Documentation

{% for child in publicEvents %}### {{child.kind}} {{child.name}}

{{ render("member_details", child) }}
{% endfor %}{% endif -%}

{% if exists("protectedEvents") %}## Protected Events Documentation

{% for child in protectedEvents %}### {{child.kind}} {{child.name}}

{{ render("member_details", child) }}
{% endfor %}{% endif -%}

{% if exists("publicJavaenumconstants") %}## Enum Constants Documentation

{% for child in publicJavaenumconstants %}### {{child.kind}} {{child.name}}

{{ render("member_details", child) }}
{% endfor %}{% endif -%}

{% if exists("publicFunctions") %}## Public Functions Documentation

{% for child in publicFunctions %}### {{child.kind}} {{child.name}}

{{ render("member_details", child) }}
{% endfor %}{% endif -%}

{% if exists("protectedFunctions") %}## Protected Functions Documentation

{% for child in protectedFunctions %}### {{child.kind}} {{child.name}}

{{ render("member_details", child) }}
{% endfor %}{% endif -%}

{% if exists("publicProperties") %}## Public Property Documentation

{% for child in publicProperties %}### {{child.kind}} {{child.name}}

{{ render("member_details", child) }}
{% endfor %}{% endif -%}

{% if exists("protectedProperties") %}## Protected Property Documentation

{% for child in protectedProperties %}### {{child.kind}} {{child.name}}

{{ render("member_details", child) }}
{% endfor %}{% endif -%}

{% if exists("publicAttributes") %}## Public Attributes Documentation

{% for child in publicAttributes %}### {{child.kind}} {{child.name}}

{{ render("member_details", child) }}
{% endfor %}{% endif -%}

{% if exists("protectedAttributes") %}## Protected Attributes Documentation

{% for child in protectedAttributes %}### {{child.kind}} {{child.name}}

{{ render("member_details", child) }}
{% endfor %}{% endif -%}

{% if exists("friends") %}## Friends

{% for child in friends %}### {{child.kind}} {{child.name}}

{{ render("member_details", child) }}
{% endfor %}{% endif -%})";

static const std::string TEMPLATE_KIND_NONCLASS =
    R"({% include "header" -%}

{% include "breadcrumbs" -%}

{% if exists("brief") %}{{brief}}{% endif %}{% if hasDetails %} [More...](#detailed-description){% endif %}

{% include "nonclass_members_tables" -%}

{% if hasDetails %}## Detailed Description

{% include "details" %}{% endif -%}

{% include "nonclass_members_details" %}

{% include "footer" %})";

static const std::string TEMPLATE_KIND_CLASS =
    R"({% include "header" -%}

{% include "breadcrumbs" %}

{% if exists("brief") %}{{brief}}{% endif %}{% if hasDetails %} [More...](#detailed-description){% endif %}

{% if exists("includes") %}
`#include {{includes}}`

{% endif -%}

{%- if exists("baseClasses") %}Inherits from {% for child in baseClasses %}{% if existsIn(child, "url") %}[{{child.name}}]({{child.url}}){% else %}{{child.name}}{% endif %}{% if not loop.is_last %}, {% endif %}{% endfor %}

{% endif -%}
{%- if exists("derivedClasses") %}Inherited by {% for child in derivedClasses %}{% if existsIn(child, "url") %}[{{child.name}}]({{child.url}}){% else %}{{child.name}}{% endif %}{% if not loop.is_last %}, {% endif %}{% endfor %}

{% endif -%}

{%- include "class_members_tables" -%}

{% if hasAdditionalMembers %}## Additional inherited members

{% include "class_members_inherited_tables" %}
{% endif -%}

{% if hasDetails %}## Detailed Description

```{{language}}{% if exists("templateParams") %}
template <{% for param in templateParams %}{{param.typePlain}} {{param.name}}{% if existsIn(param, "defvalPlain") %} ={{param.defvalPlain}}{% endif %}{% if not loop.is_last %},
{% endif %}{% endfor %}>{% endif %}
{% if language == "java" %}{{visibility}} {% endif -%}
{% if kind == "interface" and language != "java" %}class{% else %}{{kind}}{% endif %} {{name}};
```

{% include "details" %}{% endif -%}

{% include "class_members_details" -%}

{% include "footer" %})";

static const std::string TEMPLATE_KIND_GROUP =
    R"({% include "header" -%}

{% include "breadcrumbs" -%}

{% if exists("brief") %}{{brief}}{% endif %}{% if hasDetails %} [More...](#detailed-description){% endif %}

{% include "nonclass_members_tables" -%}

{% if hasDetails %}## Detailed Description

{% include "details" %}{% endif -%}

{% include "nonclass_members_details" -%}

{% include "footer" -%}
)";

static const std::string TEMPLATE_KIND_FILE =
    R"({% include "header" -%}

{% if exists("brief") %}{{brief}}{% endif %}{% if hasDetails %} [More...](#detailed-description){% endif %}

{% include "nonclass_members_tables" -%}

{% if hasDetails %}## Detailed Description

{% include "details" %}{% endif -%}

{% include "nonclass_members_details" -%}

{% if exists("programlisting")%}## Source code

```{{language}}
{{programlisting}}
```
{% endif %}

{% include "footer" %}
)";

static const std::string TEMPLATE_KIND_PAGE =
    R"({% include "header" %}

{% if exists("details") %}{{details}}{% endif %}

{% include "footer" %}
)";

static const std::string TEMPLATE_KIND_EXAMPLE =
    R"({% include "header" %}

{% if exists("details") %}{{details}}{% endif %}

{% include "footer" %}
)";

static const std::string TEMPLATE_INDEX =
    R"(
{% for child0 in children %}* **{{child0.kind}} [{{child0.title}}]({{child0.url}})** {% if existsIn(child0, "brief") %}<br>{{child0.brief}}{% endif %}{% if existsIn(child0, "children") %}{% for child1 in child0.children %}
    * **{{child1.kind}} [{{last(stripNamespace(child1.title))}}]({{child1.url}})** {% if existsIn(child1, "brief") %}<br>{{child1.brief}}{% endif %}{% if existsIn(child1, "children") %}{% for child2 in child1.children %}
        * **{{child2.kind}} [{{last(stripNamespace(child2.title))}}]({{child2.url}})** {% if existsIn(child2, "brief") %}<br>{{child2.brief}}{% endif %}{% if existsIn(child2, "children") %}{% for child3 in child2.children %}
            * **{{child3.kind}} [{{last(stripNamespace(child3.title))}}]({{child3.url}})** {% if existsIn(child3, "brief") %}<br>{{child3.brief}}{% endif %}{% if existsIn(child3, "children") %}{% for child4 in child3.children %}
                * **{{child4.kind}} [{{last(stripNamespace(child4.title))}}]({{child4.url}})** {% if existsIn(child4, "brief") %}<br>{{child4.brief}}{% endif %}{% if existsIn(child4, "children") %}{% for child5 in child4.children %}
                    * **{{child5.kind}} [{{last(stripNamespace(child5.title))}}]({{child5.url}})** {% if existsIn(child5, "brief") %}<br>{{child5.brief}}{% endif %}{% if existsIn(child5, "children") %}{% for child6 in child5.children %}
                        * **{{child6.kind}} [{{last(stripNamespace(child6.title))}}]({{child6.url}})** {% if existsIn(child6, "brief") %}<br>{{child6.brief}}{% endif %}{% if existsIn(child6, "children") %}{% for child7 in child6.children %}
                            * **{{child7.kind}} [{{last(stripNamespace(child7.title))}}]({{child7.url}})** {% if existsIn(child7, "brief") %}<br>{{child7.brief}}{% endif %}{% endfor %}{% endif %}{% endfor %}{% endif %}{% endfor %}{% endif %}{% endfor %}{% endif %}{% endfor %}{% endif %}{% endfor %}{% endif %}{% endfor %}{% endif %}
{% endfor %}
)";

static const std::string TEMPLATE_INDEX_CLASSES =
    R"({% include "header" %}

{% include "index" %}

{% include "footer" %}
)";

static const std::string TEMPLATE_INDEX_NAMESPACES =
    R"({% include "header" %}

{% include "index" %}

{% include "footer" %}
)";

static const std::string TEMPLATE_INDEX_GROUPS =
    R"({% include "header" %}

{% include "index" %}

{% include "footer" %}
)";

static const std::string TEMPLATE_INDEX_FILES =
    R"({% include "header" %}

{% include "index" %}

{% include "footer" %}
)";

static const std::string TEMPLATE_INDEX_PAGES =
    R"({% include "header" %}

{% include "index" %}

{% include "footer" %}
)";

static const std::string TEMPLATE_INDEX_EXAMPLES =
    R"({% include "header" %}

{% include "index" %}

{% include "footer" %}
)";

// clang-format off
std::unordered_map<std::string, Doxybook2::DefaultTemplate> Doxybook2::defaultTemplates = {
    {"meta", {
        TEMPLATE_META,
        {}
    }},
    {"header", {
        TEMPLATE_HEADER,
        {"meta"}
    }},
    {"footer", {
        TEMPLATE_FOOTER,
        {}
    }},
    {"details", {
        TEMPLATE_DETAILS,
        {}
    }},
    {"breadcrumbs", {
        TEMPLATE_BREADCRUMBS,
        {}
    }},
    {"member_details", {
        TEMPLATE_MEMBER_DETAILS,
        {"details"}
    }},
    {"class_members_tables", {
        TEMPLATE_CLASS_MEMBERS_TABLES,
        {}
    }},
    {"class_members_inherited_tables", {
        TEMPLATE_CLASS_MEMBERS_INHERITED_TABLES,
        {}
    }},
    {"class_members_details", {
        TEMPLATE_CLASS_MEMBERS_DETAILS,
        {"member_details"}
    }},
    {"nonclass_members_tables", {
        TEMPLATE_NONCLASS_MEMBERS_TABLES,
        {}
    }},
    {"nonclass_members_details", {
        TEMPLATE_NONCLASS_MEMBERS_DETAILS,
        {"member_details"}
    }},
    {"index", {
        TEMPLATE_INDEX,
        {}
    }},
    {"kind_nonclass", {
        TEMPLATE_KIND_NONCLASS,
        {
            "header",
            "breadcrumbs",
            "nonclass_members_tables",
            "nonclass_members_details",
            "footer"
        }
    }},
    {"kind_class",{
        TEMPLATE_KIND_CLASS,
        {
            "header",
            "breadcrumbs",
            "class_members_tables",
            "class_members_inherited_tables",
            "class_members_details",
            "footer"
        }
    }},
    {"kind_group", {
        TEMPLATE_KIND_GROUP,
        {
            "header",
            "breadcrumbs",
            "nonclass_members_tables",
            "nonclass_members_details",
            "footer"
        }
    }},
    {"kind_file", {
        TEMPLATE_KIND_FILE,
        {
            "header",
            "nonclass_members_tables",
            "nonclass_members_details",
            "footer"
        }
    }},
    {"kind_page", {
        TEMPLATE_KIND_PAGE,
        {"header", "details", "footer"}
    }},
    {"kind_example", {
        TEMPLATE_KIND_EXAMPLE,
        {"header", "details", "footer"}
    }},
    {"index_classes", {
        TEMPLATE_INDEX_CLASSES,
        {"header", "index", "footer"}
    }},
    {"index_namespaces", {
        TEMPLATE_INDEX_NAMESPACES,
        {"header", "index", "footer"}
    }},
    {"index_groups", {
        TEMPLATE_INDEX_GROUPS,
        {"header", "index", "footer"}
    }},
    {"index_files", {
        TEMPLATE_INDEX_FILES,
        {"header", "index", "footer"}
    }},
    {"index_pages", {
        TEMPLATE_INDEX_PAGES,
        {"header", "index", "footer"}
    }},
    {"index_examples", {
        TEMPLATE_INDEX_EXAMPLES,
        {"header", "index", "footer"}
    }}
};
// clang-format on

void Doxybook2::saveDefaultTemplates(const std::string& path) {
    for (const auto& tmpl : defaultTemplates) {
        const auto tmplPath = Utils::join(path, tmpl.first + ".tmpl");
        spdlog::info("Creating default template {}", tmplPath);
        std::ofstream file(tmplPath);
        if (!file)
            throw EXCEPTION("Failed to open file {} for writing", tmplPath);

        file << tmpl.second.src;
    }
}
