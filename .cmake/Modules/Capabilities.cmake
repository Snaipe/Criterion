# Copyright (C) 2015-2016 Franklin "Snaipe" Mathieu.
# Redistribution and use of this file is allowed according to the terms of the MIT license.
# For details see the LICENSE file distributed with Criterion.

include(CheckPrototypeDefinition)
include(CheckLibraryExists)
include(CheckFunctionExists)
include(CheckSymbolExists)
include(CheckCSourceCompiles)
include(PackageUtils)

# Check for packages

cr_find_package (Gettext)
cr_find_package (Libintl)

if (I18N AND GETTEXT_FOUND AND LIBINTL_LIB_FOUND)
  set (GettextTranslate_ALL 1)
  set (GettextTranslate_GMO_BINARY 1)
  set (ENABLE_NLS 1)
endif ()

# Check for functions

check_function_exists(strtok_s HAVE_STRTOK_S)
check_function_exists(strtok_r HAVE_STRTOK_R)

check_library_exists (anl getaddrinfo_a "" HAVE_GETADDRINFO_A)

check_function_exists(funopen HAVE_FUNOPEN)
check_function_exists(fopencookie HAVE_FOPENCOOKIE)
check_function_exists(open_memstream HAVE_OPEN_MEMSTREAM)

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

check_symbol_exists(_Unwind_ForcedUnwind "unwind.h" HAVE_UNWIND_FORCEDUNWIND)
check_symbol_exists(_Unwind_GetCFA "unwind.h" HAVE_UNWIND_GETCFA)

check_c_source_compiles("
  int main(void) {
    __builtin_dwarf_cfa();
  }
" HAVE_BUILTIN_DWARF_CFA)

check_symbol_exists(_AddressOfReturnAddress "intrin.h" HAVE_ADDRESS_OF_RETURN_ADDRESS)

if (HAVE_UNWIND_FORCEDUNWIND AND HAVE_UNWIND_GETCFA)
  set (HAVE_GNU_UNWINDER 1 CACHE INTERNAL "The unwind ABI is IA64 w/ GNU extensions")
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
