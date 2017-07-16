# Copyright (C) 2015-2016 Franklin "Snaipe" Mathieu.
# Redistribution and use of this file is allowed according to the terms of the MIT license.
# For details see the LICENSE file distributed with Criterion.

include(CheckPrototypeDefinition)
include(CheckLibraryExists)
include(CheckFunctionExists)
include(CheckSymbolExists)
include(CheckIncludeFile)
include(PackageUtils)

# Set definitions

if (WIN32)
  add_definitions (-D_CRT_SECURE_NO_WARNINGS=1)
  add_definitions (-DVC_EXTRALEAN)
  add_definitions (-DWIN32_LEAN_AND_MEAN)
  add_definitions (-D_WIN32_WINNT=0x600)
endif ()

# Check for packages

cr_find_package (Gettext)
cr_find_package (Libintl)

if (I18N AND GETTEXT_FOUND AND LIBINTL_LIB_FOUND)
  set (GettextTranslate_ALL 1)
  set (GettextTranslate_GMO_BINARY 1)
  set (ENABLE_NLS 1)
endif ()

list (APPEND CMAKE_REQUIRED_DEFINITIONS
  -D_GNU_SOURCE
  -D_CRT_RAND_S
  -DVC_EXTRALEAN
  -DWIN32_LEAN_AND_MEAN)

# Check for functions

check_symbol_exists(strtok_s "string.h" HAVE_STRTOK_S)
check_symbol_exists(strtok_r "string.h" HAVE_STRTOK_R)

check_library_exists (anl getaddrinfo_a "" HAVE_GETADDRINFO_A)

check_symbol_exists(funopen "stdio.h" HAVE_FUNOPEN)
check_symbol_exists(fopencookie "stdio.h" HAVE_FOPENCOOKIE)
check_symbol_exists(open_memstream "stdio.h" HAVE_OPEN_MEMSTREAM)

check_symbol_exists(getcwd "unistd.h" HAVE_GETCWD)
check_symbol_exists(GetCurrentDirectory "windows.h" HAVE_GETCURRENTDIRECTORY)

check_library_exists(rt clock_gettime "time.h" HAVE_CLOCK_GETTIME_RT)
if (HAVE_CLOCK_GETTIME_RT AND NOT HAVE_LIBRT)
  set (HAVE_LIBRT 1)
endif ()

if (NOT HAVE_CLOCK_GETTIME_RT)
  check_symbol_exists(clock_gettime "time.h" HAVE_CLOCK_GETTIME)
else ()
  set (HAVE_CLOCK_GETTIME "${HAVE_CLOCK_GETTIME_RT}" CACHE INTERNAL "Have symbol clock_gettime")
endif ()

check_symbol_exists(CLOCK_MONOTONIC_RAW "time.h" HAVE_CLOCK_MONOTONIC_RAW)
check_symbol_exists(isatty "unistd.h" HAVE_ISATTY)
check_symbol_exists(nl_langinfo "langinfo.h" HAVE_NL_LANGINFO)

# Check thread API

find_package(Threads)

if (CMAKE_USE_WIN32_THREADS_INIT)
  check_include_file("synchapi.h" HAVE_WIN32_SYNCHAPI)
  set (HAVE_WIN32_THREADS 1 CACHE INTERNAL "Have win32 threads")
elseif (CMAKE_USE_PTHREADS_INIT)
  set (HAVE_PTHREADS 1 CACHE INTERNAL "Have posix threads")
else ()
  message (FATAL_ERROR "Could not find a suitable thread library")
endif ()

# Check for C++11

if (LANG_CXX)
  enable_language(CXX)
endif ()

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

# Valgrind support

if (DEV_BUILD)
  set(ENABLE_VALGRIND_ERRORS 1)
endif ()
