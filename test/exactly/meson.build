exactly = find_program('exactly', required: false)
if exactly.found()
	env = environment()
	env.prepend('PATH',
			join_paths(meson.current_build_dir(), '..', '..', 'samples'),
			join_paths(meson.current_build_dir(), '..', 'full'))
			
    suite_file = 'c.suite'
	if has_cxx
		suite_file = 'exactly.suite'
	endif
	suite_path = join_paths(meson.current_source_dir(), suite_file)

	test('exactly tests', exactly,
			timeout: 240,
			args: ['suite', suite_path],
			env: env)
endif

