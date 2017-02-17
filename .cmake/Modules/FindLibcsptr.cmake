# Copyright (C) 2015-2016 Franklin "Snaipe" Mathieu.
# Redistribution and use of this file is allowed according to the terms of the MIT license.
# For details see the LICENSE file distributed with Criterion.

# - Find libcsptr
# Find the native libcsptr headers and libraries.
#
# CSPTR_INCLUDE_DIRS	- where to find smart_ptr.h, etc.
# CSPTR_LIBRARIES	- List of libraries when using libcsptr.
# CSPTR_FOUND	- True if libcsptr has been found.

# Look for the header file.
FIND_PATH(CSPTR_INCLUDE_DIR csptr/smart_ptr.h PATH_SUFFIXES csptr)

# Look for the library.
FIND_LIBRARY(CSPTR_LIBRARY NAMES csptr)

# Handle the QUIETLY and REQUIRED arguments and set CSPTR_FOUND to TRUE if all listed variables are TRUE.
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(CSPTR DEFAULT_MSG CSPTR_LIBRARY CSPTR_INCLUDE_DIR)

# Copy the results to the output variables.
IF(CSPTR_FOUND)
    SET(CSPTR_LIBRARIES ${CSPTR_LIBRARY})
    SET(CSPTR_INCLUDE_DIRS ${CSPTR_INCLUDE_DIR})
ELSE(CSPTR_FOUND)
    SET(CSPTR_LIBRARIES)
    SET(CSPTR_INCLUDE_DIRS)
ENDIF(CSPTR_FOUND)

MARK_AS_ADVANCED(CSPTR_INCLUDE_DIRS CSPTR_LIBRARIES)
