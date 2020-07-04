include(FindPackageHandleStandardArgs)

find_path(ArgAgg_INCLUDE_DIR NAMES argagg/argagg.hpp)
mark_as_advanced(FORCE ArgAgg_INCLUDE_DIR)

if(NOT TARGET ArgAgg::ArgAgg)
    add_library(ArgAgg::ArgAgg INTERFACE IMPORTED)
    set_target_properties(ArgAgg::ArgAgg PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES ${ArgAgg_INCLUDE_DIR})
endif()

find_package_handle_standard_args(ArgAgg DEFAULT_MSG ArgAgg_INCLUDE_DIR)
