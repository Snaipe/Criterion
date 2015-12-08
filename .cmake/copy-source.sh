#!/bin/sh

CURDIR=$(dirname $0)
SOURCE_DIR=$1; shift
DEST_DIR=$1; shift

(
  cd "$SOURCE_DIR"
  mkdir -p "$DEST_DIR"
  "$CURDIR/git-archive-all.sh" --format tar -- - | tar -x -C "$DEST_DIR"
)
