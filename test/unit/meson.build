unit_test_sources = files(
	'ordered-set.c',
	'path.c',
)
if get_option('theories').enabled()
    unit_test_sources += files('asprintf.c')
endif

#if has_cxx
#	unit_test_sources += files(
#		'alloc.cc',
#		'redirect.cc',
#	)
#endif

unit_tests = executable('unit_tests', unit_test_sources,
		include_directories: [criterion_includedir],
		link_with: libcriterion.get_static_lib())
test('unit tests',
	unit_tests,
	args: ['--tap'],
	protocol: 'tap')
