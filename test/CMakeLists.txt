if (NOT MSVC)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS_DEFAULT} -std=gnu99 -Wall -Wextra")
  if (CMAKE_CXX_COMPILER_WORKS)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_DEFAULT} -std=c++11 -Wall -Wextra")
  endif ()
endif ()

include_directories(../include ../src)

set(TEST_SOURCES
    ordered-set.c
    asprintf.c
)

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
