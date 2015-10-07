if(CMAKE_ObjectiveC_COMPILER_FORCED)
  # The compiler configuration was forced by the user.
  # Assume the user has configured all compiler information.
  set(CMAKE_ObjectiveC_COMPILER_WORKS TRUE)
  return()
endif()

include(CMakeTestCompilerCommon)

# Remove any cached result from an older CMake version.
# We now store this in CMakeCCompiler.cmake.
unset(CMAKE_ObjectiveC_COMPILER_WORKS CACHE)

# This file is used by EnableLanguage in cmGlobalGenerator to
# determine that that selected C compiler can actually compile
# and link the most basic of programs.   If not, a fatal error
# is set and cmake stops processing commands and will not generate
# any makefiles or projects.
if(NOT CMAKE_ObjectiveC_COMPILER_WORKS)
  PrintTestCompilerStatus("ObjectiveC" "")
  file(WRITE ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeTmp/testObjectiveCCompiler.m
    "#ifdef __cplusplus\n"
    "# error \"The CMAKE_ObjectiveC_COMPILER is set to an ObjectiveC++ compiler\"\n"
    "#endif\n"
    "@interface Foo\n"
    "@end\n"
    "int main(int argc, char* argv[])\n"
    "{ (void)argv; return argc-1;}\n")
try_compile(CMAKE_ObjectiveC_COMPILER_WORKS ${CMAKE_BINARY_DIR}
    ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeTmp/testObjectiveCCompiler.m
    OUTPUT_VARIABLE __CMAKE_ObjectiveC_COMPILER_OUTPUT)
  # Move result from cache to normal variable.
  set(CMAKE_ObjectiveC_COMPILER_WORKS ${CMAKE_ObjectiveC_COMPILER_WORKS})
  unset(CMAKE_ObjectiveC_COMPILER_WORKS CACHE)
  set(ObjectiveC_TEST_WAS_RUN 1)
endif()

if(NOT CMAKE_ObjectiveC_COMPILER_WORKS)
  PrintTestCompilerStatus("ObjectiveC" " -- broken")
  file(APPEND ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeError.log
    "Determining if the ObjectiveC compiler works failed with "
    "the following output:\n${__CMAKE_ObjectiveC_COMPILER_OUTPUT}\n\n")
message(FATAL_ERROR "The ObjectiveC compiler \"${CMAKE_ObjectiveC_COMPILER}\" "
    "is not able to compile a simple test program.\nIt fails "
    "with the following output:\n ${__CMAKE_ObjectiveC_COMPILER_OUTPUT}\n\n"
    "CMake will not be able to correctly generate this project.")
else()
    if(ObjectiveC_TEST_WAS_RUN)
    PrintTestCompilerStatus("ObjectiveC" " -- works")
    file(APPEND ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeOutput.log
      "Determining if the ObjectiveC compiler works passed with "
      "the following output:\n${__CMAKE_ObjectiveC_COMPILER_OUTPUT}\n\n")
  endif()

  # Re-configure file
  configure_file(
    ${CMAKE_CURRENT_LIST_DIR}/CMakeObjectiveCCompiler.cmake.in
    ${CMAKE_PLATFORM_INFO_DIR}/CMakeObjectiveCCompiler.cmake
    @ONLY
    )
  include(${CMAKE_PLATFORM_INFO_DIR}/CMakeObjectiveCCompiler.cmake)
endif()

unset(__CMAKE_ObjectiveC_COMPILER_OUTPUT)
