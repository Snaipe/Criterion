#!/bin/bash
repo="https://github.com/Snaipe/libcsptr.git"
tag="v2.0.4"

mkdir dependencies
git clone --branch ${tag} --depth 1 ${repo} dependencies/libcsptr &&
(
    cd dependencies/libcsptr &&
    mkdir build &&
    cd $_ &&
    cmake -DCMAKE_INSTALL_PREFIX=$LOCAL_INSTALL "$@" .. &&
    make &&
    make install
)
