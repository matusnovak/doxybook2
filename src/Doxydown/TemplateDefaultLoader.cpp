#include <fstream>
#include <Doxydown/TemplateDefaultLoader.hpp>
#include <Doxydown/Utils.hpp>
#include "ExceptionUtils.hpp"

static const std::string TEMPLATE_META =
R"()";

static const std::string TEMPLATE_HEADER =
R"(---
{% if exists("title") %}title: {{title}}{% else if exists("name") %}title: {{name}}{% endif %}
{% if exists("summary") %}summary: {{summary}} {% endif%}
{% include "meta" %}
---

{% if exists("title") %}# {{title}}{% else if exists("kind") and kind != "page" %}# {{name}} {{title(kind)}} Reference{% endif %}
)";

static const std::string TEMPLATE_BREADCRUMBS =
R"({% if exists("moduleBreadcrumbs") %}**Module:** {% for module in moduleBreadcrumbs %}**[{{module.title}}]({{module.url}})**{% if not loop.is_last %} **/** {% endif %}{% endfor %}{% endif %})";

static const std::string TEMPLATE_FOOTER =
R"(-------------------------------

Updated on {{date("%e %B %Y at %H:%M:%S %Z")}})";

static const std::string TEMPLATE_DETAILS =
R"({% if exists("brief") %}{{brief}}
{% endif %}
{% if exists("paramsList") %}**Parameters**: 

{% for param in paramsList %}  * **{{param.name}}** {{param.text}}
{% endfor %}
{% endif %}{% if exists("returnsList") %}**Returns**: 

{% for param in returnsList %}  * **{{param.name}}** {{param.text}}
{% endfor %}
{% endif %}{% if exists("exceptionsList") %}**Exceptions**: 

{% for param in exceptionsList %}  * **{{param.name}}** {{param.text}}
{% endfor %}
{% endif %}
{% if exists("templateParamsList") %}**Template Parameters**: 

{% for param in templateParamsList %}  * **{{param.name}}** {{param.text}}
{% endfor %}
{% endif %}{% if exists("see") %}**See**: {% if length(see) == 1 %}{{first(see)}}{% else %}

{% for item in see %}  * {{item}}
{% endfor %}{% endif %}
{% endif %}{% if exists("returns") %}**Return**: {% if length(returns) == 1 %}{{first(returns)}}{% else %}

{% for item in returns %}  * {{item}}
{% endfor %}{% endif %}
{% endif %}{% if exists("authors") %}**Author**: {% if length(authors) == 1 %}{{first(authors)}}{% else %}

{% for item in authors %}  * {{item}}
{% endfor %}{% endif %}
{% endif %}{% if exists("version") %}**Version**: {% if length(version) == 1 %}{{first(version)}}{% else %}

{% for item in version %}  * {{item}}
{% endfor %}{% endif %}
{% endif %}{% if exists("since") %}**Since**: {% if length(since) == 1 %}{{first(since)}}{% else %}

{% for item in since %}  * {{item}}
{% endfor %}{% endif %}
{% endif %}{% if exists("date") %}**Date**: {% if length(date) == 1 %}{{first(date)}}{% else %}

{% for item in date %}  * {{item}}
{% endfor %}{% endif %}
{% endif %}{% if exists("note") %}**Note**: {% if length(note) == 1 %}{{first(note)}}{% else %}

{% for item in note %}  * {{item}}
{% endfor %}{% endif %}
{% endif %}{% if exists("bugs") %}**Bug**: {% if length(bugs) == 1 %}{{first(bugs)}}{% else %}

{% for item in bugs %}  * {{item}}
{% endfor %}{% endif %}
{% endif %}{% if exists("tests") %}**Test**: {% if length(tests) == 1 %}{{first(tests)}}{% else %}

{% for item in tests %}  * {{item}}
{% endfor %}{% endif %}
{% endif %}{% if exists("todos") %}**Todo**: {% if length(todos) == 1 %}{{first(todos)}}{% else %}

