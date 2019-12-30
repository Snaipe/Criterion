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
	]
endif

foreach sample : samples
	e = executable(sample + '.bin', sample,
			include_directories: [criterion_api],
			dependencies: [libm],
			link_with: libcriterion.get_shared_lib())
	test('sample / ' + sample, e, args: '--always-succeed')
endforeach
