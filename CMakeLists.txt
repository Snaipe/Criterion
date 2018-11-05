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
include (Capabilities)
include (Subprojects)
include (PackageUtils)
include (CheckCCompilerFlag)

check_c_compiler_flag("-fPIC" CC_HAVE_FPIC)
check_c_compiler_flag("-fvisibility=hidden" CC_HAVE_VISIBILITY)

# Linker flags are ignored by check_c_compiler_flag, so we have
# to use this workaround
set (CMAKE_REQUIRED_FLAGS "-Wl,--exclude-libs=ALL")
check_c_compiler_flag("" LINKER_HAS_EXCLUDE_LIBS)
unset (CMAKE_REQUIRED_FLAGS)

if (LINKER_HAS_EXCLUDE_LIBS)
  set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wl,--exclude-libs=ALL")
endif ()

if (CC_HAVE_FPIC)
  set (PIC_C_FLAGS "-fPIC")
endif ()

if (CC_HAVE_VISIBILITY)
  set (VISI_C_FLAGS "-fvisibility=hidden")
endif ()

find_package(Libcsptr)
find_package(Dyncall)
find_package(Nanomsg)
find_package(BoxFort)
find_package(Libgit2)

cr_add_subproject (csptr
  GIT "https://github.com/Snaipe/libcsptr#0d52904"
  PATH dependencies/libcsptr
  OPTS
    -DLIBCSPTR_TESTS=OFF
    "-DCMAKE_C_FLAGS=${CMAKE_C_FLAGS} ${PIC_C_FLAGS} ${VISI_C_FLAGS}"
  CMAKE
  IF NOT CSPTR_FOUND
)

cr_add_subproject (dyncall_s
  GIT "https://github.com/Snaipe/dyncall#51e79a8"
  PATH dependencies/dyncall
  OPTS
    -DLANG_CXX=${LANG_CXX}
    "-DCMAKE_C_FLAGS=${CMAKE_C_FLAGS} ${PIC_C_FLAGS} ${VISI_C_FLAGS}"
  CMAKE
  IF THEORIES AND NOT DYNCALL_FOUND
)

cr_add_subproject (nanomsg
  GIT "https://github.com/nanomsg/nanomsg.git#7e12a20"
  PATH dependencies/nanomsg
  OPTS
    -DNN_TESTS=OFF
    -DNN_TOOLS=OFF
    -DNN_STATIC_LIB=ON
    -DCMAKE_INSTALL_LIBDIR=lib
    "-DCMAKE_C_FLAGS=${CMAKE_C_FLAGS} ${PIC_C_FLAGS} ${VISI_C_FLAGS}"
  CMAKE
  IF NOT NANOMSG_FOUND
)

cr_add_subproject (boxfort PATH dependencies/boxfort
  GIT "https://github.com/diacritic/BoxFort.git#926bd4c"
  PATH dependencies/boxfort
  OPTS
    -DBXF_TESTS=OFF
    -DBXF_SAMPLES=OFF
    -DBXF_STATIC_LIB=ON
    -DBXF_FORK_RESILIENCE=OFF
    -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
    "-DCMAKE_C_FLAGS=${CMAKE_C_FLAGS} ${PIC_C_FLAGS} ${VISI_C_FLAGS}"
  CMAKE
  IF NOT BOXFORT_FOUND)

cr_add_subproject (git2
  GIT "git://github.com/libgit2/libgit2#v0.25.1"
  OPTS
    -DBUILD_SHARED_LIBS=OFF
    -DBUILD_CLAR=OFF
    -DUSE_ICONV=OFF
    -DUSE_SSH=OFF
    -DUSE_GSSAPI=OFF
    -DUSE_OPENSSL=OFF
    -DVALGRIND=OFF
    -DCURL=OFF
    -DWINHTTP=OFF
    -DCMAKE_DISABLE_FIND_PACKAGE_HTTP_Parser=TRUE
    -DCMAKE_DISABLE_FIND_PACKAGE_ZLIB=TRUE
    -DCMAKE_DISABLE_FIND_PACKAGE_Iconv=TRUE
    -DCMAKE_DISABLE_FIND_PACKAGE_Security=TRUE
    "-DCMAKE_C_FLAGS=${PIC_C_FLAGS} ${VISI_C_FLAGS}"
  CMAKE
  IF NOT LIBGIT2_FOUND)

