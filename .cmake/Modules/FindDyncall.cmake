# Copyright (C) 2015-2016 Franklin "Snaipe" Mathieu.
# Redistribution and use of this file is allowed according to the terms of the MIT license.
# For details see the LICENSE file distributed with Criterion.

# - Find dyncall
# Find the native libcsptr headers and libraries.
#
# DYNCALL_INCLUDE_DIRS	- where to find smart_ptr.h, etc.
# DYNCALL_LIBRARIES	- List of libraries when using libcsptr.
# DYNCALL_FOUND	- True if libcsptr has been found.

# Look for the header file.
FIND_PATH(DYNCALL_INCLUDE_DIR dyncall.h)

# Look for the library.
FIND_LIBRARY(DYNCALL_LIBRARY NAMES dyncall_s)

# Handle the QUIETLY and REQUIRED arguments and set DYNCALL_FOUND to TRUE if all listed variables are TRUE.
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(DYNCALL DEFAULT_MSG DYNCALL_LIBRARY DYNCALL_INCLUDE_DIR)

# Copy the results to the output variables.
IF(DYNCALL_FOUND)
    SET(DYNCALL_LIBRARIES ${DYNCALL_LIBRARY})
    SET(DYNCALL_INCLUDE_DIRS ${DYNCALL_INCLUDE_DIR})
ELSE(DYNCALL_FOUND)
    SET(DYNCALL_LIBRARIES)
    SET(DYNCALL_INCLUDE_DIRS)
ENDIF(DYNCALL_FOUND)

MARK_AS_ADVANCED(DYNCALL_INCLUDE_DIRS DYNCALL_LIBRARIES)
