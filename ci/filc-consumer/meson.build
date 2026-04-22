project('criterion-filc-consumer', 'c',
        meson_version: '>= 0.55.0',
        default_options: ['c_std=c11', 'warning_level=2'])

criterion_proj = subproject('criterion', default_options: [
  'theories=disabled',
  'diffs=disabled',
  'shared-library=false',
  'tests=false',
  'samples=false',
  'filc-simple=true',
])

criterion_dep = criterion_proj.get_variable('criterion')

consumer_tests = executable('consumer-tests', 'sample_tests.c',
  dependencies: [criterion_dep],
)

test('consumer-tests', consumer_tests)
