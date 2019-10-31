#include <fstream>
#include <Doxydown/TemplateDefaultLoader.hpp>
#include <Doxydown/Utils.hpp>
#include "ExceptionUtils.hpp"

static const std::string TEMPLATE_HEADER =
R"(---
title: {{name}}
{% if exists("summary") %}summary: {{summary}} {% endif%}
---

{{name}} Reference
============================================
)";

static const std::string TEMPLATE_FOOTER =
R"(Created on {{date("%Y-%m-%d")}})";

static const std::string TEMPLATE_CLASS_MEMBERS_TABLES =
R"({% if exists("publicClasses") %}## Public Classes

|                | Name           |
| -------------- | -------------- |
{% for child in publicClasses %}| {{child.kind}} | **[{{stripNamespace(child.name)}}]({{child.url}})** {% if existsIn(child, "brief") %}<br/>{{child.brief}}{% endif %} |
{% endfor %}{% endif %}
{% if exists("protectedClasses") %}## Protected Classes

|                | Name           |
| -------------- | -------------- |
{% for child in protectedClasses %}| {{child.kind}} | **[{{stripNamespace(child.name)}}]({{child.url}})** {% if existsIn(child, "brief") %}<br/>{{child.brief}}{% endif %} |
{% endfor %}{% endif %}
{% if exists("publicTypes") %}## Public Types

|                | Name           |
| -------------- | -------------- |
{% for child in publicTypes %}| {{child.kind}}{% if existsIn(child, "type") %} {{child.type}}{% endif %} | **[{{child.name}}]({{child.url}})** {% if child.kind == "enum" %}{ {% for enumvalue in child.enumvalues %}{{enumvalue.name}}{% if existsIn(enumvalue, "initializer") %} {{enumvalue.initializer}}{% endif %}{% if not loop.is_last %}, {% endif %}{% endfor %} }{% endif %}{% if existsIn(child, "brief") %}<br/>{{child.brief}}{% endif %} |
{% endfor %}{% endif %}
{% if exists("protectedTypes") %}## Protected Types

|                | Name           |
| -------------- | -------------- |
{% for child in protectedTypes %}| {{child.kind}}{% if existsIn(child, "type") %} {{child.type}}{% endif %} | **[{{child.name}}]({{child.url}})** {% if child.kind == "enum" %}{ {% for enumvalue in child.enumvalues %}{{enumvalue.name}}{% if existsIn(enumvalue, "initializer") %} {{enumvalue.initializer}}{% endif %}{% if not loop.is_last %}, {% endif %}{% endfor %} }{% endif %}{% if existsIn(child, "brief") %}<br/>{{child.brief}}{% endif %} |
{% endfor %}{% endif %}
{% if exists("publicFunctions") %}## Public Functions

|                | Name           |
| -------------- | -------------- |
{% for child in publicFunctions %}| {% if child.virtual %}virtual {% endif %}{% if exists("type") %}{{child.type}}{% endif %} | **[{{child.name}}]({{child.url}})**({% for param in child.params %}{{param.type}} {{param.name}}{% if existsIn(param, "defval") %} ={{param.defval}}{% endif %}{% if not loop.is_last %}, {% endif %}{% endfor %}){% if child.const %} const{% endif %}{% if child.default %} =default{% endif %}{% if child.deleted %} =deleted{% endif %}{% if child.pureVirtual %} =0{% endif %} {% if existsIn(child, "brief") %}<br/>{{child.brief}}{% endif %} |
{% endfor %}{% endif %}
{% if exists("protectedFunctions") %}## Protected Functions

