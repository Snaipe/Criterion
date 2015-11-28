cmake_minimum_required(VERSION 2.8)

project(Criterion C)

set(MODULE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/.cmake/Modules")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${MODULE_DIR})
set(LIBCSPTR_DISABLE_TESTS ON)
set(LIBCSPTR_DISABLE_COVERALLS ON)

# Content options

option(THEORIES "Activate the support for theories" ON)

# Initialization

include(Submodules)
include(Capabilities)

if (MSVC)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS=1)
endif ()

add_subdirectory(dependencies/libcsptr/ EXCLUDE_FROM_ALL)

if (THEORIES)
  add_subdirectory(dependencies/dyncall/ EXCLUDE_FROM_ALL)
  include_directories(dependencies/dyncall/dyncall/)
endif ()

include_directories(SYSTEM
  /usr/local/include
  /usr/include/GNUstep
)

include_directories(
  dependencies/libcsptr/include/
  dependencies/valgrind/include/
  dependencies/klib/
)

if (MSVC)
  add_subdirectory(dependencies/wingetopt/ EXCLUDE_FROM_ALL)
  include_directories(dependencies/wingetopt/src/)
endif ()

# Language support

include(Languages)

# Check for C++11

if (NOT MSVC AND CMAKE_CXX_COMPILER_WORKS)
  include(CheckCXXCompilerFlag)
  CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
  CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)

  if(COMPILER_SUPPORTS_CXX11)
    set(CXX11_FLAG "-std=c++11")
  elseif(COMPILER_SUPPORTS_CXX0X)
    set(CXX11_FLAG "-std=c++0x")
  else()
    message(FATAL_ERROR "Compiler ${CMAKE_CXX_COMPILER} has no C++11 support.")
  endif()
endif()

# Project setup & environment variables

set(PROJECT_VERSION "2.1.0")
set(LOCALEDIR_REL "share/locale")
set(LOCALEDIR "${CMAKE_INSTALL_PREFIX}/${LOCALEDIR_REL}")
set(GettextTranslate_ALL 1)
set(GettextTranslate_GMO_BINARY 1)

add_definitions(-DCRITERION_BUILDING_DLL=1)

