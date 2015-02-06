#!/bin/sh
git submodule update --init --recursive
mkdir -p m4
autoreconf -i
