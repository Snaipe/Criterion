#!/bin/sh -e

cmp_dir=$1; shift
out_dir=$1; shift
bin_dir=$1; shift

mkdir -p $out_dir

if [ -f $cmp_dir/$1.in ]; then
    sh -c "$bin_dir/$*" > $out_dir/$1.out 2> $out_dir/$1.err < $cmp_dir/$1.in
else
    mkdir -p $(dirname $out_dir/$1)
    sh -c "$bin_dir/$*" > $out_dir/$1.out 2> $out_dir/$1.err
fi

if [ -f $cmp_dir/$1.out.expected ] && [ "$(md5sum $out_dir/$1.out | cut -d' ' -f1)" != "$(md5sum $cmp_dir/$1.out.expected | cut -d' ' -f1)" ]; then
    if ! diff --strip-trailing-cr $out_dir/$1.out $cmp_dir/$1.out.expected ; then
        exit 255
    fi
fi

if [ -f $cmp_dir/$1.err.expected ] && [ "$(md5sum $out_dir/$1.err | cut -d' ' -f1)" != "$(md5sum $cmp_dir/$1.err.expected | cut -d' ' -f1)" ]; then
    if ! diff --strip-trailing-cr $out_dir/$1.err $cmp_dir/$1.err.expected ; then
        exit 255
    fi
fi
