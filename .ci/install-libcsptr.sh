#!/bin/bash
repo="https://github.com/Snaipe/libcsptr.git"
tag="v2.0.1"

mkdir dependencies
git clone --branch ${tag} --depth 1 ${repo} dependencies/libcsptr &&
(
    cd dependencies/libcsptr &&
    ./autogen.sh &&
    ./configure --prefix=/usr &&
    make &&
    sudo make install
)
rm -Rf dependencies