{% for item in todos %}  * {{item}}
{% endfor %}{% endif %}
{% endif %}{% if exists("warning") %}**Warning**: {% if length(warning) == 1 %}{{first(warning)}}{% else %}

{% for item in warning %}  * {{item}}
{% endfor %}{% endif %}
{% endif %}{% if exists("pre") %}**Precondition**: {% if length(pre) == 1 %}{{first(pre)}}{% else %}

{% for item in pre %}  * {{item}}
{% endfor %}{% endif %}
{% endif %}{% if exists("post") %}**Postcondition**: {% if length(post) == 1 %}{{first(post)}}{% else %}

{% for item in post %}  * {{item}}
{% endfor %}{% endif %}
{% endif %}{% if exists("copyright") %}**Copyright**: {% if length(copyright) == 1 %}{{first(copyright)}}{% else %}

{% for item in copyright %}  * {{item}}
{% endfor %}{% endif %}
{% endif %}{% if exists("invariant") %}**Invariant**: {% if length(invariant) == 1 %}{{first(invariant)}}{% else %}

{% for item in invariant %}  * {{item}}
{% endfor %}{% endif %}
{% endif %}{% if exists("remark") %}**Remark**: {% if length(remark) == 1 %}{{first(remark)}}{% else %}

{% for item in remark %}  * {{item}}
{% endfor %}{% endif %}
{% endif %}{% if exists("attention") %}**Attention**: {% if length(attention) == 1 %}{{first(attention)}}{% else %}

{% for item in attention %}  * {{item}}
{% endfor %}{% endif %}
{% endif %}{% if exists("par") %}**Par**: {% if length(par) == 1 %}{{first(par)}}{% else %}

{% for item in par %}  * {{item}}
{% endfor %}{% endif %}
{% endif %}{% if exists("rcs") %}**Rcs**: {% if length(rcs) == 1 %}{{first(rcs)}}{% else %}

{% for item in rcs %}  * {{item}}
{% endfor %}{% endif %}
{% endif %}{% if exists("reimplements") %}**Reimplements**: [{{reimplements.fullname}}]({{reimplements.url}})

{% endif %}{% if exists("reimplementedBy") %}**Reimplemented by**: {% for impl in reimplementedBy %}[{{impl.fullname}}]({{impl.url}}){% if not loop.is_last %}, {% endif %}{% endfor %}

{% endif %}
{% if exists("details") %}{{details}}
{% endif %}
{% if exists("inbody") %}{{inbody}}
{% endif %})";


static const std::string TEMPLATE_CLASS_MEMBERS_INHERITED_TABLES =
R"({% for base in baseClasses %}
{% if existsIn(base, "publicClasses") %}**Public Classes inherited from [{{base.name}}]({{base.url}})**

|                | Name           |
| -------------- | -------------- |
{% for child in base.publicClasses %}| {{child.kind}} | **[{{last(split(child.name, "::"))}}]({{child.url}})** {% if existsIn(child, "brief") %}<br/>{{child.brief}}{% endif %} |
{% endfor %}{% endif %}
{% if existsIn(base, "protectedClasses") %}**Protected Classes inherited from [{{base.name}}]({{base.url}})**

|                | Name           |
| -------------- | -------------- |
{% for child in base.protectedClasses %}| {{child.kind}} | **[{{last(split(child.name, "::"))}}]({{child.url}})** {% if existsIn(child, "brief") %}<br/>{{child.brief}}{% endif %} |
{% endfor %}{% endif %}
{% if existsIn(base, "publicTypes") %}**Public Types inherited from [{{base.name}}]({{base.url}})**

