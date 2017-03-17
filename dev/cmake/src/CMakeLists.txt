set (SOURCES
  foo.c
)

add_library (foo ${SOURCES})

# use C99
set_property(TARGET foo PROPERTY C_STANDARD 99)

# Enable a few warnings
if (CMAKE_COMPILER_IS_GNUCC)
  target_compile_options(foo PRIVATE -Wall -Wextra)
endif (CMAKE_COMPILER_IS_GNUCC)
if (MSVC)
  target_compile_options(foo PRIVATE /W4)
endif (MSVC)
