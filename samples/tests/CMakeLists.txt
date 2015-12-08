set(SAMPLES
  failmessages.c
  exit.c
  long-messages.c
  other-crashes.c

)

if (CMAKE_CXX_COMPILER_WORKS)
  set(SAMPLES ${SAMPLES}
    failmessages.cc
    exit.cc
    long-messages.cc
    other-crashes.cc

    exception.cc
  )
endif ()

if (THEORIES)
  set(SAMPLES ${SAMPLES} theories_regression.c)
  if (CMAKE_CXX_COMPILER_WORKS)
    set(SAMPLES ${SAMPLES} theories_regression.cc)
  endif ()
endif ()

add_samples("${CMAKE_CURRENT_LIST_DIR}" "${SAMPLES}")
