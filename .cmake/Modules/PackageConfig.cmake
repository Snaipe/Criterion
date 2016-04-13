
function(extract_version version major minor patch extra)
  string(REGEX MATCH "([0-9]+)\\.([0-9]+)\\.([0-9]+)(.*)?" version_valid ${version})
  if(version_valid)
    string(REGEX REPLACE "([0-9]+)\\.([0-9]+)\\.([0-9]+)(.*)?" "\\1;\\2;\\3;\\4" VERSION_MATCHES ${version})
    list(GET VERSION_MATCHES 0 version_major)
    set(${major} ${version_major} PARENT_SCOPE)
    list(GET VERSION_MATCHES 1 version_minor)
    set(${minor} ${version_minor} PARENT_SCOPE)
    list(GET VERSION_MATCHES 2 version_patch)
    set(${patch} ${version_patch} PARENT_SCOPE)
    list(GET VERSION_MATCHES 3 version_extra)
    set(${extra} ${version_extra} PARENT_SCOPE)
  else(version_valid)
    message(AUTHOR_WARNING "Bad version ${version}; falling back to 0 (have you made an initial release?)")
    set(${major} "0" PARENT_SCOPE)
    set(${minor} "" PARENT_SCOPE)
    set(${patch} "" PARENT_SCOPE)
    set(${extra} "" PARENT_SCOPE)
  endif(version_valid)
endfunction(extract_version)

if (WIN32)
  set(CPACK_GENERATOR "ZIP")
  set(CPACK_SOURCE_GENERATOR "ZIP")
else ()
  set(CPACK_GENERATOR "TGZ")
  set(CPACK_SOURCE_GENERATOR "TGZ")
endif ()

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A KISS, modern unit testing framework for C and C++.")
set(CPACK_PACKAGE_FILE_NAME           "${CMAKE_PROJECT_NAME}-binary-${PROJECT_VERSION}")
set(CPACK_SOURCE_PACKAGE_FILE_NAME    "${CMAKE_PROJECT_NAME}-${PROJECT_VERSION}")
set(CPACK_PACKAGE_INSTALL_DIRECTORY   "${CMAKE_PROJECT_NAME}-${PROJECT_VERSION}")
set(CPACK_PACKAGE_VENDOR              "Franklin \"Snaipe\" Mathieu")
set(CPACK_PACKAGE_CONTACT             "Franklin \"Snaipe\" Mathieu <franklinmathieu@gmail.com>")
set(CPACK_PACKAGE_VERSION             "${PROJECT_VERSION}")

if (WIN32)
  # add snapshot specific versioning information
  if (CPACK_DEBIAN_PACKAGE_TYPE STREQUAL "snapshot")
    execute_process(COMMAND date +%Y%m%d%0k%0M%0S%z OUTPUT_VARIABLE SNAPSHOT_DATE_TIME)
    set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}-snapshot-${SNAPSHOT_DATE_TIME}")
    STRING(REPLACE "\n" "" CPACK_PACKAGE_VERSION ${CPACK_PACKAGE_VERSION})
  endif ()
endif ()

set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/debian.copyright")
extract_version(${PROJECT_VERSION}
    CPACK_PACKAGE_VERSION_MAJOR
    CPACK_PACKAGE_VERSION_MINOR
    CPACK_PACKAGE_VERSION_PATCH
    VERSION_EXTRA
)

set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})

file(GLOB TRASH_FILES "${PROJECT_SOURCE_DIR}/*")
set(KEEP_FILES
  "${PROJECT_SOURCE_DIR}/.cmake"
  "${PROJECT_SOURCE_DIR}/src"
  "${PROJECT_SOURCE_DIR}/include"
  "${PROJECT_SOURCE_DIR}/doc"
  "${PROJECT_SOURCE_DIR}/dev"
  "${PROJECT_SOURCE_DIR}/po"
  "${PROJECT_SOURCE_DIR}/dependencies"
  "${PROJECT_SOURCE_DIR}/CMakeLists.txt"
  "${PROJECT_SOURCE_DIR}/README.md"
  "${PROJECT_SOURCE_DIR}/CONTRIBUTING.md"
  "${PROJECT_SOURCE_DIR}/LICENSE"
  "${PROJECT_SOURCE_DIR}/ChangeLog"
  "${PROJECT_SOURCE_DIR}/description.txt"
)
list(REMOVE_ITEM TRASH_FILES ${KEEP_FILES})
# Escape any '.' characters
string(REPLACE "." "\\\\." TRASH_FILES "${TRASH_FILES}")
set(CPACK_SOURCE_IGNORE_FILES "${TRASH_FILES}")

include(CPack)
