if (NOT MSVC)
  set(CMAKE_C_FLAGS "-std=gnu99 -Wall -Wextra")
endif ()

include_directories(../include ../src)

set(TEST_SOURCES
    ordered-set.c
)

add_executable(criterion_unit_tests EXCLUDE_FROM_ALL ${TEST_SOURCES})
target_link_libraries(criterion_unit_tests criterion)

add_dependencies(criterion_tests criterion_unit_tests)

add_test(criterion_unit_tests criterion_unit_tests)