|                | Name           |
| -------------- | -------------- |
{% for child in protectedFunctions %}| {% if child.virtual %}virtual {% endif %}{% if exists("type") %}{{child.type}}{% endif %} | **[{{child.name}}]({{child.url}})**({% for param in child.params %}{{param.type}} {{param.name}}{% if existsIn(param, "defval") %} ={{param.defval}}{% endif %}{% if not loop.is_last %}, {% endif %}{% endfor %}){% if child.const %} const{% endif %}{% if child.default %} =default{% endif %}{% if child.deleted %} =deleted{% endif %}{% if child.pureVirtual %} =0{% endif %} {% if existsIn(child, "brief") %}<br/>{{child.brief}}{% endif %} |
{% endfor %}{% endif %}
{% if exists("publicAttributes") %}## Public Attributes

|                | Name           |
| -------------- | -------------- |
{% for child in publicAttributes %}| {% if existsIn(child, "type") %}{{child.type}}{% endif %} | **[{{child.name}}]({{child.url}})** {% if existsIn(child, "brief") %}<br/>{{child.brief}}{% endif %} |
{% endfor %}{% endif %}
{% if exists("protectedAttributes") %}## Protected Attributes

|                | Name           |
| -------------- | -------------- |
{% for child in protectedAttributes %}| {% if existsIn(child, "type") %}{{child.type}}{% endif %} | **[{{child.name}}]({{child.url}})** {% if existsIn(child, "brief") %}<br/>{{child.brief}}{% endif %} |
{% endfor %}{% endif %}
{% if exists("friends") %}## Friends

|                | Name           |
| -------------- | -------------- |
{% for child in friends %}| {% if existsIn(child, "type") %}{{child.type}}{% endif %} | **[{{child.name}}]({{child.url}})**{% if child.type != "class" %}({% for param in child.params %}{{param.type}} {{param.name}}{% if existsIn(param, "defval") %} ={{param.defval}}{% endif %}{% if not loop.is_last %}, {% endif %}{% endfor %}){% if child.const %} const{% endif %}{% endif %} {% if existsIn(child, "brief") %}<br/>{{child.brief}}{% endif %} |
{% endfor %}{% endif %})";

static const std::string TEMPLATE_NONCLASS_MEMBERS_TABLES =
R"({% if exists("groups") %}## Groups

| Name           |
| -------------- |
{% for child in groups %}| **[{{stripNamespace(child.name)}}]({{child.url}})** {% if existsIn(child, "brief") %}<br/>{{child.brief}}{% endif %} |
{% endfor %}{% endif %}
{% if exists("namespaces") %}## Namespaces

| Name           |
| -------------- |
{% for child in namespaces %}| **[{{stripNamespace(child.name)}}]({{child.url}})** {% if existsIn(child, "brief") %}<br/>{{child.brief}}{% endif %} |
{% endfor %}{% endif %}
{% if exists("publicClasses") %}## Classes

|                | Name           |
| -------------- | -------------- |
{% for child in publicClasses %}| {{child.kind}} | **[{{stripNamespace(child.name)}}]({{child.url}})** {% if existsIn(child, "brief") %}<br/>{{child.brief}}{% endif %} |
{% endfor %}{% endif %}
{% if exists("publicTypes") %}## Types

|                | Name           |
| -------------- | -------------- |
{% for child in publicTypes %}| {{child.kind}}{% if existsIn(child, "type") %} {{child.type}}{% endif %} | **[{{child.name}}]({{child.url}})** {% if child.kind == "enum" %}{ {% for enumvalue in child.enumvalues %}{{enumvalue.name}}{% if existsIn(enumvalue, "initializer") %} {{enumvalue.initializer}}{% endif %}{% if not loop.is_last %}, {% endif %}{% endfor %} }{% endif %}{% if existsIn(child, "brief") %}<br/>{{child.brief}}{% endif %} |
{% endfor %}{% endif %}
{% if exists("publicFunctions") %}## Functions

