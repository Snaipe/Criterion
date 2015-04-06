#!/bin/sh
git submodule update --init --recursive
autopoint
autoreconf -i
