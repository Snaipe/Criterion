if (NOT MSVC)
  set(CMAKE_C_FLAGS "-std=c99 -Wall -Wextra -pedantic")
  set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -pedantic")
endif ()

include_directories(../include)

set(SAMPLES
  signal.c
  exit.c
  report.c
  suites.c
  fixtures.c
  asserts.c
  more-suites.c
  long-messages.c
  description.c
  other-crashes.c
  simple.c
  theories.c
  tests/failmessages.c

  signal.cc
  exit.cc
  report.cc
  suites.cc
  fixtures.cc
  asserts.cc
  more-suites.cc
  long-messages.cc
  description.cc
  other-crashes.cc
  simple.cc
  theories.cc
)

set(SCRIPTS
  tap_test
  early_exit
  verbose
  list
  fail_fast
  help
)

if (HAVE_PCRE)
    set(SCRIPTS ${SCRIPTS} pattern)
endif ()

foreach(sample ${SAMPLES})
    add_executable(${sample}.bin ${sample})
    target_link_libraries(${sample}.bin criterion)
    add_test(${sample} ${sample}.bin)
    set_property(TEST ${sample} PROPERTY
        ENVIRONMENT "CRITERION_ALWAYS_SUCCEED=1"
    )

    if (NOT MSVC) # we disable the scripted tests when building with MSVC
        add_test(${sample}_compare sh ${CMAKE_CURRENT_LIST_DIR}/tests/run_test.sh "${CMAKE_CURRENT_LIST_DIR}" . . ${sample}.bin)
        set_property(TEST ${sample}_compare PROPERTY
            ENVIRONMENT "LC_ALL=en_US.utf8"
            ENVIRONMENT "CRITERION_ALWAYS_SUCCEED=1"
            ENVIRONMENT "CRITERION_SHORT_FILENAME=1"
        )
    endif ()
endforeach()

if (NOT MSVC) # we disable the scripted tests when building with MSVC

foreach(script ${SCRIPTS})
    add_test(${script} sh ${CMAKE_CURRENT_LIST_DIR}/tests/${script}.sh)
    set_property(TEST ${script} PROPERTY
        ENVIRONMENT "CRITERION_ALWAYS_SUCCEED=1"
    )

    add_test(${script}_compare sh ${CMAKE_CURRENT_LIST_DIR}/tests/run_test.sh "${CMAKE_CURRENT_LIST_DIR}" . "${CMAKE_CURRENT_LIST_DIR}" tests/${script})
    set_property(TEST ${script}_compare PROPERTY
        ENVIRONMENT "LC_ALL=en_US.utf8"
        ENVIRONMENT "CRITERION_ALWAYS_SUCCEED=1"
        ENVIRONMENT "CRITERION_SHORT_FILENAME=1"
    )
endforeach()

endif()