|                | Name           |
| -------------- | -------------- |
{% for child in publicFunctions %}| {% if child.virtual %}virtual {% endif %}{% if existsIn(child, "type") %}{{child.type}}{% endif %} | **[{{child.name}}]({{child.url}})**({% for param in child.params %}{{param.type}} {{param.name}}{% if existsIn(param, "defval") %} ={{param.defval}}{% endif %}{% if not loop.is_last %}, {% endif %}{% endfor %}){% if child.const %} const{% endif %}{% if child.default %} =default{% endif %}{% if child.deleted %} =deleted{% endif %}{% if child.pureVirtual %} =0{% endif %} {% if existsIn(child, "brief") %}<br/>{{child.brief}}{% endif %} |
{% endfor %}{% endif %}
{% if exists("publicAttributes") %}## Attributes

|                | Name           |
| -------------- | -------------- |
{% for child in publicAttributes %}| {% if existsIn(child, "type") %}{{child.type}}{% endif %} | **[{{child.name}}]({{child.url}})** {% if existsIn(child, "brief") %}<br/>{{child.brief}}{% endif %} |
{% endfor %}{% endif %})";

static const std::string TEMPLATE_MEMBER_DETAILS =
R"({% if kind == "function" %}```cpp
{% if static %}static {% endif %}{% if inline %}inline {% endif %}{% if explicit %}explicit {% endif %}{% if virtual %}virtual {% endif %}{% if exists("typePlain") %}{{typePlain}} {% endif %}{{name}}{% if length(params) > 0 %}(
{% for param in params %}    {{param.typePlain}} {{param.name}}{% if existsIn(param, "defvalPlain") %} ={{param.defvalPlain}}{% endif %}{% if not loop.is_last %},{% endif %}
{% endfor %}){% else %}(){% endif %}{% if const %} const{% endif %}{% if default %} =default{% endif %}{% if deleted %} =deleted{% endif %}{% if pureVirtual %} =0{% endif %}
```{% endif %}{% if kind == "enum" %}```cpp
enum {{name}} {
{% for enumvalue in enumvalues %}    {{enumvalue.name}}{% if existsIn(enumvalue, "initializer") %} {{enumvalue.initializer}}{% endif %}{% if not loop.is_last %},{% endif%}
{% endfor %}}
```{% endif %}{% if kind == "variable" %}```cpp
{% if static %}static {% endif %}{% if exists("typePlain") %}{{typePlain}} {% endif %}{{name}}{% if exists("initializer") %} {{initializer}}{% endif %};
```{% endif %}{% if kind == "typedef" %}```cpp
typedef {% if exists("typePlain") %}{{typePlain}} {% endif %}{{name}};
```{% endif %}{% if kind == "friend" %}```cpp
friend {% if exists("typePlain") %}{{typePlain}} {% endif %}{{name}}{% if exists("params") %}{% endif %}{% if length(params) > 0 %}(
{% for param in params %}    {{param.typePlain}} {{param.name}}{% if existsIn(param, "defvalPlain") %} ={{param.defvalPlain}}{% endif %}{% if not loop.is_last %},{% endif %}
{% endfor %}){% else %}(){% endif %};
```{% endif %}

{% if exists("brief") %}{{brief}}
{% endif %}
{% if exists("detailsSections") %}{% for section in detailsSections %}**{{title(section.type)}}**: {{section.text}}

{% endfor %}{% endif %}
{% if exists("details") %}{{details}}
{% endif %}
{% if exists("inbody") %}{{inbody}}
{% endif %})";

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
{% endfor %}{% endif %})";

static const std::string TEMPLATE_CLASS_MEMBERS_DETAILS =
R"({% if exists("publicTypes") %}## Public Types Documentation

{% for child in publicTypes %}### {{child.kind}} {{child.name}}

{{ render("member_details", child) }}
{% endfor %}{% endif %}
{% if exists("protectedTypes") %}## Protected Types Documentation

{% for child in protectedTypes %}### {{child.kind}} {{child.name}}

{{ render("member_details", child) }}
{% endfor %}{% endif %}
{% if exists("publicFunctions") %}## Public Functions Documentation

{% for child in publicFunctions %}### {{child.kind}} {{child.name}}

{{ render("member_details", child) }}
{% endfor %}{% endif %}
{% if exists("protectedFunctions") %}## Protected Functions Documentation

{% for child in protectedFunctions %}### {{child.kind}} {{child.name}}

{{ render("member_details", child) }}
{% endfor %}{% endif %}
{% if exists("publicAttributes") %}## Public Attributes Documentation

{% for child in publicAttributes %}### {{child.kind}} {{child.name}}

{{ render("member_details", child) }}
{% endfor %}{% endif %}
{% if exists("protectedAttributes") %}## Protected Attributes Documentation

{% for child in protectedAttributes %}### {{child.kind}} {{child.name}}

{{ render("member_details", child) }}
{% endfor %}{% endif %}
{% if exists("friends") %}## Friends

{% for child in friends %}### {{child.kind}} {{child.name}}

{{ render("member_details", child) }}
{% endfor %}{% endif %})";

static const std::string TEMPLATE_KIND_NONCLASS =
R"({% include "header" %}

{% if exists("brief") %}{{brief}}{% endif %}{% if exists("details") %} [More...](#detailed-description)
{% endif %}

{% include "nonclass_members_tables" %}

{% if exists("details") or exists("detailsSections") %}## Detailed Description

{% if exists("brief") %}{{brief}}
{% endif %}
{% if exists("detailsSections") %}{% for section in detailsSections %}**{{title(section.type)}}**: {{section.text}}

{% endfor %}{% endif %}
{% if exists("details") %}{{ details}}{% endif %}{% endif %}

{% include "nonclass_members_details" %}

{% include "footer" %})";

static const std::string TEMPLATE_KIND_CLASS =
R"({% include "header" %}

{% if exists("brief") %}{{brief}}{% endif %}{% if exists("details") %} [More...](#detailed-description)
{% endif %}

{% if exists("includes") %}`#include {{includes}}`
{% endif %}

