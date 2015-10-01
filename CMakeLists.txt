cmake_minimum_required(VERSION 2.8)

project(Criterion C CXX)

set(MODULE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/.cmake/Modules")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${MODULE_DIR})
set(LIBCSPTR_DISABLE_TESTS ON)
set(LIBCSPTR_DISABLE_COVERALLS ON)

include(Submodules)

if (MSVC)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS=1)
endif ()

add_subdirectory(dependencies/libcsptr/ EXCLUDE_FROM_ALL)
add_subdirectory(dependencies/dyncall/ EXCLUDE_FROM_ALL)

include_directories(SYSTEM /usr/local/include)

include_directories(
  dependencies/libcsptr/include/
  dependencies/dyncall/dyncall/
  dependencies/valgrind/include/
)

if (MSVC)
  add_subdirectory(dependencies/wingetopt/ EXCLUDE_FROM_ALL)
  include_directories(dependencies/wingetopt/src/)
endif ()

# Project setup & environment variables

set(PROJECT_VERSION "2.1.0")
set(LOCALEDIR ${CMAKE_INSTALL_PREFIX}/share/locale)
set(GettextTranslate_ALL 1)
set(GettextTranslate_GMO_BINARY 1)

add_definitions(-DCRITERION_BUILDING_DLL=1)

if (NOT MSVC)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror -g -std=gnu99")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -g -std=c++11")
endif ()

if (MSVC)
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /SAFESEH:NO")
endif ()

if (WIN32 AND NOT MSVC)
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-no-undefined")
endif()

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

find_package(Gettext)
find_package(Libintl)
if (GETTEXT_FOUND AND LIBINTL_LIB_FOUND)
  include(GettextTranslate)
  add_subdirectory(po)
  set(ENABLE_NLS 1)
endif ()

include(CheckLibraryExists)
CHECK_LIBRARY_EXISTS(rt clock_gettime "time.h" HAVE_CLOCK_GETTIME)

find_package(PCRE)

# List sources and headers

set(SOURCE_FILES
  src/core/wrappers/wrap.c
  src/core/wrappers/wrap.cc
  src/core/abort.c
  src/core/abort.h
  src/core/report.c
  src/core/report.h
  src/core/runner.c
  src/core/runner.h
  src/core/runner_coroutine.c
  src/core/runner_coroutine.h
  src/core/worker.c
  src/core/worker.h
  src/core/stats.c
  src/core/stats.h
  src/core/ordered-set.c
  src/core/theories.c
  src/compat/internal.h
  src/compat/pipe.c
  src/compat/pipe.h
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
  src/log/logging.c
  src/log/tap.c
  src/log/normal.c
  src/string/i18n.c
  src/string/i18n.h
  src/entry/options.c
  src/entry/main.c
  src/entry/entry.c
)

if (PCRE_FOUND)
  set (SOURCE_FILES ${SOURCE_FILES}
    src/string/extmatch.c
    src/string/extmatch.h
  )
  set(HAVE_PCRE 1)
endif ()

set(INTERFACE_FILES
  include/criterion/assert.h
  include/criterion/abort.h
  include/criterion/common.h
  include/criterion/criterion.h
  include/criterion/event.h
  include/criterion/hooks.h
  include/criterion/logging.h
  include/criterion/types.h
  include/criterion/options.h
  include/criterion/ordered-set.h
  include/criterion/stats.h
  include/criterion/theories.h
  include/criterion/asprintf-compat.h
  include/criterion/designated-initializer-compat.h
  include/criterion/preprocess.h
  include/criterion/alloc.h
)

# Generate the configure file

configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/src/config.h.in"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/config.h"
)

include_directories(include src)
add_library(criterion SHARED ${SOURCE_FILES} ${INTERFACE_FILES})
target_link_libraries(criterion csptr dyncall_s)

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

install(FILES ${INTERFACE_FILES} DESTINATION include/criterion)
install(TARGETS criterion
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
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
