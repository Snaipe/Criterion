# Copyright (C) 2015 Franklin "Snaipe" Mathieu.
# Redistribution and use of this file is allowed according to the terms of the MIT license.
# For details see the LICENSE file distributed with Criterion.

# CMake <2.8.10 backward compat
if(NOT CMAKE_PLATFORM_INFO_DIR)
  set(CMAKE_PLATFORM_INFO_DIR ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY})
endif(NOT CMAKE_PLATFORM_INFO_DIR)

set(CMAKE_ObjectiveC_COMPILER_ID ${CMAKE_C_COMPILER_ID})
set(CMAKE_ObjectiveC_COMPILER ${CMAKE_C_COMPILER})

# configure variables set in this file for fast reload later on
configure_file(${CMAKE_CURRENT_LIST_DIR}/CMakeObjectiveCCompiler.cmake.in
  ${CMAKE_PLATFORM_INFO_DIR}/CMakeObjectiveCCompiler.cmake
  @ONLY
  )

set(CMAKE_ObjectiveC_COMPILER_ENV_VAR "OBJCC")
