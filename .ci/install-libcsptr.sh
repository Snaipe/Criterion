#!/bin/bash
repo="https://github.com/Snaipe/libcsptr.git"
tag="v2.0.2"

mkdir dependencies
git clone --branch ${tag} --depth 1 ${repo} dependencies/libcsptr &&
(
    cd dependencies/libcsptr &&
    ./autogen.sh &&
    ./configure "$@" &&
    make &&
    {
        if command -v sudo; then
            sudo make install
        else
            make install
        fi
    }
)
rm -Rf dependencies
