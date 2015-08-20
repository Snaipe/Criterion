#!/bin/sh -e

if [ -f $1.in ]; then
    sh -c "$1" > $1.out 2> $1.err < $1.in
else
    sh -c "$1" > $1.out 2> $1.err
fi

if [ -f $2/$1.out.expected ] && [ "$(md5sum $1.out | cut -d' ' -f1)" != "$(md5sum $2/$1.out.expected | cut -d' ' -f1)" ]; then
    diff $1.out $2/$1.out.expected
    exit 255
fi

if [ -f $2/$1.err.expected ] && [ "$(md5sum $1.err | cut -d' ' -f1)" != "$(md5sum $2/$1.err.expected | cut -d' ' -f1)" ]; then
    diff $1.err $2/$1.err.expected
    exit 255
fi
