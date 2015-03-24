#!/bin/sh
./simple --tap --always-succeed
./signal --tap --always-succeed
./asserts --tap --always-succeed
./more-suites --tap --always-succeed
./long-messages --tap --always-succeed
