if (NOT MSVC)
  set(CMAKE_C_FLAGS "-std=gnu99 -Wall -Wextra")
  set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra")
endif ()

include_directories(../include ../src)

set(TEST_SOURCES
    ordered-set.c
    asprintf.c
    redirect.cc
)

add_executable(criterion_unit_tests EXCLUDE_FROM_ALL ${TEST_SOURCES})
target_link_libraries(criterion_unit_tests criterion)

add_dependencies(criterion_tests criterion_unit_tests)

add_test(criterion_unit_tests criterion_unit_tests)
set_property(TEST criterion_unit_tests PROPERTY
    ENVIRONMENT "CRITERION_NO_EARLY_EXIT=1" # for coverage
)
