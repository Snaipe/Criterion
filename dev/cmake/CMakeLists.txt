cmake_minimum_required (VERSION 3.1.0)

project (Sample C)

# Add ./cmake to the module path
list (APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

option (ENABLE_TESTS "Build tests along with the library" ON)

# Add interface to include dirs
include_directories ("${CMAKE_CURRENT_SOURCE_DIR}/include")

# Add sources
add_subdirectory (src)

if (ENABLE_TESTS)
  # Check for criterion
  find_package (Criterion REQUIRED)

  # Add tests
  enable_testing ()
  add_subdirectory (test)
endif ()
