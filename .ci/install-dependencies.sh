#!/bin/sh

git submodule update --init --recursive &&

(
    cd dependencies/dyncall &&
    mkdir build &&
    cd $_ &&
    cmake -DCMAKE_INSTALL_PREFIX=$LOCAL_INSTALL "$@" .. &&
    make &&
    make install
) &&

(
    cd dependencies/libcsptr/ &&
    mkdir build &&
    cd $_ &&
    cmake -DCMAKE_INSTALL_PREFIX=$LOCAL_INSTALL "$@" .. &&
    make &&
    make install
)
