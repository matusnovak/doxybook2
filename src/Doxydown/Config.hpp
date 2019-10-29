#pragma once
#include <string>

namespace Doxydown {
    struct Config {
        // Where to store the output files?
        std::string outputDir;

        // Generate extra JSON for each rendered template
        bool debugTemplateJson{false};

        // Should we put the class, namespace, modules, and files indexes
        // into their respective folders? (Hugo/Learn) needs that!
        bool indexInFolders{false};

        // What should the index files be named?
        std::string indexFileName{""};

        // What is the base URL to prepend all links with?
        std::string baseUrl{""};

        // What should the file end with?
        std::string fileExt{"md"};

        // What should the link end with?
        std::string linkSuffix{".md"};

        // What are the folder names?
        std::string folderGroupsName{"Modules"};
        std::string folderClassesName{"Classes"};
        std::string folderFilesName{"Files"};
        std::string folderRelatedPagesName{"Related Pages"};
        std::string folderNamespacesName{"Namespaces"};

        // What templates to use for each kind?
        std::string templateKindClass{"kind_class"};
        std::string templateKindStruct{"kind_class"};
        std::string templateKindUnion{"kind_class"};
        std::string templateKindInterface{"kind_class"};
        std::string templateKindNamespace{"kind_nonclass"};
        std::string templateKindGroup{"kind_nonclass"};
        std::string templateKindFile{"kind_file"};
        std::string templateKindDir{"kind_file"};

        // What templates to use for indexes?
        std::string templateIndexClasses{"index_classes"};
        std::string templateIndexNamespaces{"index_namespaces"};
        std::string templateIndexGroups{"index_groups"};
        std::string templateIndexFiles{"index_files"};
    };
}
