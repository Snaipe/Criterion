#!/bin/sh -e

git clone https://github.com/uncrustify/uncrustify /tmp/uncrustify
cd /tmp/uncrustify
git checkout -q 7ae7af9d3483f5f92766ec41bb3ba3efe4747494

if ! (
    mkdir build
    cd build
    CC=gcc CXX=g++ cmake -DCMAKE_INSTALL_PREFIX=$HOME ..

    make -j4
    make install
) 2>&1 >/tmp/uncrustify/make.log; then
    echo "Could not build uncrustify:"

    cat /tmp/uncrustify/make.log
    exit 1
fi

echo "Successfully installed uncrustify"
