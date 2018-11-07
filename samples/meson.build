samples = [
	'asserts.c',
	'description.c',
	'fixtures.c',
	'log.c',
	'more-suites.c',
	'parameterized.c',
	'redirect.c',
	'report.c',
	'signal.c',
	'simple.c',
	'skip.c',
	'suites.c',
	'theories.c',
	'timeout.c',

	'tests/exit.c',
	'tests/failmessages.c',
	'tests/flood.c',
	'tests/long-messages.c',
	'tests/other-crashes.c',
	'tests/theories_regression.c',
	'tests/with-time.c',
]

if has_cxx
	samples += [
		'asserts.cc',
		'description.cc',
		'fixtures.cc',
		'log.cc',
		'more-suites.cc',
		'parameterized.cc',
		'redirect.cc',
		'report.cc',
		'signal.cc',
		'simple.cc',
		'skip.cc',
		'suites.cc',
		'theories.cc',

		'tests/exception.cc',
		'tests/exit.cc',
		'tests/failmessages.cc',
		'tests/long-messages.cc',
		'tests/other-crashes.cc',
		'tests/theories_regression.cc',
	]
endif

foreach sample : samples
	e = executable(sample + '.bin', sample,
			include_directories: [criterion_api],
			dependencies: [libm],
			link_with: criterion)
	test(sample, e, args: '--always-succeed')
endforeach
