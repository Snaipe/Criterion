#!/bin/sh

CURDIR=$(dirname $0)
SOURCE_DIR=$1; shift
DEST_DIR=$1; shift

add_to_sources() {
  URL=$1; shift
  NAME=$1; shift
  HASH=$1; shift

  (
      git clone "$URL" "$DEST_DIR/$NAME"
      cd "$DEST_DIR/$NAME"
      git checkout -qf "$HASH"
      rm -Rf .git
  )
}

(
  cd "$SOURCE_DIR"
  mkdir -p "$DEST_DIR"
  "$CURDIR/git-archive-all.sh" --format tar -- - | tar -x -C "$DEST_DIR"

  add_to_sources https://github.com/Snaipe/libcsptr     dependencies/libcsptr   0d52904
  add_to_sources https://github.com/Snaipe/dyncall      dependencies/dyncall    51e79a8
  add_to_sources https://github.com/nanomsg/nanomsg     dependencies/nanomsg    7e12a20
  add_to_sources https://github.com/diacritic/BoxFort   dependencies/boxfort    926bd4c
)
