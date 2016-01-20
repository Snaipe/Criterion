if (NOT MSVC)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS_DEFAULT} -std=c99 -Wall -Wextra -pedantic")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_DEFAULT} ${CXX11_FLAG} -Wall -Wextra -pedantic")

  if (MINGW)
    # see http://sourceforge.net/p/mingw/bugs/2250/
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__NO_INLINE__")
  endif ()
endif ()

include_directories(../include)

set(SAMPLES
  signal.c
  report.c
  suites.c
  fixtures.c
  asserts.c
  more-suites.c
  description.c
  simple.c
  timeout.c
  redirect.c
  parameterized.c
  log.c
)

if (CMAKE_CXX_COMPILER_WORKS)
  set(SAMPLES ${SAMPLES}
    signal.cc
    report.cc
    suites.cc
    fixtures.cc
    asserts.cc
    more-suites.cc
    description.cc
    simple.cc
    redirect.cc
    parameterized.cc
  )
endif ()

if (THEORIES)
  set(SAMPLES ${SAMPLES} theories.c)
  if (CMAKE_CXX_COMPILER_WORKS)
    set(SAMPLES ${SAMPLES} theories.cc)
  endif ()
endif ()

set(SCRIPTS
  tap_test
  xml_test
  json_test
  early_exit
  verbose
  list
  fail_fast
  help
)

if (HAVE_PCRE)
    set(SCRIPTS ${SCRIPTS} pattern)
endif ()

add_custom_target(criterion_samples)
add_dependencies(criterion_tests criterion_samples)

macro(add_samples DIR_ SAMPLES_)
    foreach(sample ${SAMPLES_})
        add_executable(${sample}.bin EXCLUDE_FROM_ALL ${sample})
        add_dependencies(criterion_samples ${sample}.bin)
        target_link_libraries(${sample}.bin criterion)
        add_test(${sample} ${sample}.bin)
        set_property(TEST ${sample} PROPERTY
            ENVIRONMENT "CRITERION_ALWAYS_SUCCEED=1"
            ENVIRONMENT "CRITERION_NO_EARLY_EXIT=1" # for coverage
        )
    endforeach()
endmacro()

add_samples("${CMAKE_CURRENT_LIST_DIR}/tests" "${SAMPLES}")

add_subdirectory(tests)
