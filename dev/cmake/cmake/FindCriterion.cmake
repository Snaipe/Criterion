# This file is licensed under the WTFPL version 2 -- you can see the full
# license over at http://www.wtfpl.net/txt/copying/
#
# Find Criterion.
#
# Module defines the following imported library:
#   Criterion::Criterion
#
# Module defines the following result variables:
#   Criterion_FOUND - True if the Criterion library has been found
#   Criterion_INCLUDE_DIRS - Include directories needed to use Criterion
#   Criterion_LIBRARIES - Libraries needed to link to Criterion
#   Criterion_VERSION - The version of the Criterion which was found
#
# Cache variables:
#   Criterion_INCLUDE_DIR - The directory containing criterion/criterion.h
#   Criterion_LIBRARY - The path to the Criterion library
#
# Example usage:
#   find_package(Criterion)
#   target_link_libraries(foo PRIVATE Criterion::Criterion)

set(_criterionReason "")

# Try pkg-config.
find_package(PkgConfig QUIET)
if(PKG_CONFIG_FOUND)
  pkg_check_modules(PC_Criterion QUIET criterion)
endif()

find_path(
  Criterion_INCLUDE_DIR
  criterion/criterion.h
  HINTS ${PC_Criterion_INCLUDE_DIRS}
  DOC "The directory containing criterion/criterion.h"
)
mark_as_advanced(Criterion_INCLUDE_DIR)

if(NOT Criterion_INCLUDE_DIR)
  string(APPEND _criterionReason "criterion/criterion.h not found. ")
endif()

find_library(
  Criterion_LIBRARY
  NAMES criterion libcriterion
  HINTS ${PC_Criterion_LIBRARY_DIRS}
  DOC "The path to the Criterion library"
)
mark_as_advanced(Criterion_LIBRARY)

if(NOT Criterion_LIBRARY)
  string(APPEND _criterionReason "Criterion library (libcriterion) not found. ")
endif()

# Get Criterion version from pkg-config if available. Conditional check ensures
# that Criterion found by pkg-config is the one found by the find_path() and
# find_library().
if(PC_Criterion_VERSION AND Criterion_INCLUDE_DIR IN_LIST PC_Criterion_INCLUDE_DIRS)
  set(Criterion_VERSION ${PC_Criterion_VERSION})
endif()

include(FindPackageHandleStandardArgs)
# Handle the REQUIRED, QUIET, and version-related arguments of find_package().
find_package_handle_standard_args(
  Criterion
  REQUIRED_VARS Criterion_LIBRARY Criterion_INCLUDE_DIR
  VERSION_VAR Criterion_VERSION
  HANDLE_VERSION_RANGE
  REASON_FAILURE_MESSAGE "${_criterionReason}"
)

unset(_criterionReason)

if(NOT Criterion_FOUND)
  return()
endif()

set(Criterion_INCLUDE_DIRS ${Criterion_INCLUDE_DIR})
set(Criterion_LIBRARIES ${Criterion_LIBRARY})

if(NOT TARGET Criterion::Criterion)
  if(IS_ABSOLUTE "${Criterion_LIBRARY}")
    add_library(Criterion::Criterion UNKNOWN IMPORTED)
    set_target_properties(
      Criterion::Criterion
      PROPERTIES IMPORTED_LOCATION "${Criterion_LIBRARY}"
    )
  else()
    add_library(Criterion::Criterion INTERFACE IMPORTED)
    set_target_properties(
      Criterion::Criterion
      PROPERTIES IMPORTED_LIBNAME "${Criterion_LIBRARY}"
    )
  endif()

  set_target_properties(
    Criterion::Criterion
    PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${Criterion_INCLUDE_DIRS}"
  )
endif()