|                | Name           |
| -------------- | -------------- |
{% for child in base.publicTypes %}| {{child.kind}}{% if existsIn(child, "type") %} {{child.type}}{% endif %} | **[{{child.name}}]({{child.url}})** {% if child.kind == "enum" %}{ {% for enumvalue in child.enumvalues %}{{enumvalue.name}}{% if existsIn(enumvalue, "initializer") %} {{enumvalue.initializer}}{% endif %}{% if not loop.is_last %}, {% endif %}{% endfor %} }{% endif %}{% if existsIn(child, "brief") %}<br/>{{child.brief}}{% endif %} |
{% endfor %}{% endif %}
{% if existsIn(base, "protectedTypes") %}**Protected Types inherited from [{{base.name}}]({{base.url}})**

|                | Name           |
| -------------- | -------------- |
{% for child in base.protectedTypes %}| {{child.kind}}{% if existsIn(child, "type") %} {{child.type}}{% endif %} | **[{{child.name}}]({{child.url}})** {% if child.kind == "enum" %}{ {% for enumvalue in child.enumvalues %}{{enumvalue.name}}{% if existsIn(enumvalue, "initializer") %} {{enumvalue.initializer}}{% endif %}{% if not loop.is_last %}, {% endif %}{% endfor %} }{% endif %}{% if existsIn(child, "brief") %}<br/>{{child.brief}}{% endif %} |
{% endfor %}{% endif %}
{% if existsIn(base, "publicFunctions") %}**Public Functions inherited from [{{base.name}}]({{base.url}})**

|                | Name           |
| -------------- | -------------- |
{% for child in base.publicFunctions %}| {% if child.virtual %}virtual {% endif %}{% if existsIn(child, "type") %}{{child.type}}{% endif %} | **[{{child.name}}]({{child.url}})**({% for param in child.params %}{{param.type}} {{param.name}}{% if existsIn(param, "defval") %} ={{param.defval}}{% endif %}{% if not loop.is_last %}, {% endif %}{% endfor %}){% if child.const %} const{% endif %}{% if child.override %} override{% endif %}{% if child.default %} =default{% endif %}{% if child.deleted %} =deleted{% endif %}{% if child.pureVirtual %} =0{% endif %} {% if existsIn(child, "brief") %}<br/>{{child.brief}}{% endif %} |
{% endfor %}{% endif %}
{% if existsIn(base, "protectedFunctions") %}**Protected Functions inherited from [{{base.name}}]({{base.url}})**

|                | Name           |
| -------------- | -------------- |
{% for child in base.protectedFunctions %}| {% if child.virtual %}virtual {% endif %}{% if existsIn(child, "type") %}{{child.type}}{% endif %} | **[{{child.name}}]({{child.url}})**({% for param in child.params %}{{param.type}} {{param.name}}{% if existsIn(param, "defval") %} ={{param.defval}}{% endif %}{% if not loop.is_last %}, {% endif %}{% endfor %}){% if child.const %} const{% endif %}{% if child.override %} override{% endif %}{% if child.default %} =default{% endif %}{% if child.deleted %} =deleted{% endif %}{% if child.pureVirtual %} =0{% endif %} {% if existsIn(child, "brief") %}<br/>{{child.brief}}{% endif %} |
{% endfor %}{% endif %}
{% if existsIn(base, "publicAttributes") %}**Public Attributes inherited from [{{base.name}}]({{base.url}})**

|                | Name           |
| -------------- | -------------- |
{% for child in base.publicAttributes %}| {% if existsIn(child, "type") %}{{child.type}}{% endif %} | **[{{child.name}}]({{child.url}})** {% if existsIn(child, "brief") %}<br/>{{child.brief}}{% endif %} |
{% endfor %}{% endif %}
{% if existsIn(base, "protectedAttributes") %}**Protected Attributes inherited from [{{base.name}}]({{base.url}})**

|                | Name           |
| -------------- | -------------- |
{% for child in base.protectedAttributes %}| {% if existsIn(child, "type") %}{{child.type}}{% endif %} | **[{{child.name}}]({{child.url}})** {% if existsIn(child, "brief") %}<br/>{{child.brief}}{% endif %} |
{% endfor %}{% endif %}
{% if existsIn(base, "friends") %}**Friends inherited from [{{base.name}}]({{base.url}})**

