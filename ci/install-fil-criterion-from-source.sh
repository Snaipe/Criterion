#!/usr/bin/env bash
set -euo pipefail

if [ "${1:-}" = "" ] || [ "${2:-}" = "" ]; then
  echo "usage: $0 <criterion-source-dir> <install-prefix>" >&2
  exit 1
fi

criterion_src="$1"
install_prefix="$2"
shared_library="${CRITERION_FILC_SHARED_LIBRARY:-false}"

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
  -Dshared-library="$shared_library" \
  -Dtests=false \
  -Dsamples=false \
  --prefix="$install_prefix"

CC=/opt/fil/bin/filcc \
CXX=/opt/fil/bin/fil++ \
meson compile -C "$build_dir"

# Meson install can still be too broad in some environments. Install only the
# pieces the downstream project needs to compile and link tests.
mkdir -p "$install_prefix/include"
mkdir -p "$install_prefix/lib/pkgconfig"

cp -R "$criterion_src/include/criterion" "$install_prefix/include/"

criterion_lib="$build_dir/src/libcriterion.a"
if [ ! -f "$criterion_lib" ]; then
  criterion_lib="$(find "$build_dir" -type f -name 'libcriterion.a' | head -n1)"
fi
if [ -z "${criterion_lib:-}" ] || [ ! -f "$criterion_lib" ]; then
  criterion_lib="$(find "$build_dir" -type f -name 'libcriterion*.a' | head -n1)"
fi

criterion_shared_lib=""
if [ "$shared_library" = "true" ]; then
  criterion_shared_lib="$(find "$build_dir" -type f -name 'libcriterion.so*' | head -n1)"
  if [ -z "${criterion_shared_lib:-}" ] || [ ! -f "$criterion_shared_lib" ]; then
    echo "Missing Criterion shared library (libcriterion.so)" >&2
    exit 1
  fi
fi

boxfort_lib="$(find "$build_dir" -type f -name 'libboxfort*.a' | head -n1)"
nanomsg_lib="$(find "$build_dir" -type f -name 'libnanomsg*.a' | head -n1)"
nanopb_lib="$(find "$build_dir" -type f \( -name 'libprotobuf_nanopb_static.a' -o -name 'libprotobuf-nanopb-static.a' -o -name 'libprotobuf*.a' \) | head -n1)"

if [ -z "${criterion_lib:-}" ] || [ ! -f "$criterion_lib" ]; then
  echo "Missing Criterion archive" >&2
  exit 1
fi

if [ -z "$boxfort_lib" ] || [ ! -f "$boxfort_lib" ]; then
  echo "Missing BoxFort archive" >&2
  exit 1
fi

if [ -z "$nanomsg_lib" ] || [ ! -f "$nanomsg_lib" ]; then
  echo "Missing nanomsg archive" >&2
  exit 1
fi

if [ -z "$nanopb_lib" ] || [ ! -f "$nanopb_lib" ]; then
  echo "Missing nanopb archive" >&2
  exit 1
fi

cp "$criterion_lib" "$install_prefix/lib/"
if [ -n "${criterion_shared_lib:-}" ]; then
  # Preserve symlinks if Meson produced them.
  cp -P "$build_dir/src/libcriterion.so"* "$install_prefix/lib/" 2>/dev/null || true
  cp "$criterion_shared_lib" "$install_prefix/lib/"
fi
cp "$boxfort_lib" "$install_prefix/lib/"
cp "$nanomsg_lib" "$install_prefix/lib/"
cp "$nanopb_lib" "$install_prefix/lib/"

cat > "$install_prefix/lib/pkgconfig/criterion.pc" <<EOF
prefix=$install_prefix
exec_prefix=\${prefix}
libdir=\${exec_prefix}/lib
includedir=\${prefix}/include

Name: criterion
Description: Criterion unit testing framework for C/C++
Version: 2.4.3-fil-sdk
Cflags: -I\${includedir} -DCRITERION_FILC_SIMPLE=1
Libs: -L\${libdir} -lcriterion
Libs.private: -lboxfort -lnanomsg -lprotobuf_nanopb_static -lm -lrt -lpthread -lanl
EOF

echo "installed Fil-C Criterion source build to $install_prefix"
