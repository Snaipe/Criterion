
if (NOT DEFINED ENV{GCOV})
	find_program(GCOV_EXECUTABLE gcov)
else()
	find_program(GCOV_EXECUTABLE $ENV{GCOV})
endif()

#file(GLOB_RECURSE GCNO_FILES "${CMAKE_CURRENT_BINARY_DIR}/*.gcno")
if (WIN32)
  file(GLOB_RECURSE GCDA_FILES "${COV_PATH}\\*.gcda")
else ()
  file(GLOB_RECURSE GCDA_FILES "${COV_PATH}/*.gcda")
endif ()

#execute_process(
#    COMMAND ${GCOV_EXECUTABLE} -lcp ${GCNO_FILES}
#    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
#)

foreach (GCDA ${GCDA_FILES})
  get_filename_component(DIR ${GCDA} PATH)
  execute_process(
    COMMAND ${GCOV_EXECUTABLE} -lcp -o ${DIR} ${GCDA}
    WORKING_DIRECTORY ${COV_PATH}
  )
endforeach ()
