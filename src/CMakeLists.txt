# Copyright (C) 2015-2017 Franklin "Snaipe" Mathieu.
# Redistribution and use of this file is allowed according to the terms of the MIT license.
# For details see the LICENSE file distributed with Criterion.

# List sources and headers

set(SOURCE_FILES
  src/capi/specifiers.c
  src/capi/stream.c
  src/core/abort.c
  src/core/abort.h
  src/core/assert.c
  src/core/report.c
  src/core/report.h
  src/core/runner.c
  src/core/runner.h
  src/core/runner_coroutine.c
  src/core/runner_coroutine.h
  src/core/coroutine.h
  src/core/stats.c
  src/core/stats.h
  src/core/ordered-set.c
  src/core/test.c
  src/core/client.c
  src/core/client.h
  src/compat/internal.h
  src/compat/pipe.c
  src/compat/pipe.h
  src/compat/pipe-internal.h
  src/compat/section.c
  src/compat/section.h
  src/compat/strtok.c
  src/compat/strtok.h
  src/compat/process.c
  src/compat/process.h
  src/compat/path.c
  src/compat/path.h
  src/compat/mockfile.c
  src/compat/time.c
  src/compat/time.h
  src/compat/posix.h
  src/compat/alloc.c
  src/compat/alloc.h
  src/compat/processor.c
  src/compat/processor.h
  src/compat/kill.c
  src/compat/kill.h
  src/io/redirect.c
  src/io/event.c
  src/io/event.h
  src/io/file.c
  src/io/output.c
  src/io/output.h
  src/io/tap.c
  src/io/xml.c
  src/io/json.c
  src/log/logging.c
  src/log/logging.h
  src/log/normal.c
  src/string/diff.c
  src/string/fmt.c
  src/string/fmt.h
  src/string/i18n.c
  src/string/i18n.h
  src/entry/options.c
  src/entry/params.c
  src/entry/entry.c
  src/err.c
  src/err.h
  src/mutex.c
  src/mutex.h
  src/protocol/criterion.pb.c
  src/protocol/criterion.pb.h
  src/protocol/protocol.c
  src/protocol/messages.c
  src/protocol/messages.h
  src/protocol/connect.c
  src/protocol/connect.h
  src/common.h
  src/config.h
  src/string/extglobmatch.c
  src/string/extglobmatch.h
  src/string/brz.c
  src/string/brz.h
  src/string/xxd.c

  dependencies/nanopb/pb_common.c
  dependencies/nanopb/pb_common.h
  dependencies/nanopb/pb_encode.c
  dependencies/nanopb/pb_encode.h
  dependencies/nanopb/pb_decode.c
  dependencies/nanopb/pb_decode.h
)

if (THEORIES)
  set (SOURCE_FILES ${SOURCE_FILES}
    src/core/theories.c
  )
endif ()


set(INTERFACE_FILES
  include/criterion/abort.h
  include/criterion/alloc.h
  include/criterion/assert.h
  include/criterion/criterion.h
  include/criterion/embedded.h
  include/criterion/event.h
  include/criterion/hooks.h
  include/criterion/internal/asprintf-compat.h
  include/criterion/internal/assert.h
  include/criterion/internal/assert/complex.h
  include/criterion/internal/assert/exceptions.h
  include/criterion/internal/assert/ieee.h
  include/criterion/internal/assert/memory.h
  include/criterion/internal/assert/op.h
  include/criterion/internal/assert/op.hxx
  include/criterion/internal/assert/tag.h
  include/criterion/internal/assert/tostr.h
  include/criterion/internal/assert/types.h
  include/criterion/internal/assert/stream.h
  include/criterion/internal/capabilities.h
  include/criterion/internal/common.h
  include/criterion/internal/deprecation.h
  include/criterion/internal/designated-initializer-compat.h
  include/criterion/internal/hooks.h
  include/criterion/internal/new_asserts.h
  include/criterion/internal/ordered-set.h
  include/criterion/internal/parameterized.h
  include/criterion/internal/preprocess.h
  include/criterion/internal/redirect.h
  include/criterion/internal/stdio_filebuf.hxx
  include/criterion/internal/stream.hxx
  include/criterion/internal/test.h
  include/criterion/internal/theories.h
  include/criterion/logging.h
  include/criterion/new/assert.h
  include/criterion/new/memory.h
  include/criterion/new/stream.h
  include/criterion/options.h
  include/criterion/output.h
  include/criterion/parameterized.h
  include/criterion/redirect.h
  include/criterion/stats.h
  include/criterion/theories.h
  include/criterion/types.h
)

if (THEORIES)
  set(INTERFACE_FILES ${INTERFACE_FILES}
    include/criterion/theories.h
    include/criterion/internal/theories.h
  )
endif()

# Generate the configure file

configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/config.h.in"
  "${CMAKE_CURRENT_BINARY_DIR}/config.h"
)

configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/criterion.pc.in"
  "${CMAKE_CURRENT_BINARY_DIR}/criterion.pc"
  @ONLY
)

install (FILES "${CMAKE_CURRENT_BINARY_DIR}/criterion.pc" DESTINATION "share/pkgconfig")

set (SOURCE_FILES ${SOURCE_FILES} PARENT_SCOPE)
set (INTERFACE_FILES ${INTERFACE_FILES} PARENT_SCOPE)
