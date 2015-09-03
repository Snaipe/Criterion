#!/bin/bash
repo="https://github.com/Snaipe/dyncall.git"
tag="master"

mkdir -p dependencies
git clone --branch ${tag} --depth 1 ${repo} dependencies/dyncall &&
(
    cd dependencies/dyncall &&
    mkdir build &&
    cd $_ &&
    cmake -DCMAKE_INSTALL_PREFIX=$LOCAL_INSTALL "$@" .. &&
    make &&
    make install
)
