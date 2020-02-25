full_tests = [
	'exit.c',
	'failmessages.c',
	'flood.c',
	'long-messages.c',
	'other-crashes.c',
	'with-time.c',
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
		'other-crashes.cc'
	]
	if get_option('theories').enabled()
		full_tests += ['theories_regression.cc']
	endif
endif

foreach tst : full_tests
	e = executable(tst + '.bin', tst,
			include_directories: [criterion_api],
			dependencies: [libm],
			link_with: libcriterion.get_shared_lib())
	test('full / ' + tst, e, args: '--always-succeed')
endforeach
