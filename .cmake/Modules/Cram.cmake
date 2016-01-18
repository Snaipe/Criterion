# Copyright (C) 2015-2016 Franklin "Snaipe" Mathieu.
# Redistribution and use of this file is allowed according to the terms of the MIT license.
# For details see the LICENSE file distributed with Criterion.

set(PATH_VAR
  "${PROJECT_BINARY_DIR}/samples"
  "${PROJECT_BINARY_DIR}/samples/tests"
  "${PROJECT_BINARY_DIR}/external/lib"
  "${PROJECT_BINARY_DIR}/external/bin"
)

set(NEW_PATH "")
foreach (P ${PATH_VAR})
  file (TO_NATIVE_PATH "${P}" P)
  list (APPEND NEW_PATH "${P}")
endforeach ()
set (PATH_VAR "${NEW_PATH}")

if (NOT WIN32)
  string (REPLACE ";" ":" PATH_VAR "${PATH_VAR}")
  set(ENV{PATH} "$ENV{PATH}:${PATH_VAR}")
else ()
  set(ENV{PATH} "$ENV{PATH};${PATH_VAR}")
endif ()

set(ENV{LC_ALL} "en_US.utf8")
set(ENV{CRITERION_ALWAYS_SUCCEED} "1")
set(ENV{CRITERION_SHORT_FILENAME} "1")
set(ENV{CRITERION_NO_EARLY_EXIT} "1")
set(ENV{CRITERION_JOBS} "1")
set(ENV{CRITERION_DISABLE_TIME_MEASUREMENTS} "1")

execute_process (COMMAND cram -v "${CRAM_PATH}" TIMEOUT 60 RESULT_VARIABLE RES)

if (NOT RES STREQUAL "0")
  message (FATAL_ERROR "Cram tests failed")
endif ()