set(CMAKE_C_FLAGS_DEFAULT "${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS_DEFAULT "${CMAKE_CXX_FLAGS}")

if (NOT MSVC)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror -g -std=gnu99")
  if (CMAKE_CXX_COMPILER_WORKS)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -g ${CXX11_FLAG}")
  endif ()
endif ()

if (MSVC)
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /SAFESEH:NO")
endif ()

if (WIN32 AND NOT MSVC)
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-no-undefined")
endif()

# Compilation options

option(MINGW_DEFINE_OFF_T "Define off_t and off64_t ourselves before including io.h" OFF)

# Setup coveralls

option(COVERALLS "Turn on coveralls support" OFF)
option(COVERALLS_UPLOAD "Upload the generated coveralls json" ON)
option(DEV_BUILD "Compile in developer mode" OFF)
option(CTESTS "Turn on the samples and test" ${DEV_BUILD})

if (DEV_BUILD)
  set(ENABLE_VALGRIND_ERRORS 1)
endif ()

if (COVERALLS)
  include(Coveralls)
  coveralls_turn_on_coverage()
endif()

# Find dependencies

option(I18N "Turn on internationalization" ON)

if (I18N)
  find_package(Gettext)
  find_package(Libintl)
  if (GETTEXT_FOUND AND LIBINTL_LIB_FOUND)
    include(GettextTranslate)
    add_subdirectory(po)
    set(ENABLE_NLS 1)
  endif ()
endif ()

include(CheckLibraryExists)
CHECK_LIBRARY_EXISTS(rt clock_gettime "time.h" HAVE_CLOCK_GETTIME)

find_package(PCRE)

# List sources and headers

set(SOURCE_FILES
  src/core/wrappers/wrap.h
  src/core/wrappers/wrap.c
  src/core/wrappers/wrappers.c
  src/core/abort.c
  src/core/abort.h
  src/core/report.c
  src/core/report.h
  src/core/runner.c
  src/core/runner.h
  src/core/runner_coroutine.c
  src/core/runner_coroutine.h
  src/core/coroutine.h
  src/core/worker.c
  src/core/worker.h
  src/core/stats.c
  src/core/stats.h
  src/core/ordered-set.c
  src/compat/internal.h
  src/compat/pipe.c
  src/compat/pipe.h
  src/compat/pipe-internal.h
  src/compat/section.c
  src/compat/section.h
  src/compat/process.c
  src/compat/process.h
  src/compat/basename.c
  src/compat/basename.h
  src/compat/mockfile.c
  src/compat/time.c
  src/compat/time.h
  src/compat/posix.h
  src/compat/alloc.c
  src/compat/alloc.h
  src/compat/processor.c
  src/compat/processor.h
  src/io/redirect.c
  src/io/event.c
  src/io/event.h
  src/io/asprintf.c
  src/io/file.c
  src/io/output.c
  src/io/output.h
  src/io/tap.c
  src/io/xml.c
  src/io/json.c
  src/log/logging.c
  src/log/normal.c
  src/string/i18n.c
  src/string/i18n.h
  src/entry/options.c
  src/entry/params.c
  src/entry/entry.c
  src/common.h
  src/config.h
)

if (THEORIES)
  set (SOURCE_FILES ${SOURCE_FILES}
    src/core/theories.c
  )
endif ()

if (PCRE_FOUND)
  set (SOURCE_FILES ${SOURCE_FILES}
    src/string/extmatch.c
    src/string/extmatch.h
  )
  set(HAVE_PCRE 1)
endif ()

if (CMAKE_CXX_COMPILER_WORKS)
  set(SOURCE_FILES ${SOURCE_FILES} src/core/wrappers/wrap.cc)
endif ()

if (CMAKE_ObjectiveC_COMPILER_WORKS)
  set(CMAKE_ObjectiveC_FLAGS "${CMAKE_ObjectiveC_FLAGS} ${CMAKE_C_FLAGS} -fobjc-exceptions")
  if ("${CMAKE_ObjectiveC_COMPILER_ID}" STREQUAL "Clang" AND APPLE)
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -framework Foundation")
  endif ()
  add_definitions(-D_NATIVE_OBJC_EXCEPTIONS)

  if (NOT WIN32)
    set(CMAKE_ObjectiveC_FLAGS "${CMAKE_ObjectiveC_FLAGS} -fPIC")
  endif ()
  set(SOURCE_FILES ${SOURCE_FILES} src/core/wrappers/wrap.m)
endif ()

set(INTERFACE_FILES
  include/criterion/assert.h
  include/criterion/abort.h
  include/criterion/criterion.h
  include/criterion/event.h
  include/criterion/hooks.h
  include/criterion/logging.h
  include/criterion/types.h
  include/criterion/options.h
  include/criterion/stats.h
  include/criterion/alloc.h
  include/criterion/parameterized.h
  include/criterion/redirect.h
  include/criterion/output.h
  include/criterion/internal/assert.h
  include/criterion/internal/test.h
  include/criterion/internal/common.h
  include/criterion/internal/ordered-set.h
  include/criterion/internal/asprintf-compat.h
  include/criterion/internal/designated-initializer-compat.h
  include/criterion/internal/preprocess.h
  include/criterion/internal/parameterized.h
  include/criterion/internal/stdio_filebuf.hxx
  include/criterion/internal/stream.hxx
  include/criterion/internal/hooks.h
  include/criterion/internal/redirect.h
  include/criterion/internal/stdio_filebuf.hxx
)

if (THEORIES)
  set(INTERFACE_FILES ${INTERFACE_FILES}
    include/criterion/theories.h
    include/criterion/internal/theories.h
  )
endif()

# Generate the configure file

configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/src/config.h.in"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/config.h"
)

include_directories(include src)
add_library(criterion SHARED ${SOURCE_FILES} ${INTERFACE_FILES})
target_link_libraries(criterion csptr)

if (THEORIES)
  target_link_libraries(criterion dyncall_s)
endif ()

if (MSVC)
  target_link_libraries(criterion wingetopt)
endif ()

if (HAVE_CLOCK_GETTIME)
  target_link_libraries(criterion rt)
endif()

if (PCRE_FOUND)
  target_link_libraries(criterion ${PCRE_LIBRARIES})
endif()

if (LIBINTL_LIB_FOUND)
  target_link_libraries(criterion ${LIBINTL_LIBRARIES})
  include_directories(${LIBINTL_INCLUDE_DIR})
endif()

if (COVERALLS)
  coveralls_setup("${SOURCE_FILES}" ${COVERALLS_UPLOAD})
endif()

foreach (F ${INTERFACE_FILES})
  get_filename_component(DEST "${F}" PATH)
  install(FILES "${F}" DESTINATION "${DEST}" COMPONENT dev)
endforeach ()

install(TARGETS criterion
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib COMPONENT dev
)

add_custom_target(criterion_tests)

add_custom_target(gcov
    "${CMAKE_COMMAND}"
        -DSOURCE_FILES="${SOURCE_FILES}"
        -DCOV_PATH="${CMAKE_CURRENT_BINARY_DIR}"
        -P "${CMAKE_MODULE_PATH}/Gcov.cmake"
)

if (CTESTS)
enable_testing()
add_subdirectory(samples)
add_subdirectory(test)
endif ()

# Add toolchain patch number for incremental deb builds
set(PROJECT_VERSION "${PROJECT_VERSION}-6")

include (PackageConfig)

option(UPLOAD_DEB "Upload package to launchpad" OFF)

if (UNIX AND UPLOAD_DEB)
  include (DebConfig)
endif ()
