#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "lcpl-ast" for configuration ""
set_property(TARGET lcpl-ast APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(lcpl-ast PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "CXX"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/liblcpl-ast.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS lcpl-ast )
list(APPEND _IMPORT_CHECK_FILES_FOR_lcpl-ast "${_IMPORT_PREFIX}/lib/liblcpl-ast.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
