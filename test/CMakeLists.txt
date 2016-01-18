if (NOT MSVC)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS_DEFAULT} -std=gnu99 -Wall -Wextra")
  if (CMAKE_CXX_COMPILER_WORKS)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_DEFAULT} ${CXX11_FLAG} -Wall -Wextra")
  endif ()
endif ()

include_directories(../include ../src)

set(TEST_SOURCES
    ordered-set.c
)

if (THEORIES)
  set(TEST_SOURCES ${TEST_SOURCES}
    asprintf.c
  )
endif ()

if (CMAKE_CXX_COMPILER_WORKS)
  set(TEST_SOURCES ${TEST_SOURCES}
    redirect.cc
  )
endif ()

add_executable(criterion_unit_tests EXCLUDE_FROM_ALL ${TEST_SOURCES})
target_link_libraries(criterion_unit_tests criterion)

add_dependencies(criterion_tests criterion_unit_tests)

add_test(criterion_unit_tests criterion_unit_tests)
set_property(TEST criterion_unit_tests PROPERTY
    ENVIRONMENT "CRITERION_NO_EARLY_EXIT=1" # for coverage
)

if (NOT MSVC) # we disable the scripted tests when building with MSVC
  add_custom_target(cram_tests)

  set(PATH_VAR
    $ENV{PATH}
    "${PROJECT_BINARY_DIR}/samples"
    "${PROJECT_BINARY_DIR}/samples/tests"
    "${PROJECT_BINARY_DIR}/external/lib"
    "${PROJECT_BINARY_DIR}/external/bin"
  )
  string (REPLACE ";" ":" PATH_VAR "${PATH_VAR}")

  set(CMD "")
  set(CMD "${CMD}PATH=${PATH_VAR} ")
  set(CMD "${CMD}LC_ALL=en_US.utf8 ")
  set(CMD "${CMD}CRITERION_ALWAYS_SUCCEED=1 ")
  set(CMD "${CMD}CRITERION_SHORT_FILENAME=1 ")
  set(CMD "${CMD}CRITERION_NO_EARLY_EXIT=1 ")
  set(CMD "${CMD}CRITERION_JOBS=1 ")
  set(CMD "${CMD}CRITERION_DISABLE_TIME_MEASUREMENTS=1 ")
  set(CMD "${CMD}cram -v ${CMAKE_CURRENT_SOURCE_DIR}/cram")
  add_custom_command(TARGET cram_tests POST_BUILD COMMAND sh -c "${CMD}")
  add_dependencies(cram_tests criterion_samples)

  add_test(cram_tests make cram_tests)

endif ()
