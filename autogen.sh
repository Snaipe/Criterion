#!/bin/sh
git submodule update --init --recursive
gettextize
autoreconf -i
