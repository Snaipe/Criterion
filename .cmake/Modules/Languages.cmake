
option(LANG_CXX "Turn on C++ support" ON)
option(LANG_OBJC "Turn on Objective-C support" OFF)

if (LANG_CXX)
  enable_language(CXX)
  if (CMAKE_CXX_COMPILER_WORKS)
    set(CXX_BRIDGE 1)
  endif ()
endif ()

if (LANG_OBJC)
  enable_language(ObjectiveC)
  if (CMAKE_ObjectiveC_COMPILER_WORKS)
    set(OBJC_BRIDGE 1)
  endif ()
endif ()
