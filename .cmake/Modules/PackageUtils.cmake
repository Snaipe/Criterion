# Copyright (C) 2015-2016 Franklin "Snaipe" Mathieu.
# Redistribution and use of this file is allowed according to the terms of the MIT license.
# For details see the LICENSE file distributed with Criterion.

include(CMakeParseArguments)

function (cr_link_package _TARGET _PKG)
  if (${_PKG}_LIB_FOUND OR ${_PKG}_FOUND)
    target_link_libraries(${_TARGET} ${${_PKG}_LIBRARIES})
    include_directories(${${_PKG}_INCLUDE_DIRS})
  endif ()
endfunction ()

function (cr_link_libraries _TARGET)
  set (multiValueArgs IF)
  cmake_parse_arguments (ARGS "" "" "${multiValueArgs}" ${ARGN})

  if (ARGS_IF)
    if (${ARGS_IF})
    else ()
      return ()
    endif ()
  endif ()

  target_link_libraries(${_TARGET} ${ARGS_UNPARSED_ARGUMENTS})
endfunction ()

function (cr_add_library _LIB)
  set (options SHARED STATIC)
  set (oneValueArgs COMPONENT)
  set (multiValueArgs SOURCES HEADERS PROPERTIES)
  cmake_parse_arguments (ARGS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  set (flags "")
  if (ARGS_SHARED)
    set (flags ${flags} SHARED)
  elseif (ARGS_STATIC)
    set (flags ${flags} STATIC)
  endif ()

  add_library(${_LIB} ${flags} ${ARGS_SOURCES} ${ARGS_HEADERS})
  set_target_properties(${_LIB} PROPERTIES ${ARGS_PROPERTIES})

  foreach (F ${INTERFACE_FILES})
    get_filename_component(DEST "${F}" PATH)
    if (ARGS_COMPONENT)
      set (install_flags COMPONENT ${ARGS_COMPONENT})
    endif ()
    install(FILES "${F}" DESTINATION "${DEST}" ${install_flags})
  endforeach ()

  install(TARGETS ${_LIB}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib ${install_flags}
  )
endfunction ()

find_package(PkgConfig)

macro (cr_find_package _PKG)
  set (options REQUIRED)
  set (oneValueArgs PKGCONFIG)
  cmake_parse_arguments (ARGS "${options}" "${oneValueArgs}" "" ${ARGN})

  string (TOUPPER "${_PKG}" _PKG_UP)

  find_package (${_PKG})
  if (NOT ${_PKG_UP}_FOUND AND ${_PKG_UP}_LIB_FOUND)
    set (${_PKG_UP}_FOUND ON)
  endif ()

  if (NOT ${_PKG_UP}_FOUND AND PKGCONFIG_FOUND)
    message (STATUS "Checking for package ${_PKG} with pkg-config")
    if (NOT ARGS_PKGCONFIG)
      set (ARGS_PKGCONFIG ${_PKG})
    endif ()
    pkg_check_modules(${_PKG_UP}_PKG ${ARGS_PKGCONFIG})

    if (${_PKG_UP}_PKG_FOUND)
      if (${_PKG_UP}_PKG_LIBRARY_DIRS)
        link_directories(${${_PKG_UP}_PKG_LIBRARY_DIRS})
      endif ()
      set (${_PKG_UP}_LIBRARIES ${${_PKG_UP}_PKG_LIBRARIES})
      set (${_PKG_UP}_INCLUDE_DIRS ${${_PKG_UP}_PKG_INCLUDE_DIRS})
      set (${_PKG_UP}_FOUND 1)
    endif()
  endif ()

  if (NOT ${_PKG_UP}_FOUND AND ARGS_REQUIRED)
    message (FATAL_ERROR "Could not find required package ${_PKG}")
  endif ()

  if (${_PKG_UP}_FOUND)
    set (HAVE_${_PKG_UP} 1)
  endif ()
endmacro ()