{% if exists("baseClasses") %}Inherits from {% for child in baseClasses %}{% if existsIn(child, "url") %}[{{child.name}}]({{child.url}}){% else %}{{child.name}}{% endif %}{% if not loop.is_last %}, {% endif %}{% endfor %}
{% endif %}
{% if exists("derivedClasses") %}Inherited by {% for child in derivedClasses %}{% if existsIn(child, "url") %}[{{child.name}}]({{child.url}}){% else %}{{child.name}}{% endif %}{% if not loop.is_last %}, {% endif %}{% endfor %}
{% endif %}

{% include "class_members_tables" %}

{% if exists("details") or exists("detailsSections") %}## Detailed Description

{% if exists("brief") %}{{brief}}
{% endif %}
{% if exists("detailsSections") %}{% for section in detailsSections %}**{{title(section.type)}}**: {{section.text}}

{% endfor %}{% endif %}
{% if exists("details") %}{{ details}}{% endif %}{% endif %}

{% include "class_members_details" %}

{% include "footer" %})";

static const std::string TEMPLATE_KIND_GROUP =
R"({% include "header" %}

{% if exists("brief") %}{{brief}}{% endif %}{% if exists("details") %} [More...](#detailed-description)
{% endif %}

{% include "nonclass_members_tables" %}

{% if exists("details") or exists("detailsSections") %}## Detailed Description

{% if exists("brief") %}{{brief}}
{% endif %}
{% if exists("detailsSections") %}{% for section in detailsSections %}**{{title(section.type)}}**: {{section.text}}

{% endfor %}{% endif %}
{% if exists("details") %}{{ details}}{% endif %}{% endif %}

{% include "nonclass_members_details" %}

{% include "footer" %}
)";

