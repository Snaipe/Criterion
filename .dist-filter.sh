#!/usr/bin/env sh

for f in "${MESON_DIST_ROOT}"/subprojects/*/; do
  rm -rf "$f/.git/"
done
