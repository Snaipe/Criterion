Introduction
============

Criterion is a dead-simple, non-intrusive testing framework for the C
programming language.

Philosophy
----------

Most test frameworks for C require a lot of boilerplate code to
set up tests and test suites -- you need to create a main,
then register new test suites, then register the tests within
these suits, and finally call the right functions.

This gives the user great control, at the unfortunate cost of simplicity.

Criterion follows the KISS principle, while keeping the control
the user would have with other frameworks.

Features
--------

* Tests are automatically registered when declared.
* A default entry point is provided, no need to declare a main
  unless you want to do special handling.
* Test are isolated in their own process, crashes and signals can be
  reported and tested.
* Progress and statistics can be followed in real time with report hooks.