|                | Name           |
| -------------- | -------------- |
{% for child in base.friends %}| {% if existsIn(child, "type") %}{{child.type}}{% endif %} | **[{{child.name}}]({{child.url}})**{% if child.type != "class" %}({% for param in child.params %}{{param.type}} {{param.name}}{% if existsIn(param, "defval") %} ={{param.defval}}{% endif %}{% if not loop.is_last %}, {% endif %}{% endfor %}){% if child.const %} const{% endif %}{% endif %} {% if existsIn(child, "brief") %}<br/>{{child.brief}}{% endif %} |
{% endfor %}{% endif %}{% endfor %})";


static const std::string TEMPLATE_CLASS_MEMBERS_TABLES =
R"({% if exists("publicClasses") %}## Public Classes

|                | Name           |
| -------------- | -------------- |
{% for child in publicClasses %}| {{child.kind}} | **[{{last(split(child.name, "::"))}}]({{child.url}})** {% if existsIn(child, "brief") %}<br/>{{child.brief}}{% endif %} |
{% endfor %}{% endif %}
{% if exists("protectedClasses") %}## Protected Classes

|                | Name           |
| -------------- | -------------- |
{% for child in protectedClasses %}| {{child.kind}} | **[{{last(split(child.name, "::"))}}]({{child.url}})** {% if existsIn(child, "brief") %}<br/>{{child.brief}}{% endif %} |
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
{% for child in publicFunctions %}| {% if child.virtual %}virtual {% endif %}{% if existsIn(child, "type") %}{{child.type}}{% endif %} | **[{{child.name}}]({{child.url}})**({% for param in child.params %}{{param.type}} {{param.name}}{% if existsIn(param, "defval") %} ={{param.defval}}{% endif %}{% if not loop.is_last %}, {% endif %}{% endfor %}){% if child.const %} const{% endif %}{% if child.override %} override{% endif %}{% if child.default %} =default{% endif %}{% if child.deleted %} =deleted{% endif %}{% if child.pureVirtual %} =0{% endif %} {% if existsIn(child, "brief") %}<br/>{{child.brief}}{% endif %} |
{% endfor %}{% endif %}
{% if exists("protectedFunctions") %}## Protected Functions

|                | Name           |
| -------------- | -------------- |
{% for child in protectedFunctions %}| {% if child.virtual %}virtual {% endif %}{% if existsIn(child, "type") %}{{child.type}}{% endif %} | **[{{child.name}}]({{child.url}})**({% for param in child.params %}{{param.type}} {{param.name}}{% if existsIn(param, "defval") %} ={{param.defval}}{% endif %}{% if not loop.is_last %}, {% endif %}{% endfor %}){% if child.const %} const{% endif %}{% if child.override %} override{% endif %}{% if child.default %} =default{% endif %}{% if child.deleted %} =deleted{% endif %}{% if child.pureVirtual %} =0{% endif %} {% if existsIn(child, "brief") %}<br/>{{child.brief}}{% endif %} |
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
R"({% if exists("groups") %}## Modules

| Name           |
| -------------- |
{% for child in groups %}| **[{{child.title}}]({{child.url}})** {% if existsIn(child, "brief") %}<br/>{{child.brief}}{% endif %} |
{% endfor %}{% endif %}
{% if exists("dirs") %}## Directories

| Name           |
| -------------- |
{% for child in dirs %}| **[{{child.title}}]({{child.url}})** {% if existsIn(child, "brief") %}<br/>{{child.brief}}{% endif %} |
{% endfor %}{% endif %}
{% if exists("files") %}## Files