add_definitions(-DBXF_STATIC_LIB -DNN_STATIC_LIB)

cr_add_subproject (wingetopt
  GIT "git://github.com/alex85k/wingetopt.git#76a5d1a"
  PATH dependencies/wingetopt
  CMAKE
  IF MSVC
)

include (Properties)

if (NOT NANOMSG_FOUND OR NANOMSG_LIBRARIES MATCHES ".*\\.a")
  set (STATIC_NANOMSG 1)
  add_definitions (-DNN_STATIC_LIB)
endif ()

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
  include_directories(SYSTEM ${LIBINTL_INCLUDE_DIR})
  include(GettextTranslate)
  add_subdirectory(po)
endif ()

# Project

include_directories(include src ${CMAKE_CURRENT_BINARY_DIR}/src)
add_subdirectory (src)

cr_add_library(criterion SHARED
    SOURCES ${SOURCE_FILES}
    HEADERS ${INTERFACE_FILES}
    COMPONENT dev
    PROPERTIES
       VERSION ${PROJECT_SONAME}
       SOVERSION ${PROJECT_SOVERSION}
)

cr_link_subproject(criterion csptr STATIC IF NOT CSPTR_FOUND)
cr_link_subproject(criterion nanomsg STATIC IF NOT NANOMSG_FOUND)
cr_link_subproject(criterion dyncall_s STATIC IF NOT DYNCALL_FOUND)
cr_link_subproject(criterion boxfort STATIC IF NOT BOXFORT_FOUND)
cr_link_subproject(criterion git2 STATIC IF NOT LIBGIT2_FOUND)
cr_link_subproject(criterion wingetopt STATIC)

if (CSPTR_FOUND)
  include_directories(${CSPTR_INCLUDE_DIRS})
  cr_link_libraries(criterion ${CSPTR_LIBRARIES})
endif ()
if (NANOMSG_FOUND)
  include_directories(${NANOMSG_INCLUDE_DIRS})
  cr_link_libraries(criterion ${NANOMSG_LIBRARIES})
endif ()
if (DYNCALL_FOUND)
  include_directories(${DYNCALL_INCLUDE_DIRS})
  cr_link_libraries(criterion ${DYNCALL_LIBRARIES})
endif ()
if (BOXFORT_FOUND)
  include_directories(${BOXFORT_INCLUDE_DIRS})
  cr_link_libraries(criterion ${BOXFORT_LIBRARIES})
endif ()
if (LIBGIT2_FOUND)
  include_directories(${LIBGIT2_INCLUDE_DIRS})
  cr_link_libraries(criterion ${LIBGIT2_LIBRARIES})
endif ()

cr_link_libraries(criterion pthread IF NOT WIN32)
cr_link_libraries(criterion rt IF HAVE_LIBRT)

# Required by nanomsg
if (STATIC_NANOMSG)
  cr_link_libraries(criterion anl IF HAVE_GETADDRINFO_A)
  cr_link_libraries(criterion ws2_32 mswsock IF WIN32)
endif ()

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

add_custom_target(uninstall
    "${CMAKE_COMMAND}" -P "${CMAKE_MODULE_PATH}/uninstall.cmake"
)

if (CTESTS)
  enable_testing()
  add_custom_target(criterion_tests)
  add_subdirectory(samples)
  add_subdirectory(test)
endif ()

if (UPLOAD_DEB)
  include (DebUpload)
endif ()
