# Copyright (C) 2015-2016 Franklin "Snaipe" Mathieu.
# Redistribution and use of this file is allowed according to the terms of the MIT license.
# For details see the LICENSE file distributed with Criterion.

option(LANG_CXX "Turn on C++ support" ON)
option(THEORIES "Activate the support for theories" ON)
option(MINGW_DEFINE_OFF_T "Define off_t and off64_t ourselves before including io.h" OFF)
option(COVERALLS "Turn on coveralls support" OFF)
option(COVERALLS_UPLOAD "Upload the generated coveralls json" ON)
option(DEV_BUILD "Compile in developer mode" OFF)
option(CTESTS "Turn on the samples and test" ${DEV_BUILD})
option(I18N "Turn on internationalization" ON)
option(UPLOAD_DEB "Upload package to launchpad" OFF)