| Name           |
| -------------- |
{% for child in files %}| **[{{child.title}}]({{child.url}})** {% if existsIn(child, "brief") %}<br/>{{child.brief}}{% endif %} |
{% endfor %}{% endif %}
{% if exists("namespaces") %}## Namespaces

| Name           |
| -------------- |
{% for child in namespaces %}| **[{{child.name}}]({{child.url}})** {% if existsIn(child, "brief") %}<br/>{{child.brief}}{% endif %} |
{% endfor %}{% endif %}
{% if exists("publicClasses") %}## Classes

|                | Name           |
| -------------- | -------------- |
{% for child in publicClasses %}| {{child.kind}} | **[{{child.name}}]({{child.url}})** {% if existsIn(child, "brief") %}<br/>{{child.brief}}{% endif %} |
{% endfor %}{% endif %}
{% if exists("publicTypes") %}## Types

|                | Name           |
| -------------- | -------------- |
{% for child in publicTypes %}| {{child.kind}}{% if existsIn(child, "type") %} {{child.type}}{% endif %} | **[{{child.name}}]({{child.url}})** {% if child.kind == "enum" %}{ {% for enumvalue in child.enumvalues %}{{enumvalue.name}}{% if existsIn(enumvalue, "initializer") %} {{enumvalue.initializer}}{% endif %}{% if not loop.is_last %}, {% endif %}{% endfor %} }{% endif %}{% if existsIn(child, "brief") %}<br/>{{child.brief}}{% endif %} |
{% endfor %}{% endif %}
{% if exists("publicFunctions") %}## Functions

|                | Name           |
| -------------- | -------------- |
{% for child in publicFunctions %}| {% if child.virtual %}virtual {% endif %}{% if existsIn(child, "type") %}{{child.type}}{% endif %} | **[{{child.name}}]({{child.url}})**({% for param in child.params %}{{param.type}} {{param.name}}{% if existsIn(param, "defval") %} ={{param.defval}}{% endif %}{% if not loop.is_last %}, {% endif %}{% endfor %}){% if child.const %} const{% endif %}{% if child.override %} override{% endif %}{% if child.default %} =default{% endif %}{% if child.deleted %} =deleted{% endif %}{% if child.pureVirtual %} =0{% endif %} {% if existsIn(child, "brief") %}<br/>{{child.brief}}{% endif %} |
{% endfor %}{% endif %}
{% if exists("publicAttributes") %}## Attributes

|                | Name           |
| -------------- | -------------- |
{% for child in publicAttributes %}| {% if existsIn(child, "type") %}{{child.type}}{% endif %} | **[{{child.name}}]({{child.url}})** {% if existsIn(child, "brief") %}<br/>{{child.brief}}{% endif %} |
{% endfor %}{% endif %}
{% if exists("defines") %}## Defines

|                | Name           |
| -------------- | -------------- |
{% for child in defines %}| {% if existsIn(child, "type") %}{{child.type}}{% endif %} | **[{{child.name}}]({{child.url}})**{% if existsIn(child, "params") %}({% for param in child.params %}{{param.name}}{% if existsIn(param, "defval") %} ={{param.defval}}{% endif %}{% if not loop.is_last %}, {% endif %}{% endfor %}){% endif %} {% if existsIn(child, "brief") %}<br/>{{child.brief}}{% endif %} |
{% endfor %}{% endif %})";

