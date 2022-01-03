sh   = find_program('sh', required: false)
cram = find_program('cram', required: false)
if sh.found() and cram.found()
	env = environment()
	env.prepend('PATH',
			join_paths(meson.current_build_dir(), '..', '..', 'samples'),
			join_paths(meson.current_build_dir(), '..', 'full'))
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
			timeout: 240,
			workdir: join_paths(meson.current_source_dir(), '..', '..'),
			args: ['--shell=' + sh.path(), 'test/cram'],
			env: env)
endif
