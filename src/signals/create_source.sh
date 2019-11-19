#!/bin/sh

cat << EOF > signal_names.c
#include "signals/signal_names.h"
#include <signal.h>
#include <stdio.h>

#define BUF_SIZE 10
char const * get_signal_name(int signal){
    static char str[BUF_SIZE];
    switch(signal){
EOF

for sig in `kill -l`
do
    if echo $sig | grep '^[0-9]\+$' ; then
        echo "case $sig: return \"$sig\";" >> signal_names.c
    elif ! echo "$sig" | grep -q RTMAX\*  && ! echo "$sig" | grep -q RTMIN\* ; then
        echo "case SIG$sig: return \"SIG$sig\";" >> signal_names.c
    fi
done

cat << EOF >> signal_names.c

        default:
            snprintf(str, BUF_SIZE, "%d", signal);
            return str;
    }
}
EOF
