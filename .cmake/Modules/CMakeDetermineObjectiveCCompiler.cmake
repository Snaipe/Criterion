# Copyright (C) 2015 Franklin "Snaipe" Mathieu.
# Redistribution and use of this file is allowed according to the terms of the MIT license.
# For details see the LICENSE file distributed with Criterion.

include(${CMAKE_ROOT}/Modules/CMakeDetermineCompiler.cmake)

if("${CMAKE_GENERATOR}" MATCHES "Visual Studio")
elseif("${CMAKE_GENERATOR}" MATCHES "Xcode")
  set(CMAKE_ObjectiveC_COMPILER_XCODE_TYPE sourcecode.m)
  _cmake_find_compiler_path(ObjectiveC)
else()
  if(NOT CMAKE_ObjectiveC_COMPILER)
    set(CMAKE_ObjectiveC_COMPILER_INIT NOTFOUND)

    if(NOT CMAKE_ObjectiveC_COMPILER_INIT)
      set(CMAKE_ObjectiveC_COMPILER_LIST ${_CMAKE_TOOLCHAIN_PREFIX}gcc clang)
    endif()

    _cmake_find_compiler(ObjectiveC)
  else()
    _cmake_find_compiler_path(ObjectiveC)
  endif()
  mark_as_advanced(CMAKE_ObjectiveC_COMPILER)

  set(CMAKE_ObjectiveC_COMPILER_ID_TEST_FLAGS_FIRST)
  set(CMAKE_ObjectiveC_COMPILER_ID_TEST_FLAGS "-c")

endif()

if(NOT CMAKE_ObjectiveC_COMPILER_ID_RUN)
  set(CMAKE_ObjectiveC_COMPILER_ID_RUN 1)

  # Try to identify the compiler.
  set(CMAKE_ObjectiveC_COMPILER_ID)
  set(CMAKE_ObjectiveC_PLATFORM_ID)
  file(READ ${CMAKE_ROOT}/Modules/CMakePlatformId.h.in
    CMAKE_ObjectiveC_COMPILER_ID_PLATFORM_CONTENT)

  include(${CMAKE_ROOT}/Modules/CMakeDetermineCompilerId.cmake)

  # we use the same compiler id source than C
  CMAKE_DETERMINE_COMPILER_ID(ObjectiveC CFLAGS CMakeCCompilerId.c)

endif()

include(CMakeFindBinUtils)

# configure variables set in this file for fast reload later on
configure_file(${CMAKE_CURRENT_LIST_DIR}/CMakeObjectiveCCompiler.cmake.in
  ${CMAKE_PLATFORM_INFO_DIR}/CMakeObjectiveCCompiler.cmake
  @ONLY
  )

set(CMAKE_ObjectiveC_COMPILER_ENV_VAR "OBJCC")
