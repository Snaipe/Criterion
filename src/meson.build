sources = files(
	'err.c',
	'mutex.c',

	'capi/specifiers.c',
	'capi/stream.c',

	'compat/alloc.c',
	'compat/kill.c',
	'compat/mockfile.c',
	'compat/path.c',
	'compat/pipe.c',
	'compat/process.c',
	'compat/processor.c',
	'compat/section.c',
	'compat/strtok.c',
	'compat/time.c',

	'core/abort.c',
	'core/assert.c',
	'core/client.c',
	'core/ordered-set.c',
	'core/report.c',
	'core/runner.c',
	'core/runner_coroutine.c',
	'core/stats.c',
	'core/test.c',

	'csptr/array.c',
	'csptr/mman.c',

	'entry/entry.c',
	'entry/options.c',
	'entry/params.c',

	'io/event.c',
	'io/file.c',
	'io/json.c',
	'io/output.c',
	'io/redirect.c',
	'io/tap.c',
	'io/xml.c',

	'log/logging.c',
	'log/normal.c',

	'protocol/connect.c',
	'protocol/criterion.pb.c',
	'protocol/messages.c',
	'protocol/protocol.c',

	'string/brz.c',
	'string/diff.c',
	'string/extglobmatch.c',
	'string/fmt.c',
	'string/i18n.c',
	'string/xxd.c',

	meson.current_source_dir() + '/../dependencies/nanopb/pb_common.c',
	meson.current_source_dir() + '/../dependencies/nanopb/pb_encode.c',
	meson.current_source_dir() + '/../dependencies/nanopb/pb_decode.c',
)

if get_option('theories').enabled()
	sources += files('core/theories.c')
endif

configure_file(input         : 'config.h.in',
               output        : 'config.h',
               configuration : config)

libcriterion = both_libraries('criterion', sources,
	include_directories: [criterion_includedir],
	# extra args that we don't want to pass to tests, samples, etc
	c_args: [
		'-D__USE_MINGW_ANSI_STDIO',
	],
	version: '3.1.0',
	dependencies: deps,
	install: true,
	link_args: cc.get_supported_link_arguments([
		'-Wl,--exclude-libs,ALL',
	]),
)

criterion = declare_dependency(
	include_directories: [criterion_api],
	dependencies: deps,
	link_with: [libcriterion])

pkgconfig = import('pkgconfig')
pkgconfig.generate(
	name: meson.project_name(),
	description: 'A KISS, Cross platform unit testing framework for C and C++',
	url: 'https://snai.pe/git/criterion',
	libraries: [criterion],
)
