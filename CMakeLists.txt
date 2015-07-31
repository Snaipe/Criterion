cmake_minimum_required(VERSION 2.8)

project(Criterion C)

# Project setup & environment variables

enable_testing()
add_subdirectory(samples)

set(PROJECT_VERSION "1.2.3")
set(LOCALEDIR ${CMAKE_INSTALL_PREFIX}/share/locale)
set(GettextTranslate_ALL)
set(GettextTranslate_GMO_BINARY)
set(MODULE_DIR "${CMAKE_SOURCE_DIR}/.cmake/Modules")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${MODULE_DIR})

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror -g -std=gnu99")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-no-undefined")

# Setup coveralls

option(COVERALLS "Turn on coveralls support" OFF)
option(COVERALLS_UPLOAD "Upload the generated coveralls json" ON)

if (COVERALLS)
  include(Coveralls)
  coveralls_turn_on_coverage()
endif()

# Find dependencies

find_package(Gettext)
find_package(Libintl)
if (GETTEXT_FOUND AND Libintl_FOUND)
  include(GettextTranslate)
  add_subdirectory(po)
  set(ENABLE_NLS 1)
endif ()

include(CheckLibraryExists)
CHECK_LIBRARY_EXISTS(rt clock_gettime "time.h" HAVE_CLOCK_GETTIME)

find_package(PCRE)
find_package(Libcsptr REQUIRED)

# Generate the configure file

configure_file(
  "${CMAKE_SOURCE_DIR}/src/config.h.in"
  "${CMAKE_SOURCE_DIR}/src/config.h"
)

# List sources and headers

set(SOURCE_FILES
  src/abort.c
  src/abort.h
  src/event.c
  src/event.h
  src/report.c
  src/report.h
  src/runner.c
  src/runner.h
  src/process.c
  src/process.h
  src/stats.c
  src/stats.h
  src/log/logging.c
  src/log/tap.c
  src/log/normal.c
  src/options.c
  src/timer.c
  src/timer.h
  src/i18n.c
  src/i18n.h
  src/ordered-set.c
  src/posix-compat.c
  src/main.c
)

if (PCRE_FOUND)
  set (SOURCE_FILES ${SOURCE_FILES}
    src/extmatch.c
    src/extmatch.h
  )
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
)

include_directories(include src ${CSPTR_INCLUDE_DIRS})
add_library(criterion SHARED ${SOURCE_FILES} ${INTERFACE_FILES})
target_link_libraries(criterion ${CSPTR_LIBRARIES})

if (HAVE_CLOCK_GETTIME)
  target_link_libraries(criterion rt)
endif()

if (PCRE_FOUND)
  target_link_libraries(criterion ${PCRE_LIBRARIES})
endif()

if (LIBINTL_FOUND)
  target_link_libraries(criterion ${LIBINTL_LIBRARIES})
endif()

if (COVERALLS)
  coveralls_setup("${SOURCE_FILES}" ${COVERALLS_UPLOAD})
endif()

install(FILES ${INTERFACE_FILES} DESTINATION include/criterion)
install(TARGETS criterion
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)

add_custom_target(uninstall
    "${CMAKE_COMMAND}" -P "${CMAKE_MODULE_PATH}/uninstall.cmake"
)
