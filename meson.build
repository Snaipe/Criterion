project('criterion', 'c',
		meson_version   : '>= 0.51.0',
		license         : 'MIT',
		version         : '2.3.3',
		default_options : ['c_std=c11', 'cpp_std=c++11', 'warning_level=2'])

# standard install directories
prefix    = get_option('prefix')
localedir = get_option('localedir')

# Helper scripts
auxdir  = join_paths(meson.current_source_dir(), 'ci')
isdir   = join_paths(auxdir, 'isdir.py')

python3 = find_program('python3')
git     = find_program('git', required: false)

# Get the right version

is_git_repo = run_command([isdir, '.git']).returncode() == 0

version = 'v' + meson.project_version()
if git.found() and is_git_repo
	git_version = run_command([git.path(), 'describe', '--dirty', '--tags']).stdout().strip()
	branch = run_command([git.path(), 'rev-parse', '--abbrev-ref', 'HEAD']).stdout().strip()
	if branch == 'HEAD'
		branch = run_command([git.path(), 'rev-parse', 'HEAD']).stdout().strip()
	endif
	if git_version != ''
		version = git_version
	endif
	if branch != 'master'
		version = '@0@ (@1@)'.format(version, branch)
	endif
endif

has_cxx = add_languages('cpp', required: get_option('cxx-support'))

cc = meson.get_compiler('c')

add_project_arguments(
	cc.get_supported_arguments([
		'-Wno-unused-parameter',
		'-Wno-unused-value',
		'-fvisibility=hidden',

		# MSVC-specific stuff
		'/SAFESEH:NO',
		'/source-charset:utf-8',
	]),
	'-DCRITERION_BUILDING_DLL=1',
	'-DPB_ENABLE_MALLOC=1',
	'-D_GNU_SOURCE',
	language: ['c', 'cpp'])

if target_machine.system() == 'windows'
	add_project_arguments(
			'-DVC_EXTRALEAN',
			'-DWIN32_LEAN_AND_MEAN',
			'-D_CRT_RAND_S',
			'-D_CRT_SECURE_NO_WARNINGS=1',
			'-D_WIN32_WINNT=0x600',
			language: ['c', 'cpp'])
endif

checks = [
	{'fn': 'clock_gettime'},
	{'fn': 'fopencookie'},
	{'fn': 'funopen'},
	{'fn': 'getcwd'},
	{'fn': 'isatty'},
	{'fn': 'nl_langinfo'},
	{'fn': 'open_memstream'},
	{'fn': 'strtok_r'},
	{'fn': 'strtok_s'},

	{'sym': 'CLOCK_MONOTONIC_RAW', 'header': 'time.h'},

	{'prefix': 'Win32', 'fn':  'GetCurrentDirectory'},
	{'prefix': 'Win32', 'fn':  'PathIsRelative'},
	{'prefix': 'Win32', 'hdr': 'synchapi.h'}
]

config = configuration_data()
config.set('package', meson.project_name())
config.set('version', version)
config.set('localedir', '@0@/@1@'.format(prefix, localedir))

config.set('MINGW_DEFINE_OFF_T', get_option('mingw-define-off_t'))

check_prelude = '''
#define _GNU_SOURCE
'''

foreach check : checks
	chk_prefix = check.get('prefix', '')
	if chk_prefix != ''
		chk_prefix = chk_prefix + '_'
	endif

	result = false
	if check.has_key('fn')
		name = check.get('fn')
		result = cc.has_function(name, prefix: check_prelude)
	elif check.has_key('sym')
		name = check.get('sym')
		result = cc.has_header_symbol(check.get('header'), name, prefix: check_prelude)
	elif check.has_key('hdr')
		name = check.get('hdr')
		result = cc.has_header(name)
	endif

	name = 'HAVE_@1@@0@'.format(name.to_upper(), chk_prefix.to_upper())
	config.set(name, result)
endforeach

i18n = import('i18n')
do_i18n = get_option('i18n')

subdir('po')

if get_option('dev')
	config.set('ENABLE_VALGRIND_ERRORS', true)
endif

criterion_includedir = include_directories(
	'dependencies/debugbreak',
	'dependencies/klib',
	'dependencies/nanopb',
	'dependencies/valgrind/include',

	'include',
	'src',
)

deps = []

cmake = import('cmake')

threads = dependency('threads')

nanomsg = dependency('nanomsg', required: false)
if not nanomsg.found()
	nanomsg_proj = cmake.subproject('nanomsg',
		cmake_options: [
			'-DNN_TESTS=OFF',
			'-DNN_TOOLS=OFF',
			'-DNN_STATIC_LIB=ON',
			'-DBUILD_SHARED_LIBS=OFF',
			'-DCMAKE_POSITION_INDEPENDENT_CODE=ON',
		])
	nanomsg = nanomsg_proj.dependency('nanomsg')
	# nanomsg has a bad include file hierarchy when used from source
	config.set('NN_H', '<src/nn.h>')
	config.set('NN_REQREP_H', '<src/reqrep.h>')

	# Required by nanomsg
	deps += [
		cc.find_library('anl', required: false),
		cc.find_library('ws2_32', required: false),
		cc.find_library('mswsock', required: false),
	]

	# Required on windows to avoid symbols being dllimport-ed on a static lib
	add_project_arguments('-DNN_STATIC_LIB', language: ['c', 'cpp'])
else
	config.set('NN_H', '<nanomsg/nn.h>')
	config.set('NN_REQREP_H', '<nanomsg/reqrep.h>')
endif

libgit2 = dependency('libgit2', required: false)
if not libgit2.found()
	libgit2_proj = cmake.subproject('libgit2',
		cmake_options: [
			'-DBUILD_SHARED_LIBS=OFF',
			'-DBUILD_CLAR=OFF',
			'-DUSE_ICONV=OFF',
			'-DUSE_SSH=OFF',
			'-DUSE_GSSAPI=OFF',
			'-DUSE_OPENSSL=OFF',
			'-DVALGRIND=OFF',
			'-DCURL=OFF',
			'-DWINHTTP=OFF',
			'-DCMAKE_DISABLE_FIND_PACKAGE_HTTP_Parser=TRUE',
			'-DCMAKE_DISABLE_FIND_PACKAGE_ZLIB=TRUE',
			'-DCMAKE_DISABLE_FIND_PACKAGE_Iconv=TRUE',
			'-DCMAKE_DISABLE_FIND_PACKAGE_Security=TRUE',
			'-DCMAKE_POSITION_INDEPENDENT_CODE=ON',
		])
	libgit2 = libgit2_proj.dependency('git2')
endif

boxfort = dependency('boxfort', fallback: ['boxfort', 'boxfort'])
libffi = dependency('libffi', required: get_option('theories'), fallback: ['libffi', 'ffi_dep'])

# optional platform-dependent standard libs
librt = cc.find_library('rt', required: false)
libm  = cc.find_library('m',  required: false)

if not do_i18n.disabled()
	libintl = cc.find_library('intl', required: false)
	has_gettext_fn = cc.has_header_symbol('libintl.h', 'gettext', dependencies: [libintl], required: do_i18n)

	deps += libintl
	config.set('ENABLE_NLS', has_gettext_fn)
endif

if target_machine.system() == 'windows'
	config.set10('HAVE_WIN32_THREADS', true)
else
	config.set10('HAVE_PTHREADS', true)
endif

deps += [
	threads,
	boxfort,
	libffi,
	libgit2,
	nanomsg,
	librt,
	libm,
]

subdir('include')
subdir('src')

if get_option('samples')
	subdir('samples')
endif

if get_option('tests')
	subdir('test')
endif