static const std::string TEMPLATE_MEMBER_DETAILS =
R"({% if kind == "function" %}```cpp
{% if static %}static {% endif %}{% if inline %}inline {% endif %}{% if explicit %}explicit {% endif %}{% if virtual %}virtual {% endif %}{% if exists("typePlain") %}{{typePlain}} {% endif %}{{name}}{% if length(params) > 0 %}(
{% for param in params %}    {{param.typePlain}} {{param.name}}{% if existsIn(param, "defvalPlain") %} ={{param.defvalPlain}}{% endif %}{% if not loop.is_last %},{% endif %}
{% endfor %}){% else %}(){% endif %}{% if const %} const{% endif %}{% if override %} override{% endif %}{% if default %} =default{% endif %}{% if deleted %} =deleted{% endif %}{% if pureVirtual %} =0{% endif %}
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
{% endfor %}){% else if typePlain != "class" %}(){% endif %};
```{% endif %}{% if kind == "define" %}```cpp
#define {{name}}{% if exists("params") %}(
{% for param in params %}    {{param.name}}{% if existsIn(param, "defvalPlain") %} ={{param.defvalPlain}}{% endif %}{% if not loop.is_last %},{% endif %}
{% endfor %}){% endif %}{% if exists("initializer") %} {{initializer}}{% endif %}
```{% endif %}

{% include "details" %})";

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
{% if exists("defines") %}## Macro Documentation

{% for child in defines %}### {{child.kind}} {{child.name}}

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

{% include "breadcrumbs" %}

{% if exists("brief") %}{{brief}}{% endif %}{% if hasDetails %} [More...](#detailed-description)
{% endif %}

{% include "nonclass_members_tables" %}

{% if hasDetails %}## Detailed Description

{% include "details" %}{% endif %}

{% include "nonclass_members_details" %}

{% include "footer" %})";

static const std::string TEMPLATE_KIND_CLASS =
R"({% include "header" %}

{% include "breadcrumbs" %}

{% if exists("brief") %}{{brief}}{% endif %}{% if hasDetails %} [More...](#detailed-description)
{% endif %}

{% if exists("includes") %}`#include {{includes}}`
{% endif %}

{% if exists("baseClasses") %}Inherits from {% for child in baseClasses %}{% if existsIn(child, "url") %}[{{child.name}}]({{child.url}}){% else %}{{child.name}}{% endif %}{% if not loop.is_last %}, {% endif %}{% endfor %}
{% endif %}
{% if exists("derivedClasses") %}Inherited by {% for child in derivedClasses %}{% if existsIn(child, "url") %}[{{child.name}}]({{child.url}}){% else %}{{child.name}}{% endif %}{% if not loop.is_last %}, {% endif %}{% endfor %}
{% endif %}

{% include "class_members_tables" %}

{% if hasAdditionalMembers %}## Additional inherited members

{% include "class_members_inherited_tables" %}
{% endif %}

{% if hasDetails %}## Detailed Description

```cpp{% if exists("templateParams") %}
template <{% for param in templateParams %}{{param.typePlain}} {{param.name}}{% if existsIn(param, "defvalPlain") %} ={{param.defvalPlain}}{% endif %}{% if not loop.is_last %},
{% endif %}{% endfor %}>{% endif %}
{{kind}} {{name}};
```

{% include "details" %}{% endif %}

{% include "class_members_details" %}

{% include "footer" %})";

static const std::string TEMPLATE_KIND_GROUP =
R"({% include "header" %}

{% include "breadcrumbs" %}

{% if exists("brief") %}{{brief}}{% endif %}{% if hasDetails %} [More...](#detailed-description)
{% endif %}

{% include "nonclass_members_tables" %}

{% if hasDetails %}## Detailed Description

{% include "details" %}{% endif %}

{% include "nonclass_members_details" %}

{% include "footer" %}
)";

static const std::string TEMPLATE_KIND_FILE =
R"({% include "header" %}

{% if exists("brief") %}{{brief}}{% endif %}{% if hasDetails %} [More...](#detailed-description)
{% endif %}

{% include "nonclass_members_tables" %}

{% if hasDetails %}## Detailed Description

{% include "details" %}{% endif %}

{% include "nonclass_members_details" %}

{% include "footer" %}
)";

static const std::string TEMPLATE_KIND_PAGE =
R"({% include "header" %}

{% if exists("details") %}{{details}}{% endif %}

{% include "footer" %}
)";

