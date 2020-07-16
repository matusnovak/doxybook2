include(FindPackageHandleStandardArgs)

if(NOT TARGET ArgAgg)
  find_path(ARGAGG_INCLUDE_DIR NAMES argagg/argagg.hpp)
  mark_as_advanced(FORCE ARGAGG_INCLUDE_DIR)
  add_library(ArgAgg INTERFACE IMPORTED)
  set_target_properties(ArgAgg PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${ARGAGG_INCLUDE_DIR})
endif()

find_package_handle_standard_args(ArgAgg DEFAULT_MSG ARGAGG_INCLUDE_DIR)
