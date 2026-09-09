full_tests = [
	'combinators.c',
	'exit.c',
	'failmessages.c',
	'flood.c',
	'long-messages.c',
	'other-crashes.c',
	'with-time.c',

	# bug-specific programs
	'bug463.c',
	'bug521.c',
	'bug550.c',
	'bug594.c',
	'bug342.c',
]

if get_option('theories').enabled()
	full_tests += ['theories_regression.c']
endif

if has_cxx
	full_tests += [
		'exception.cc',
		'exit.cc',
		'failmessages.cc',
		'long-messages.cc',
		'other-crashes.cc',
	]

	if get_option('theories').enabled()
		full_tests += ['theories_regression.cc']
	endif
endif

default_timeout = executable('default-timeout.c.bin', 'default-timeout.c',
		include_directories: [criterion_api],
		link_with: libcriterion.get_shared_lib())

foreach tst : full_tests
	e = executable(tst + '.bin', tst,
			include_directories: [criterion_api],
			dependencies: [libm],
			link_with: libcriterion.get_shared_lib())
	test('full / ' + tst, e, args: '--always-succeed')
endforeach

bug584 = executable('bug584.c.bin', ['bug584.c', 'bug584-other-file.c'],
		include_directories: [criterion_api],
		link_with: libcriterion.get_shared_lib())
test('full / bug584.c', bug584, args: '--always-succeed')
