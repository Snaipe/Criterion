add_executable (foo_test test.c)

target_link_libraries (foo_test foo ${CRITERION_LIBRARIES})

# use C99
set_property (TARGET foo_test PROPERTY C_STANDARD 99)

# Enable a few warnings
if (CMAKE_COMPILER_IS_GNUCC)
  target_compile_options (foo_test PRIVATE -Wall -Wextra)
endif (CMAKE_COMPILER_IS_GNUCC)
if (MSVC)
  target_compile_options (foo_test PRIVATE /W4)
endif (MSVC)

add_test (foo_test foo_test)
