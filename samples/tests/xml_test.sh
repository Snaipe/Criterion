#!/bin/sh
./simple.c.bin --xml --always-succeed
./signal.c.bin --xml --always-succeed
./asserts.c.bin --xml --always-succeed
./more-suites.c.bin --xml --always-succeed
./tests/long-messages.c.bin --xml --always-succeed
./description.c.bin --xml --always-succeed
