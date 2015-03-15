#!/bin/sh
./simple --tap --always-succeed
./signal --tap --always-succeed
./asserts --tap --always-succeed
