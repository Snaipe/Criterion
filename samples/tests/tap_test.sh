#!/bin/sh
./simple.c.bin --tap --always-succeed
./signal.c.bin --tap --always-succeed
./asserts.c.bin --tap --always-succeed
./more-suites.c.bin --tap --always-succeed
./long-messages.c.bin --tap --always-succeed
./description.c.bin --tap --always-succeed
