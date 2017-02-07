# build a Debian package for Launchpad
set(CPACK_DEBIAN_PACKAGE_NAME "criterion")
set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
set(CPACK_DEBIAN_PACKAGE_SECTION "libs")
set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://github.com/Snaipe/Criterion")
set(CPACK_DEBIAN_BUILD_DEPENDS
  debhelper
  cmake
  gettext
  git
)

set(CPACK_DEBIAN_PACKAGE_DEPENDS)

set(CPACK_DEBIAN_CMAKE_OPTIONS)
set(CPACK_PACKAGE_DESCRIPTION_FILE "${PROJECT_SOURCE_DIR}/description.txt")

set(CPACK_DEBIAN_PACKAGE_SOURCE_COPY "${PROJECT_SOURCE_DIR}/.cmake/copy-source.sh")

set(CPACK_DEBIAN_DISTRIBUTION_NAME ubuntu)
set(CPACK_DEBIAN_DISTRIBUTION_RELEASES precise trusty vivid wily xenial)

set(DPUT_HOST "snaipewastaken-ppa")
set(DPUT_SNAPSHOT_HOST "snaipewastaken-ppa")
set(CPACK_DEBIAN_PACKAGE_DOCS "")
set(CPACK_DEBIAN_PACKAGE_INSTALL
  "/usr/lib/*.so"
  "/usr/lib/*.so.*"
  "/usr/share/locale/*"
)

set(CPACK_COMPONENTS_ALL "dev")
set(CPACK_COMPONENT_DEV_DISPLAY_NAME "Criterion library development files")
set(CPACK_COMPONENT_DEV_DESCRIPTION "These are the development files.")
set(CPACK_COMPONENT_DEV_SECTION "devel")

set(CPACK_COMPONENT_DEV_DEPENDS
  criterion
)

set(CPACK_COMPONENT_DEV_DOCS "")
set(CPACK_COMPONENT_DEV_INSTALL "/usr/include")

include (DebSourcePPA)
