include(FindPackageHandleStandardArgs)

if(NOT TARGET Dirent)
  find_path(DIRENT_INCLUDE_DIR NAMES dirent.h)
  mark_as_advanced(FORCE DIRENT_INCLUDE_DIR)
  add_library(Dirent INTERFACE IMPORTED)
  set_target_properties(Dirent PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${DIRENT_INCLUDE_DIR})
endif()

find_package_handle_standard_args(Dirent DEFAULT_MSG DIRENT_INCLUDE_DIR)
