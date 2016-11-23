# Copyright (C) 2015-2016 Franklin "Snaipe" Mathieu.
# Redistribution and use of this file is allowed according to the terms of the MIT license.
# For details see the LICENSE file distributed with Criterion.

# - Find nanomsg
# Find the native nanomsg headers and libraries.
#
# NANOMSG_INCLUDE_DIRS	- where to find smart_ptr.h, etc.
# NANOMSG_LIBRARIES	- List of libraries when using nanomsg.
# NANOMSG_FOUND	- True if nanomsg has been found.

# Look for the header file.
FIND_PATH(NANOMSG_INCLUDE_DIR nanomsg/nn.h PATH_SUFFIXES nanomsg)

# Look for the library.
FIND_LIBRARY(NANOMSG_LIBRARY NAMES nanomsg)

# Handle the QUIETLY and REQUIRED arguments and set NANOMSG_FOUND to TRUE if all listed variables are TRUE.
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(NANOMSG DEFAULT_MSG NANOMSG_LIBRARY NANOMSG_INCLUDE_DIR)

# Copy the results to the output variables.
IF(NANOMSG_FOUND)
  SET(NANOMSG_LIBRARIES ${NANOMSG_LIBRARY})
  SET(NANOMSG_INCLUDE_DIRS ${NANOMSG_INCLUDE_DIR})
ELSE(NANOMSG_FOUND)
  SET(NANOMSG_LIBRARIES)
  SET(NANOMSG_INCLUDE_DIRS)
ENDIF(NANOMSG_FOUND)

MARK_AS_ADVANCED(NANOMSG_INCLUDE_DIRS NANOMSG_LIBRARIES)
