if (NOT MSVC)
  set(CMAKE_C_FLAGS "-std=c99 -Wall -Wextra -pedantic")
endif ()

include_directories(../include)

set(SAMPLES
  signal
  exit
  report
  suites
  fixtures
  asserts
  more-suites
  long-messages
  description
  other-crashes
  simple
  theories
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
    add_executable(${sample} ${sample}.c)
    target_link_libraries(${sample} criterion)
    add_test(${sample} ${sample})
    set_property(TEST ${sample} PROPERTY
        ENVIRONMENT "CRITERION_ALWAYS_SUCCEED=1"
    )

    if (NOT MSVC) # we disable the scripted tests when building with MSVC
        add_test(${sample}_compare sh ${CMAKE_CURRENT_LIST_DIR}/tests/run_test.sh "${CMAKE_CURRENT_LIST_DIR}" . . ${sample})
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
