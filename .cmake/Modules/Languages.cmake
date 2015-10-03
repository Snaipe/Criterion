
if (NOT LANGUAGES)
  set(LANGUAGES "")
endif ()

string(REPLACE "," ";" LANGUAGES "${LANGUAGES}")

foreach (LANG ${LANGUAGES})
  enable_language(${LANG} OPTIONAL)
  if (CMAKE_${LANG}_COMPILER_WORKS)
      string(TOUPPER ${LANG} LANG_NAME)
      set(${LANG_NAME}_BRIDGE 1)
  endif ()
endforeach ()

