cmake_minimum_required(VERSION 3.2.2)
cmake_policy(SET CMP0048 NEW)

project(Criterion VERSION 1.2.2)
get_cmake_property(_variableNames VARIABLES)
foreach (_variableName ${_variableNames})
    message(STATUS "${_variableName}=${${_variableName}}")
endforeach()

set(LOCALEDIR ${CMAKE_INSTALL_PREFIX}/share/locale)
set(GettextTranslate_ALL)
set(GettextTranslate_GMO_BINARY)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/.cmake/Modules/")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror -g -std=gnu11")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-no-undefined")

find_package(Gettext)
if (GETTEXT_FOUND)
  include(GettextTranslate)
  add_subdirectory(po)
  set(ENABLE_NLS 1)
endif ()

include(CheckLibraryExists)
CHECK_LIBRARY_EXISTS(rt clock_gettime "time.h" HAVE_CLOCK_GETTIME)

find_package(PCRE)
find_package(Libcsptr)

configure_file(
  "${CMAKE_SOURCE_DIR}/src/config.h.in"
  "${CMAKE_SOURCE_DIR}/src/config.h"
)

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
  src/extmatch.c
  src/extmatch.h
  src/main.c
)

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

add_subdirectory(samples)

include_directories(include src)
add_library(criterion SHARED ${SOURCE_FILES} ${INTERFACE_FILES})

link_directories(/usr/lib)

target_link_libraries(criterion csptr)

if (HAVE_CLOCK_GETTIME)
  target_link_libraries(criterion rt)
endif()

if (PCRE_FOUND)
  target_link_libraries(criterion pcre)
endif()

install(FILES ${INTERFACE_FILES} DESTINATION include/criterion)
install(TARGETS criterion
  LIBRARY DESTINATION lib
)

add_custom_target(uninstall
    "${CMAKE_COMMAND}" -P "${CMAKE_MODULE_PATH}/uninstall.cmake"
)
