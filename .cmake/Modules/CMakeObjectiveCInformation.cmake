if(UNIX)
  set(CMAKE_ObjectiveC_OUTPUT_EXTENSION .o)
else()
  set(CMAKE_ObjectiveC_OUTPUT_EXTENSION .obj)
endif()

set(_INCLUDED_FILE 0)
set(_INCLUDED_FILE_2 0)

# Load compiler-specific information.
if(CMAKE_ObjectiveC_COMPILER_ID)
  include(Compiler/${CMAKE_ObjectiveC_COMPILER_ID}-C OPTIONAL)
endif()

set(CMAKE_BASE_NAME)
get_filename_component(CMAKE_BASE_NAME "${CMAKE_ObjectiveC_COMPILER}" NAME_WE)

# load a hardware specific file, mostly useful for embedded compilers
if(CMAKE_SYSTEM_PROCESSOR)
  if(CMAKE_ObjectiveC_COMPILER_ID)
    include(Platform/${CMAKE_SYSTEM_NAME}-${CMAKE_ObjectiveC_COMPILER_ID}-C-${CMAKE_SYSTEM_PROCESSOR} OPTIONAL RESULT_VARIABLE _INCLUDED_FILE)
    include(Platform/${CMAKE_SYSTEM_NAME}-${CMAKE_ObjectiveC_COMPILER_ID}-ObjectiveC-${CMAKE_SYSTEM_PROCESSOR} OPTIONAL RESULT_VARIABLE _INCLUDED_FILE_2)
  endif()
  if (NOT _INCLUDED_FILE AND NOT _INCLUDED_FILE_2)
    include(Platform/${CMAKE_SYSTEM_NAME}-${CMAKE_BASE_NAME}-${CMAKE_SYSTEM_PROCESSOR} OPTIONAL)
  endif ()
endif()

# load the system- and compiler specific files
if(CMAKE_ObjectiveC_COMPILER_ID)
  include(Platform/${CMAKE_SYSTEM_NAME}-${CMAKE_ObjectiveC_COMPILER_ID}-C
    OPTIONAL RESULT_VARIABLE _INCLUDED_FILE)
  include(Platform/${CMAKE_SYSTEM_NAME}-${CMAKE_ObjectiveC_COMPILER_ID}-ObjectiveC
    OPTIONAL RESULT_VARIABLE _INCLUDED_FILE_2)
endif()
if (NOT _INCLUDED_FILE AND _INCLUDED_FILE_2)
  include(Platform/${CMAKE_SYSTEM_NAME}-${CMAKE_BASE_NAME}
    OPTIONAL RESULT_VARIABLE _INCLUDED_FILE)
endif ()

# We specify the compiler information in the system file for some
# platforms, but this language may not have been enabled when the file
# was first included.  Include it again to get the language info.
# Remove this when all compiler info is removed from system files.
if (NOT _INCLUDED_FILE)
  include(Platform/${CMAKE_SYSTEM_NAME} OPTIONAL)
endif ()

if(CMAKE_ObjectiveC_COMPILER_LINKS_STATICALLY)
  set_property(GLOBAL PROPERTY TARGET_SUPPORTS_SHARED_LIBS FALSE)
endif()

# This should be included before the _INIT variables are
# used to initialize the cache.  Since the rule variables
# have if blocks on them, users can still define them here.
# But, it should still be after the platform file so changes can
# be made to those values.

if(CMAKE_USER_MAKE_RULES_OVERRIDE)
  # Save the full path of the file so try_compile can use it.
  include(${CMAKE_USER_MAKE_RULES_OVERRIDE} RESULT_VARIABLE _override)
  set(CMAKE_USER_MAKE_RULES_OVERRIDE "${_override}")
endif()

