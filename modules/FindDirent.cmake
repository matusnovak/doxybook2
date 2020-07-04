include(FindPackageHandleStandardArgs)

find_path(Dirent_INCLUDE_DIR NAMES dirent.h)
mark_as_advanced(FORCE Dirent_INCLUDE_DIR)

if(NOT TARGET Dirent::Dirent)
    add_library(Dirent::Dirent INTERFACE IMPORTED)
    set_target_properties(Dirent::Dirent PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES ${Dirent_INCLUDE_DIR})
endif()

find_package_handle_standard_args(Dirent DEFAULT_MSG Dirent_INCLUDE_DIR)