static const std::string TEMPLATE_INDEX =
R"(
{% for child0 in children %}  * **{{child0.kind}} [{{child0.name}}]({{child0.url}})**{% if existsIn(child0, "brief") %}</br>{{child0.brief}}{% endif %}{% if existsIn(child0, "children") %}{% for child1 in child0.children %}
    * **{{child1.kind}} [{{stripNamespace(child1.name)}}]({{child1.url}})**{% if existsIn(child1, "brief") %}</br>{{child1.brief}}{% endif %}{% if existsIn(child1, "children") %}{% for child2 in child1.children %}
      * **{{child2.kind}} [{{stripNamespace(child2.name)}}]({{child2.url}})**{% if existsIn(child2, "brief") %}</br>{{child2.brief}}{% endif %}{% if existsIn(child2, "children") %}{% for child3 in child2.children %}
        * **{{child3.kind}} [{{stripNamespace(child3.name)}}]({{child3.url}})**{% if existsIn(child3, "brief") %}</br>{{child3.brief}}{% endif %}{% if existsIn(child3, "children") %}{% for child4 in child3.children %}
          * **{{child4.kind}} [{{stripNamespace(child4.name)}}]({{child4.url}})**{% if existsIn(child4, "brief") %}</br>{{child4.brief}}{% endif %}{% if existsIn(child4, "children") %}{% for child5 in child4.children %}
            * **{{child5.kind}} [{{stripNamespace(child5.name)}}]({{child5.url}})**{% if existsIn(child5, "brief") %}</br>{{child5.brief}}{% endif %}{% if existsIn(child5, "children") %}{% for child6 in child5.children %}
              * **{{child6.kind}} [{{stripNamespace(child6.name)}}]({{child6.url}})**{% if existsIn(child6, "brief") %}</br>{{child6.brief}}{% endif %}{% if existsIn(child6, "children") %}{% for child7 in child6.children %}
                * **{{child7.kind}} [{{stripNamespace(child7.name)}}]({{child7.url}})**{% if existsIn(child7, "brief") %}</br>{{child7.brief}}{% endif %}{% endfor %}{% endif %}{% endfor %}{% endif %}{% endfor %}{% endif %}{% endfor %}{% endif %}{% endfor %}{% endif %}{% endfor %}{% endif %}{% endfor %}{% endif %}
{% endfor %}
)";

static const std::string TEMPLATE_INDEX_CLASSES =
R"(---
title: Classes
---

{% include "index" %}

{% include "footer" %}
)";

static const std::string TEMPLATE_INDEX_NAMESPACES =
R"(---
title: Namespaces
---

{% include "index" %}

{% include "footer" %}
)";

static const std::string TEMPLATE_INDEX_GROUPS =
R"(---
title: Modules
---

{% include "index" %}

{% include "footer" %}
)";

Doxydown::TemplateDefaultLoader::TemplateDefaultLoader() {
    templates = {
        {"header", TEMPLATE_HEADER},
        {"footer", TEMPLATE_FOOTER},
        {"member_details", TEMPLATE_MEMBER_DETAILS},
        {"class_members_tables", TEMPLATE_CLASS_MEMBERS_TABLES},
        {"class_members_details", TEMPLATE_CLASS_MEMBERS_DETAILS},
        {"nonclass_members_tables", TEMPLATE_NONCLASS_MEMBERS_TABLES},
        {"nonclass_members_details", TEMPLATE_NONCLASS_MEMBERS_DETAILS},
        {"kind_nonclass", TEMPLATE_KIND_NONCLASS},
        {"kind_class", TEMPLATE_KIND_CLASS},
        {"kind_group", TEMPLATE_KIND_GROUP},
        {"index", TEMPLATE_INDEX},
        {"index_classes", TEMPLATE_INDEX_CLASSES},
        {"index_namespaces", TEMPLATE_INDEX_NAMESPACES},
        {"index_groups", TEMPLATE_INDEX_GROUPS}, 
    };
}

void Doxydown::TemplateDefaultLoader::saveAll(const std::string& path) const {
    for (const auto& tmpl : templates) {
        const auto tmplPath = Utils::join(path, tmpl.name + ".tmpl");
        Log::i("Creating default template {}", tmplPath);
        std::ofstream file(tmplPath);
        if (!file) throw EXCEPTION("Failed to open file {} for writing", tmplPath);

        file << tmpl.contents;
    }
}