if(CMAKE_USER_MAKE_RULES_OVERRIDE_ObjectiveC)
  # Save the full path of the file so try_compile can use it.
  include(${CMAKE_USER_MAKE_RULES_OVERRIDE_ObjectiveC} RESULT_VARIABLE _override)
  set(CMAKE_USER_MAKE_RULES_OVERRIDE_ObjectiveC "${_override}")
endif()


# for most systems a module is the same as a shared library
# so unless the variable CMAKE_MODULE_EXISTS is set just
# copy the values from the LIBRARY variables
if(NOT CMAKE_MODULE_EXISTS)
  set(CMAKE_SHARED_MODULE_ObjectiveC_FLAGS ${CMAKE_SHARED_LIBRARY_ObjectiveC_FLAGS})
  set(CMAKE_SHARED_MODULE_CREATE_ObjectiveC_FLAGS ${CMAKE_SHARED_LIBRARY_CREATE_ObjectiveC_FLAGS})
endif()

if(NOT CMAKE_NOT_USING_CONFIG_FLAGS)
# default build type is none
  if(NOT CMAKE_NO_BUILD_TYPE)
    set (CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE_INIT} CACHE STRING
      "Choose the type of build, options are: None(CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel.")
  endif()
  set (CMAKE_ObjectiveC_FLAGS_DEBUG "${CMAKE_ObjectiveC_FLAGS_DEBUG_INIT}" CACHE STRING
    "Flags used by the compiler during debug builds.")
  set (CMAKE_ObjectiveC_FLAGS_MINSIZEREL "${CMAKE_ObjectiveC_FLAGS_MINSIZEREL_INIT}" CACHE STRING
    "Flags used by the compiler during release builds for minimum size.")
  set (CMAKE_ObjectiveC_FLAGS_RELEASE "${CMAKE_ObjectiveC_FLAGS_RELEASE_INIT}" CACHE STRING
    "Flags used by the compiler during release builds.")
  set (CMAKE_ObjectiveC_FLAGS_RELWITHDEBINFO "${CMAKE_ObjectiveC_FLAGS_RELWITHDEBINFO_INIT}" CACHE STRING
    "Flags used by the compiler during release builds with debug info.")
endif()

if(CMAKE_ObjectiveC_STANDARD_LIBRARIES_INIT)
  set(CMAKE_ObjectiveC_STANDARD_LIBRARIES "${CMAKE_C_STANDARD_LIBRARIES_INIT}"
    CACHE STRING "Libraries linked by default with all C applications.")
  mark_as_advanced(CMAKE_ObjectiveC_STANDARD_LIBRARIES)
endif()

include(CMakeCommonLanguageInclude)

# now define the following rule variables

# CMAKE_ObjectiveC_CREATE_SHARED_LIBRARY
# CMAKE_ObjectiveC_CREATE_SHARED_MODULE
# CMAKE_ObjectiveC_COMPILE_OBJECT
# CMAKE_ObjectiveC_LINK_EXECUTABLE

# variables supplied by the generator at use time
# <TARGET>
# <TARGET_BASE> the target without the suffix
# <OBJECTS>
# <OBJECT>
# <LINK_LIBRARIES>
# <FLAGS>
# <LINK_FLAGS>

# C compiler information
# <CMAKE_ObjectiveC_COMPILER>
# <CMAKE_SHARED_LIBRARY_CREATE_ObjectiveC_FLAGS>
# <CMAKE_SHARED_MODULE_CREATE_ObjectiveC_FLAGS>
# <CMAKE_ObjectiveC_LINK_FLAGS>

# Static library tools
# <CMAKE_AR>
# <CMAKE_RANLIB>


# create a C shared library
if(NOT CMAKE_ObjectiveC_CREATE_SHARED_LIBRARY)
  set(CMAKE_ObjectiveC_CREATE_SHARED_LIBRARY
    "<CMAKE_ObjectiveC_COMPILER> <CMAKE_SHARED_LIBRARY_ObjectiveC_FLAGS> <LANGUAGE_COMPILE_FLAGS> <LINK_FLAGS> <CMAKE_SHARED_LIBRARY_CREATE_ObjectiveC_FLAGS> <SONAME_FLAG><TARGET_SONAME> -o <TARGET> <OBJECTS> <LINK_LIBRARIES>")
