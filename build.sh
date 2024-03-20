#!/usr/bin/env sh
OPT="0"
if [ $1 == "release" ]; then
    OPT="3"
fi

set -xe

# CLFAGS="-Wall -Wextra -std=c99 -pedantic"
CLFAGS="-std=c99 -pedantic"
LIBS=""

cc -O$OPT $CLFAGS defer.c -o defer.out $LIBS
