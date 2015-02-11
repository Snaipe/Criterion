
Criterion
=========

[![Build Status](https://travis-ci.org/Snaipe/Criterion.svg?branch=master)](https://travis-ci.org/Snaipe/Criterion) 
[![Coverage Status](https://coveralls.io/repos/Snaipe/Criterion/badge.svg?branch=master)](https://coveralls.io/r/Snaipe/Criterion?branch=master) 
[![License](https://img.shields.io/badge/license-MIT-blue.svg?style=flat)](https://github.com/Snaipe/Criterion/blob/master/LICENSE) 
[![Version](https://img.shields.io/github/tag/Snaipe/Criterion.svg?label=version&style=flat)](https://github.com/Snaipe/Criterion/releases)

A dead-simple, yet extensible, C test framework.

## Philosophy

Most test frameworks for C require a lot of boilerplate code to
set up tests and test suites -- you need to create a main,
then register new test suites, then register the tests within
these suits, and finally call the right functions.

This gives the user great control, at the unfortunate cost of simplicity.

Criterion follows the KISS principle, while keeping the control
the user would have with other frameworks:

* Tests are automatically registered when declared.
* A default entry point is provided, no need to declare a main
  unless you want to do special handling.
* Test are isolated in their own process, crashes and signals can be
  reported and tested.
* Progress and statistics can be followed in real time with report hooks.

## Documentation

An online documentation is available on [ReadTheDocs][online-docs] 
([PDF][pdf-docs] | [Zip][zip-docs] | [Epub][epub-docs])

## Samples

Sample tests can be found in the [sample directory][samples].

* [A simple test][sample-simple]
* [Using multiple suites][sample-suites]
* [Tests with signals][sample-signal]
* [Using report hooks][sample-report]

## F.A.Q.

**Q. What's wrong with other test frameworks?**  
A. I worked with CUnit and Check, and I must say that they do their job
   very well -- the only thing that bugs me is that setting up a test
   suite from scratch is a pain, it should really be simpler. Most
   (if not all) high-level languages have test frameworks with automatic
   test registration, but all the ones for C require you to set up a
   main, manually register suites, then tests. Criterion tries to
   fix these shortcomings.

**Q. Where has this been tested?**  
A. Currently, on Linux 2.6.32 and Linux 3.15.7, although it should work on
   most \*nix systems. More tests will be added on the build matrix.

**Q. Will this work under Windows/MSVC?**  
A. Windows support with MinGW is coming, but MSVC is a bit of a lost cause
   for the C language. The project internally uses c99 features and gnu
   extensions, and MSVC is stuck at supporting c89.

[online-docs]: http://criterion.readthedocs.org/
[pdf-docs]: http://readthedocs.org/projects/criterion/downloads/pdf/latest/
[zip-docs]: http://readthedocs.org/projects/criterion/downloads/htmlzip/latest/
[epub-docs]: http://readthedocs.org/projects/criterion/downloads/epub/latest/

[samples]: https://github.com/Snaipe/Criterion/tree/master/samples
[sample-simple]: https://github.com/Snaipe/Criterion/blob/master/samples/simple.c
[sample-suites]: https://github.com/Snaipe/Criterion/blob/master/samples/suites.c
[sample-signal]: https://github.com/Snaipe/Criterion/blob/master/samples/signal.c
[sample-report]: https://github.com/Snaipe/Criterion/blob/master/samples/report.c