endif()

# create a C shared module just copy the shared library rule
if(NOT CMAKE_ObjectiveC_CREATE_SHARED_MODULE)
  set(CMAKE_ObjectiveC_CREATE_SHARED_MODULE ${CMAKE_ObjectiveC_CREATE_SHARED_LIBRARY})
endif()

# Create a static archive incrementally for large object file counts.
# If CMAKE_ObjectiveC_CREATE_STATIObjectiveC_LIBRARY is set it will override these.
if(NOT DEFINED CMAKE_ObjectiveC_ARCHIVE_CREATE)
  set(CMAKE_ObjectiveC_ARCHIVE_CREATE "<CMAKE_AR> qc <TARGET> <LINK_FLAGS> <OBJECTS>")
endif()
if(NOT DEFINED CMAKE_ObjectiveC_ARCHIVE_APPEND)
  set(CMAKE_ObjectiveC_ARCHIVE_APPEND "<CMAKE_AR> q  <TARGET> <LINK_FLAGS> <OBJECTS>")
endif()
if(NOT DEFINED CMAKE_ObjectiveC_ARCHIVE_FINISH)
  set(CMAKE_ObjectiveC_ARCHIVE_FINISH "<CMAKE_RANLIB> <TARGET>")
endif()

set(CMAKE_INCLUDE_FLAG_ObjectiveC "-I")
set(CMAKE_ObjectiveC_IMPLICIT_LINK_LIBRARIES objc)

# compile a C file into an object file
if(NOT CMAKE_ObjectiveC_COMPILE_OBJECT)
  set(CMAKE_ObjectiveC_COMPILE_OBJECT
    "<CMAKE_ObjectiveC_COMPILER> <DEFINES> <FLAGS> -o <OBJECT>   -c <SOURCE>")
endif()

if(NOT CMAKE_ObjectiveC_LINK_EXECUTABLE)
  set(CMAKE_ObjectiveC_LINK_EXECUTABLE
    "<CMAKE_ObjectiveC_COMPILER> <FLAGS> <CMAKE_ObjectiveC_LINK_FLAGS> <LINK_FLAGS> <OBJECTS>  -o <TARGET> <LINK_LIBRARIES>")
endif()

if(NOT CMAKE_EXECUTABLE_RUNTIME_ObjectiveC_FLAG)
  set(CMAKE_EXECUTABLE_RUNTIME_ObjectiveC_FLAG ${CMAKE_SHARED_LIBRARY_RUNTIME_ObjectiveC_FLAG})
endif()

if(NOT CMAKE_EXECUTABLE_RUNTIME_ObjectiveC_FLAG_SEP)
  set(CMAKE_EXECUTABLE_RUNTIME_ObjectiveC_FLAG_SEP ${CMAKE_SHARED_LIBRARY_RUNTIME_ObjectiveC_FLAG_SEP})
endif()

if(NOT CMAKE_EXECUTABLE_RPATH_LINK_ObjectiveC_FLAG)
  set(CMAKE_EXECUTABLE_RPATH_LINK_ObjectiveC_FLAG ${CMAKE_SHARED_LIBRARY_RPATH_LINK_ObjectiveC_FLAG})
endif()

mark_as_advanced(
  CMAKE_ObjectiveC_FLAGS
  CMAKE_ObjectiveC_FLAGS_DEBUG
  CMAKE_ObjectiveC_FLAGS_MINSIZEREL
  CMAKE_ObjectiveC_FLAGS_RELEASE
  CMAKE_ObjectiveC_FLAGS_RELWITHDEBINFO
)
set(CMAKE_ObjectiveC_INFORMATION_LOADED 1)
