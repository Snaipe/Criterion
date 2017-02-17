# Copyright (C) 2015-2016 Franklin "Snaipe" Mathieu.
# Redistribution and use of this file is allowed according to the terms of the MIT license.
# For details see the LICENSE file distributed with Criterion.

# - Find boxfort
# Find the native boxfort headers and libraries.
#
# BOXFORT_INCLUDE_DIRS	- where to find smart_ptr.h, etc.
# BOXFORT_LIBRARIES	- List of libraries when using boxfort.
# BOXFORT_FOUND	- True if boxfort has been found.

# Look for the header file.
FIND_PATH(BOXFORT_INCLUDE_DIR boxfort.h)

# Look for the library.
FIND_LIBRARY(BOXFORT_LIBRARY NAMES boxfort)

# Handle the QUIETLY and REQUIRED arguments and set BOXFORT_FOUND to TRUE if all listed variables are TRUE.
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(BOXFORT DEFAULT_MSG BOXFORT_LIBRARY BOXFORT_INCLUDE_DIR)

# Copy the results to the output variables.
IF(BOXFORT_FOUND)
  SET(BOXFORT_LIBRARIES ${BOXFORT_LIBRARY})
  SET(BOXFORT_INCLUDE_DIRS ${BOXFORT_INCLUDE_DIR})
ELSE(BOXFORT_FOUND)
  SET(BOXFORT_LIBRARIES)
  SET(BOXFORT_INCLUDE_DIRS)
ENDIF(BOXFORT_FOUND)

MARK_AS_ADVANCED(BOXFORT_INCLUDE_DIRS BOXFORT_LIBRARIES)

