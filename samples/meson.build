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
	'timeout.c',
]
if get_option('theories').enabled()
	samples += ['theories.c']
endif

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
		'suites.cc'
	]
	if get_option('theories').enabled()
		samples += ['theories.cc']
	endif
endif

foreach sample : samples
	e = executable(sample + '.bin', sample,
			include_directories: [criterion_api],
			dependencies: [libm],
			link_with: libcriterion.get_shared_lib())
	test('sample / ' + sample, e, args: '--always-succeed')
endforeach
