#!/bin/sh -e

# copy TAP driver into build-aux
automake_ver=$(automake --version | \grep -E -o '[0-9]\.[0-9]{2}')

mkdir -p build-aux
cp -f /usr/share/automake-$automake_ver/tap-driver.sh build-aux

# create criterion TAP log compiler
# this is necessary to print TAP (and only TAP) on the standard output,
# and always exit with 0 to let the TAP driver handle errors itself.
echo >build-aux/criterion-tap-test """#!/bin/sh
\$1 -Otap:- --always-succeed
"""
chmod +x build-aux/criterion-tap-test

autoreconf -vi
