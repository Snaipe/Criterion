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
	'core/assert.c',
	'core/client.c',
	'core/ordered-set.c',
	'core/report.c',
	'core/stats.c',

	'csptr/array.c',
	'csptr/mman.c',

	'entry/entry.c',
	'entry/options.c',
	'entry/params.c',
	'io/file.c',
	'io/json.c',
	'io/output.c',
	'io/redirect.c',
	'io/tap.c',
	'io/xml.c',

	'log/logging.c',
	'log/normal.c',

	'protocol/connect.c',
	'protocol/messages.c',
	'protocol/protocol.c',

	'string/brz.c',
	'string/extglobmatch.c',
	'string/fmt.c',
	'string/i18n.c',
	'string/xxd.c',
)

if get_option('filc-simple')
	sources = sources + files(
		'core/abort-filc.c',
		'core/runner-filc.c',
		'core/test-filc.c',
		'io/event-filc.c',
	)
else
	sources += files(
		'core/abort.c',
		'core/runner.c',
		'core/runner_coroutine.c',
		'core/test.c',
		'io/event.c',
	)
endif

if must_regenerate_pb
	protoc = find_program('protoc')
	protoc_gen_nanopb = find_program('protoc-gen-nanopb')

	gen_pb_py = meson.current_source_dir() + '/protocol/gen-pb.py'

	gen_pb = custom_target('gen-pb',
		input   : ['protocol/criterion.proto', 'protocol/criterion.options'],
		output  : ['criterion.pb.c', 'criterion.pb.h'],
		command : [gen_pb_py, protoc.path(), protoc_gen_nanopb.path(), '@INPUT0@', '@INPUT1@', meson.current_build_dir()])

	sources += gen_pb
else
	sources += 'protocol/criterion.pb.c'
endif

if get_option('diffs').enabled()
	sources += files('string/diff.c')
else
	sources += files('string/diff-stubs.c')
endif

if get_option('theories').enabled()
	sources += files('core/theories.c')
endif

configure_file(input         : 'config.h.in',
               output        : 'config.h',
               configuration : config)

if get_option('shared-library')
	libcriterion = both_libraries('criterion', sources,
		include_directories: [criterion_includedir],
		c_args: [
			'-D__USE_MINGW_ANSI_STDIO',
		],
		version: abi_version,
		dependencies: deps,
		install: true,
		link_args: cc.get_supported_link_arguments([
			'-Wl,--exclude-libs,ALL',
		]),
	)
	criterion_static_link = libcriterion.get_static_lib()
	criterion_shared_link = libcriterion.get_shared_lib()
else
	libcriterion = static_library('criterion', sources,
		include_directories: [criterion_includedir],
		c_args: [
			'-D__USE_MINGW_ANSI_STDIO',
		],
		dependencies: deps,
		install: true,
		link_args: cc.get_supported_link_arguments([
			'-Wl,--exclude-libs,ALL',
		]),
	)
	criterion_static_link = libcriterion
	criterion_shared_link = libcriterion
endif

criterion_compile_args = []
if get_option('filc-simple')
	criterion_compile_args += ['-DCRITERION_FILC_SIMPLE=1']
endif

criterion = declare_dependency(
	include_directories: [criterion_api],
	dependencies: deps,
	link_with: [libcriterion],
	compile_args: criterion_compile_args)

pkgconfig_rpath = ''
if get_option('rpath-in-pkgconfig').enabled() or (get_option('rpath-in-pkgconfig').auto() and prefix.startswith('/home/'))
	pkgconfig_rpath = '-Wl,-rpath,${libdir}'
endif

pkgconfig = import('pkgconfig')
pkgconfig.generate(
	name: meson.project_name(),
	description: 'A KISS, Cross platform unit testing framework for C and C++',
	url: 'https://snai.pe/git/criterion',
	libraries: [criterion, pkgconfig_rpath],
)
