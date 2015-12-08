# Copyright (C) 2015 Franklin "Snaipe" Mathieu.
# Redistribution and use of this file is allowed according to the terms of the MIT license.
# For details see the LICENSE file distributed with Criterion.

include(CheckPrototypeDefinition)

check_prototype_definition(
  strtok_s
  "char *strtok_s(char *strToken, const char *strDelimit, char **context)"
  NULL
  "string.h"
  HAVE_STRTOK_S)
