if (NOT MSVC)
  set(CMAKE_C_FLAGS "-std=gnu99 -Wall -Wextra")
endif ()

include_directories(../include ../src)

set(TEST_SOURCES
    ordered-set.c
)

add_executable(test_criterion ${TEST_SOURCES})
target_link_libraries(test_criterion criterion)

add_test(test_criterion test_criterion)
