set(MANIFEST "${CMAKE_CURRENT_BINARY_DIR}/install_manifest.txt")
 
if(NOT EXISTS ${MANIFEST})
  message(FATAL_ERROR "Cannot find install manifest: '${MANIFEST}'")
endif()
 
file(STRINGS ${MANIFEST} files)
foreach(file ${files})
  message(STATUS "Removing file: '${file}'")

  exec_program(
    ${CMAKE_COMMAND} ARGS "-E remove ${file}"
    OUTPUT_VARIABLE stdout
    RETURN_VALUE result
  )
  if (NOT "${result}" STREQUAL 0)
    message(FATAL_ERROR "Failed to remove file: '${file}'.")
  endif ()
endforeach ()
