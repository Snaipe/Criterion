# Copyright (C) 2015-2016 Franklin "Snaipe" Mathieu.
# Redistribution and use of this file is allowed according to the terms of the MIT license.
# For details see the LICENSE file distributed with Criterion.

include(ExternalProject)
include(CMakeParseArguments)

function (cr_add_subproject _NAME)
  set (options CMAKE AUTOTOOLS)
  set (oneValueArgs GIT PATH PREFIX GENERATOR)
  set (multiValueArgs OPTS IF)
  cmake_parse_arguments (ARGS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  if (ARGS_IF)
    if (${ARGS_IF})
      set (${_NAME}_SUBPROJECT_EXISTS 1 PARENT_SCOPE)
    else ()
      return ()
    endif ()
  else ()
    set (${_NAME}_SUBPROJECT_EXISTS 1 PARENT_SCOPE)
  endif ()

  if (ARGS_PREFIX)
    set (install_prefix ${CMAKE_BINARY_DIR}/external/${ARGS_PREFIX})
  else ()
    set (install_prefix ${CMAKE_BINARY_DIR}/external)
  endif ()

  if (ARGS_PATH)
    if (NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${ARGS_PATH}")
      set (ARGS_PATH "")
    else ()
      set (ARGS_GIT "")
    endif ()
  endif ()

  if (ARGS_GIT)
    string(REPLACE "#" ";" git_opts "${ARGS_GIT}")
    list(LENGTH git_opts git_opts_len)
    list(GET git_opts 0 repo)
    set (epa_opts GIT_REPOSITORY "${repo}")
    if (git_opts_len GREATER 1)
      list(GET git_opts 1 object)
      set (epa_opts ${epa_opts} GIT_TAG "${object}")
    endif ()
  elseif (ARGS_PATH)
      set (epa_opts SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/${ARGS_PATH}")
  endif ()

  if (ARGS_CMAKE)
    if (NOT ARGS_GENERATOR)
      set (ARGS_GENERATOR ${CMAKE_GENERATOR})
    endif ()
    if (CMAKE_TOOLCHAIN_FILE)
      set (ARGS_OPTS ${ARGS_OPTS}
        "-DCMAKE_SYSTEM_PROCESSOR=${CMAKE_SYSTEM_PROCESSOR}"
        "-DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}")
    endif ()
    set (build_cmds
      CONFIGURE_COMMAND ${CMAKE_COMMAND} <SOURCE_DIR>
      -DCMAKE_INSTALL_PREFIX=${install_prefix}
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -G "${ARGS_GENERATOR}"
        ${ARGS_OPTS}
      BUILD_COMMAND ${CMAKE_COMMAND} --build "${CMAKE_BINARY_DIR}/${_NAME}"
      INSTALL_COMMAND cmake -E echo "Skipping install step."
    )
    set (install_cmds
      ${CMAKE_COMMAND} --build "${CMAKE_BINARY_DIR}/${_NAME}" --target install
    )
  elseif (ARGS_AUTOTOOLS)
    set (make_opts "")
    if (ARGS_PARALLELIZED)
      set (make_opts "${make_opts} -j4")
    endif ()
    set (build_cmds
      UPDATE_COMMAND <SOURCE_DIR>/autogen.sh
      CONFIGURE_COMMAND <SOURCE_DIR>/configure --prefix=/ ${ARGS_OPTS}
      BUILD_COMMAND make ${make_opts}
      INSTALL_COMMAND cmake -E echo "Skipping install step."
    )
    set (install_cmds
      make install
    )
  endif ()

  externalproject_add(
    ${_NAME}
    ${epa_opts}

    PREFIX "${CMAKE_BINARY_DIR}/${_NAME}-build"
    BINARY_DIR "${CMAKE_BINARY_DIR}/${_NAME}"

    ${build_cmds}
  )

  externalproject_add_step(${_NAME} bootstrap
    COMMAND ${install_cmds}
    DEPENDEES build
  )
  externalproject_add_steptargets(${_NAME} bootstrap)

  set_target_properties(${_NAME} PROPERTIES EXCLUDE_FROM_ALL 1)

  add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/${_NAME}-dep
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target ${_NAME}-bootstrap
    COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_BINARY_DIR}/${_NAME}-dep
  )

  add_custom_target(${_NAME}-dependency
    DEPENDS ${CMAKE_BINARY_DIR}/${_NAME}-dep
  )

  if (WIN32)
    set ("${_NAME}_SHARED_LIB" "${install_prefix}/lib/${_NAME}.dll" PARENT_SCOPE)
    if (ARGS_GENERATOR MATCHES "(Unix|MSYS|MinGW) Makefiles")
      set ("${_NAME}_STATIC_LIB" "${install_prefix}/lib/lib${_NAME}.a" PARENT_SCOPE)
    else ()
      set ("${_NAME}_STATIC_LIB" "${install_prefix}/lib/${_NAME}.lib" PARENT_SCOPE)
    endif ()
  elseif (APPLE)
    set ("${_NAME}_SHARED_LIB" "${install_prefix}/lib/lib${_NAME}.dylib" PARENT_SCOPE)
    set ("${_NAME}_STATIC_LIB" "${install_prefix}/lib/lib${_NAME}.a" PARENT_SCOPE)
  elseif (UNIX)
    set ("${_NAME}_SHARED_LIB" "${install_prefix}/lib/lib${_NAME}.so" PARENT_SCOPE)
    set ("${_NAME}_STATIC_LIB" "${install_prefix}/lib/lib${_NAME}.a" PARENT_SCOPE)
  else ()
    message (FATAL_ERROR "Could not set proper library path for the current platform")
  endif ()

endfunction ()

function (cr_link_subproject _TARGET _SUBPROJECT)
  if (NOT ${_SUBPROJECT}_SUBPROJECT_EXISTS)
    return ()
  endif ()

  set (options STATIC SHARED)
  cmake_parse_arguments (ARGS "${options}" "" "" ${ARGN})

  add_dependencies("${_TARGET}" "${_SUBPROJECT}-dependency")
  if (ARGS_SHARED)
    target_link_libraries("${_TARGET}" "${${_SUBPROJECT}_SHARED_LIB}")
  endif ()
  if (ARGS_STATIC)
    target_link_libraries("${_TARGET}" "${${_SUBPROJECT}_STATIC_LIB}")
  endif ()
endfunction ()

include_directories(${CMAKE_BINARY_DIR}/external/include)
link_directories(${CMAKE_BINARY_DIR}/external/lib)


