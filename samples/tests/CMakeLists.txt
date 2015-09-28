set(SAMPLES
  failmessages.c
  exit.c
  long-messages.c
  other-crashes.c
  theories_regression.c

  failmessages.cc
  exit.cc
  long-messages.cc
  other-crashes.cc
  theories_regression.cc
)

add_samples("${CMAKE_CURRENT_LIST_DIR}" "${SAMPLES}")