static const std::string TEMPLATE_INDEX =
R"(
{% for child0 in children %}* **{{child0.kind}} [{{child0.title}}]({{child0.url}})**{% if existsIn(child0, "brief") %}</br>{{child0.brief}}{% endif %}{% if existsIn(child0, "children") %}{% for child1 in child0.children %}
    * **{{child1.kind}} [{{last(split(child1.title, "::"))}}]({{child1.url}})**{% if existsIn(child1, "brief") %}</br>{{child1.brief}}{% endif %}{% if existsIn(child1, "children") %}{% for child2 in child1.children %}
        * **{{child2.kind}} [{{last(split(child2.title, "::"))}}]({{child2.url}})**{% if existsIn(child2, "brief") %}</br>{{child2.brief}}{% endif %}{% if existsIn(child2, "children") %}{% for child3 in child2.children %}
            * **{{child3.kind}} [{{last(split(child3.title, "::"))}}]({{child3.url}})**{% if existsIn(child3, "brief") %}</br>{{child3.brief}}{% endif %}{% if existsIn(child3, "children") %}{% for child4 in child3.children %}
                * **{{child4.kind}} [{{last(split(child4.title, "::"))}}]({{child4.url}})**{% if existsIn(child4, "brief") %}</br>{{child4.brief}}{% endif %}{% if existsIn(child4, "children") %}{% for child5 in child4.children %}
                    * **{{child5.kind}} [{{last(split(child5.title, "::"))}}]({{child5.url}})**{% if existsIn(child5, "brief") %}</br>{{child5.brief}}{% endif %}{% if existsIn(child5, "children") %}{% for child6 in child5.children %}
                        * **{{child6.kind}} [{{last(split(child6.title, "::"))}}]({{child6.url}})**{% if existsIn(child6, "brief") %}</br>{{child6.brief}}{% endif %}{% if existsIn(child6, "children") %}{% for child7 in child6.children %}
                            * **{{child7.kind}} [{{last(split(child7.title, "::"))}}]({{child7.url}})**{% if existsIn(child7, "brief") %}</br>{{child7.brief}}{% endif %}{% endfor %}{% endif %}{% endfor %}{% endif %}{% endfor %}{% endif %}{% endfor %}{% endif %}{% endfor %}{% endif %}{% endfor %}{% endif %}{% endfor %}{% endif %}
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

Doxydown::TemplateDefaultLoader::TemplateDefaultLoader() {
    templates = {
        {"meta", TEMPLATE_META},
        {"header", TEMPLATE_HEADER},
        {"footer", TEMPLATE_FOOTER},
        {"details", TEMPLATE_DETAILS},
        {"breadcrumbs", TEMPLATE_BREADCRUMBS},
        {"member_details", TEMPLATE_MEMBER_DETAILS},
        {"class_members_tables", TEMPLATE_CLASS_MEMBERS_TABLES},
        {"class_members_inherited_tables", TEMPLATE_CLASS_MEMBERS_INHERITED_TABLES},
        {"class_members_details", TEMPLATE_CLASS_MEMBERS_DETAILS},
        {"nonclass_members_tables", TEMPLATE_NONCLASS_MEMBERS_TABLES},
        {"nonclass_members_details", TEMPLATE_NONCLASS_MEMBERS_DETAILS},
        {"kind_nonclass", TEMPLATE_KIND_NONCLASS},
        {"kind_class", TEMPLATE_KIND_CLASS},
        {"kind_group", TEMPLATE_KIND_GROUP},
        {"kind_file", TEMPLATE_KIND_FILE},
        {"kind_page", TEMPLATE_KIND_PAGE},
        {"index", TEMPLATE_INDEX},
        {"index_classes", TEMPLATE_INDEX_CLASSES},
        {"index_namespaces", TEMPLATE_INDEX_NAMESPACES},
        {"index_groups", TEMPLATE_INDEX_GROUPS},
        {"index_files", TEMPLATE_INDEX_FILES},
        {"index_pages", TEMPLATE_INDEX_PAGES},
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
