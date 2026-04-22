#!/usr/bin/env bash
set -euo pipefail

if [ "${1:-}" = "" ] || [ "${2:-}" = "" ]; then
  echo "usage: $0 <criterion-source-dir> <install-prefix>" >&2
  exit 1
fi

criterion_src="$1"
install_prefix="$2"

if [ ! -d "$criterion_src" ]; then
  echo "criterion source directory not found: $criterion_src" >&2
  exit 1
fi

if [ ! -x /opt/fil/bin/filcc ] || [ ! -x /opt/fil/bin/fil++ ]; then
  echo "/opt/fil Fil-C toolchain not found" >&2
  exit 1
fi

build_dir="$criterion_src/build-fil"

rm -rf "$build_dir"

CC=/opt/fil/bin/filcc \
CXX=/opt/fil/bin/fil++ \
meson setup "$build_dir" "$criterion_src" \
  --force-fallback-for=nanomsg,libgit2 \
  -Dtheories=disabled \
  -Ddiffs=disabled \
  -Dfilc-simple=true \
  -Dshared-library=false \
  -Dtests=false \
  -Dsamples=false \
  --prefix="$install_prefix"

CC=/opt/fil/bin/filcc \
CXX=/opt/fil/bin/fil++ \
meson compile -C "$build_dir" criterion

# Meson install can still be too broad in some environments. Install only the
# pieces the downstream project needs to compile and link tests.
mkdir -p "$install_prefix/include"
mkdir -p "$install_prefix/lib/pkgconfig"

cp -R "$criterion_src/include/criterion" "$install_prefix/include/"
cp "$build_dir/src/libcriterion.a" "$install_prefix/lib/"

cat > "$install_prefix/lib/pkgconfig/criterion.pc" <<EOF
prefix=$install_prefix
exec_prefix=\${prefix}
libdir=\${exec_prefix}/lib
includedir=\${prefix}/include

Name: criterion
Description: Criterion unit testing framework for C/C++
Version: 2.4.3-fil-source
Cflags: -I\${includedir}
Libs: -L\${libdir} -lcriterion
Libs.private: -lm -lrt -lpthread
EOF

echo "installed Fil-C Criterion source build to $install_prefix"
