include(FindPackageHandleStandardArgs)

find_path(Rang_INCLUDE_DIR NAMES rang.hpp)
mark_as_advanced(FORCE Rang_INCLUDE_DIR)

if(NOT TARGET Rang::Rang)
    add_library(Rang::Rang INTERFACE IMPORTED)
    set_target_properties(Rang::Rang PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES ${Rang_INCLUDE_DIR})
endif()

find_package_handle_standard_args(Rang DEFAULT_MSG Rang_INCLUDE_DIR)
