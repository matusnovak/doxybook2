include(FindPackageHandleStandardArgs)

if(NOT TARGET Rang)
  find_path(RANG_INCLUDE_DIR NAMES rang.hpp)
  mark_as_advanced(FORCE RANG_INCLUDE_DIR)
  add_library(Rang INTERFACE IMPORTED)
  set_target_properties(Rang PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${RANG_INCLUDE_DIR})
endif()

find_package_handle_standard_args(Rang DEFAULT_MSG RANG_INCLUDE_DIR)
