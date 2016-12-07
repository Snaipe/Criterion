# Copyright (C) 2015-2016 Franklin "Snaipe" Mathieu.
# Redistribution and use of this file is allowed according to the terms of the MIT license.
# For details see the LICENSE file distributed with Criterion.

# Add toolchain patch number for incremental deb builds
set(PROJECT_VERSION "${PROJECT_VERSION}-2")

include (PackageConfig)
include (DebConfig)
