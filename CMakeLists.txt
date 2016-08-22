# Copyright (C) 2015-2016 Franklin "Snaipe" Mathieu.
# Redistribution and use of this file is allowed according to the terms of the MIT license.
# For details see the LICENSE file distributed with Criterion.

cmake_minimum_required (VERSION 2.8)

project (Criterion C)

set (MODULE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/.cmake/Modules")
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${MODULE_DIR})

if (POLICY CMP0054)
  # http://www.cmake.org/cmake/help/v3.1/policy/CMP0054.html
  # This is here to allow conditions to be passed as function parameters
  cmake_policy (SET CMP0054 OLD)
endif ()

# Initialization

include (Options)
include (Submodules)
include (Capabilities)
include (Subprojects)
include (PackageUtils)

cr_add_subproject (csptr PATH dependencies/libcsptr
    OPTS -DLIBCSPTR_TESTS=OFF
    CMAKE)
cr_add_subproject (dyncall_s PATH dependencies/dyncall CMAKE IF THEORIES)

if (NOT WIN32)
    set (NN_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
else ()
    set (NN_C_FLAGS "${CMAKE_C_FLAGS}")
endif ()

cr_add_subproject (nanomsg
  PATH dependencies/nanomsg-patched
  OPTS
    -DNN_TESTS=OFF
    -DNN_TOOLS=OFF
    -DNN_FORK_STRATEGY=RESET
    -DNN_STATIC_LIB=ON
    "-DCMAKE_C_FLAGS=${NN_C_FLAGS}"
  CMAKE
)

cr_add_subproject (wingetopt PATH dependencies/wingetopt CMAKE IF MSVC)

include (Properties)

add_definitions (-DNN_STATIC_LIB)

include_directories(
  dependencies/valgrind/include/
  dependencies/klib/
  dependencies/nanopb/
  dependencies/debugbreak/
)

# Coverage

if (COVERALLS)
  include(Coveralls)
  coveralls_turn_on_coverage()
endif()

# I18N

if (I18N AND GETTEXT_FOUND AND LIBINTL_LIB_FOUND)
  include(GettextTranslate)
  add_subdirectory(po)
endif ()

# Project

include_directories(include src)
add_subdirectory (src)

cr_add_library(criterion SHARED
    SOURCES ${SOURCE_FILES}
    HEADERS ${INTERFACE_FILES}
    COMPONENT dev
    PROPERTIES
       VERSION ${PROJECT_SONAME}
       SOVERSION ${PROJECT_SOVERSION}
)

cr_link_subproject(criterion csptr STATIC)
cr_link_subproject(criterion nanomsg STATIC)
cr_link_subproject(criterion dyncall_s STATIC)
cr_link_subproject(criterion wingetopt STATIC)

cr_link_libraries(criterion pthread IF NOT WIN32)
cr_link_libraries(criterion rt IF HAVE_CLOCK_GETTIME)

# Required by nanomsg
cr_link_libraries(criterion anl IF HAVE_GETADDRINFO_A)
cr_link_libraries(criterion ws2_32 mswsock IF WIN32)

cr_link_package(criterion LIBINTL)

if (COVERALLS)
  coveralls_setup("${SOURCE_FILES}" ${COVERALLS_UPLOAD})

  add_custom_target(gcov
    "${CMAKE_COMMAND}"
        -DSOURCE_FILES="${SOURCE_FILES}"
        -DCOV_PATH="${CMAKE_CURRENT_BINARY_DIR}"
        -P "${CMAKE_MODULE_PATH}/Gcov.cmake"
  )
endif()

if (CTESTS)
  enable_testing()
  add_custom_target(criterion_tests)
  add_subdirectory(samples)
  add_subdirectory(test)
endif ()

if (UPLOAD_DEB)
  include (DebUpload)
endif ()
