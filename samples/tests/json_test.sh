#!/bin/sh
./simple.c.bin --json --always-succeed
./signal.c.bin --json --always-succeed
./asserts.c.bin --json --always-succeed
./more-suites.c.bin --json --always-succeed
./tests/long-messages.c.bin --json --always-succeed
./description.c.bin --json --always-succeed
