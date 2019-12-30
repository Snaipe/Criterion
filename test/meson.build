unit_test_sources = files(
    'asprintf.c',
    'ordered-set.c',
    'path.c',
)

unit_tests = executable('unit_tests', unit_test_sources,
		include_directories: [criterion_includedir],
		link_with: libcriterion.get_static_lib())
test('unit tests', unit_tests)

sh   = find_program('sh', required: false)
cram = find_program('cram', required: false)
if sh.found() and cram.found()
	env = environment()
	env.prepend('PATH',
			join_paths(meson.build_root(), 'samples'),
			join_paths(meson.build_root(), 'samples', 'tests'))
	env.set('CRITERION_ALWAYS_SUCCEED', '1')
	env.set('CRITERION_DISABLE_TIME_MEASUREMENTS', '1')
	env.set('CRITERION_JOBS', '1')
	env.set('CRITERION_SHORT_FILENAME', '1')
	env.set('LC_ALL', 'en_US.utf8')
	env.set('MSYS2_ARG_CONV_EXCL', '--filter=')
	env.set('TERM', 'dumb')
	if has_cxx
		env.set('CXX_SUPPORT', '1')
	endif

	test('cram tests', cram,
			timeout: 120,
			args: [join_paths(meson.current_source_dir(), 'cram')],
			env: env)
endif
