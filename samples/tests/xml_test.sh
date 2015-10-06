#!/bin/sh
./simple.c.bin --xml --always-succeed
./signal.c.bin --xml --always-succeed
./asserts.c.bin --xml --always-succeed
./more-suites.c.bin --xml --always-succeed
./long-messages.c.bin --xml --always-succeed
./description.c.bin --xml --always-succeed
