api = files(
	'criterion/abort.h',
	'criterion/alloc.h',
	'criterion/assert.h',
	'criterion/criterion.h',
	'criterion/embedded.h',
	'criterion/event.h',
	'criterion/hooks.h',
	'criterion/logging.h',
	'criterion/options.h',
	'criterion/output.h',
	'criterion/parameterized.h',
	'criterion/redirect.h',
	'criterion/stats.h',
	'criterion/theories.h',
	'criterion/types.h',
)

api_new = files(
	'criterion/new/assert.h',
	'criterion/new/memory.h',
	'criterion/new/stream.h',
)

api_internal = files(
	'criterion/internal/asprintf-compat.h',
	'criterion/internal/assert.h',
	'criterion/internal/capabilities.h',
	'criterion/internal/common.h',
	'criterion/internal/deprecation.h',
	'criterion/internal/designated-initializer-compat.h',
	'criterion/internal/hooks.h',
	'criterion/internal/new_asserts.h',
	'criterion/internal/ordered-set.h',
	'criterion/internal/parameterized.h',
	'criterion/internal/preprocess.h',
	'criterion/internal/redirect.h',
	'criterion/internal/stdio_filebuf.hxx',
	'criterion/internal/stream.hxx',
	'criterion/internal/test.h',
	'criterion/internal/theories.h',
)

api_internal_assert = files(
	'criterion/internal/assert/complex.h',
	'criterion/internal/assert/exceptions.h',
	'criterion/internal/assert/ieee.h',
	'criterion/internal/assert/memory.h',
	'criterion/internal/assert/op.h',
	'criterion/internal/assert/op.hxx',
	'criterion/internal/assert/stream.h',
	'criterion/internal/assert/tag.h',
	'criterion/internal/assert/tostr.h',
	'criterion/internal/assert/types.h',
)

criterion_api = include_directories('.')

install_headers(api,                 subdir: 'criterion')
install_headers(api_new,             subdir: join_paths('criterion', 'new'))
install_headers(api_internal,        subdir: join_paths('criterion', 'internal'))
install_headers(api_internal_assert, subdir: join_paths('criterion', 'internal', 'assert'))
