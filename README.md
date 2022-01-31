<h1><img src="doc/criterion-title.png" height="96" alt="Criterion Logo" /></h1>

[![Build Status](https://api.cirrus-ci.com/github/Snaipe/Criterion.svg)](https://cirrus-ci.com/github/Snaipe/Criterion)
[![Coverage Status](https://img.shields.io/codecov/c/github/Snaipe/Criterion/bleeding.svg)](https://codecov.io/github/Snaipe/Criterion?branch=bleeding)
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](https://github.com/Snaipe/Criterion/blob/master/LICENSE)
[![Version](https://img.shields.io/github/release/Snaipe/Criterion.svg?label=version)](https://github.com/Snaipe/Criterion/releases/latest)
[![Documentation (Stable)](https://img.shields.io/badge/docs-stable-green)][online-docs]
[![Documentation (Bleeding)](https://img.shields.io/badge/docs-bleeding-orange)][online-docs-latest]

A dead-simple, yet extensible, C and C++ unit testing framework.

![Screencast](./doc/screencast.gif)

## Philosophy

Most test frameworks for C require a lot of boilerplate code to
set up tests and test suites -- you need to create a main,
then register new test suites, then register the tests within
these suites, and finally call the right functions.

This gives the user great control, at the unfortunate cost of simplicity.

Criterion follows the KISS principle, while keeping the control
the user would have with other frameworks:

* [x] C99 and C++11 compatible.
* [x] Tests are automatically registered when declared.
* [x] Implements a xUnit framework structure.
* [x] A default entry point is provided, no need to declare a main
  unless you want to do special handling.
* [x] Test are isolated in their own process, crashes and signals can be
  reported and tested.
* [x] Unified interface between C and C++: include the criterion header and it *just* works.
* [x] Supports parameterized tests and theories.
* [x] Progress and statistics can be followed in real time with report hooks.
* [x] TAP output format can be enabled with an option.
* [x] Runs on Linux, FreeBSD, macOS, and Windows (Compiling with MinGW GCC and Visual Studio 2015+).

## Downloads

### Packages

* Ubuntu (>=21.04) / Debian (>=11): `apt-get install libcriterion-dev`, on older versions:

    ```bash
    $ sudo add-apt-repository ppa:snaipewastaken/ppa
    $ sudo apt-get update
    $ sudo apt-get install criterion-dev
    ```
* Arch Linux ([AUR](https://aur.archlinux.org/packages/criterion/)): `pacaur -S criterion`
* macOS: `brew install mranno/tap/criterion`

### Binary archives

Binary releases are available [on the release page](https://github.com/Snaipe/Criterion/releases)

If you have a different platform, you can still [build the library from source](http://criterion.readthedocs.org/en/latest/setup.html#installation)

## Developer Resources

### Documentation

An online documentation is available on [ReadTheDocs][online-docs]
([PDF][pdf-docs] | [Zip][zip-docs] | [Epub][epub-docs])

### Samples

Sample tests can be found in the [sample directory][samples].

* [A simple test][sample-simple]
* [Using multiple suites][sample-suites]
* [Writing assertions][sample-asserts]
* [Adding test fixtures][sample-fixtures]
* [Tests with signals][sample-signal]
* [Using report hooks][sample-report]

### Getting help

| Channel | Description |
| --- | --- |
[![Subscribe to the mailing list](https://img.shields.io/badge/ml-criterion@freelists.org-46BC99.svg)][mailing-list] | Criterion mailing list on freelists.org
[![Join the chat at https://gitter.im/Snaipe/Criterion](https://badges.gitter.im/Join%20Chat.svg)][gitter-chan] | `Snaipe/Criterion` on gitter.im
[![Join #criterion on freenode](https://img.shields.io/badge/irc-%23criterion-46BC99.svg)][irc-chan] | `#criterion` on irc.freenode.net

### Misc

* [autotools skeleton for projects with criterion tests][autotools]
* [CMake find module for Criterion][find-module]

## Credits

Criterion is built with the following projects:

* [boxfort](https://github.com/diacritic/BoxFort)
* [debugbreak](https://github.com/scottt/debugbreak)
* [libffi](https://sourceware.org/libffi/)
* [klib](http://attractivechaos.github.io/klib/)
* [libcsptr](https://github.com/Snaipe/libcsptr)
* [nanomsg](http://nanomsg.org/)
* [nanopb](http://koti.kapsi.fi/jpa/nanopb/)

Logo made by [Paul Bouigue](http://www.cargocollective.com/pbouigue).

[online-docs]: http://criterion.readthedocs.org/
[online-docs-latest]: http://criterion.readthedocs.org/en/latest
[pdf-docs]: http://readthedocs.org/projects/criterion/downloads/pdf/latest/
[zip-docs]: http://readthedocs.org/projects/criterion/downloads/htmlzip/latest/
[epub-docs]: http://readthedocs.org/projects/criterion/downloads/epub/latest/

[samples]: ./samples/
[sample-simple]: ./samples/simple.c
[sample-suites]: ./samples/suites.c
[sample-asserts]: ./samples/asserts.c
[sample-fixtures]: ./samples/fixtures.c
[sample-signal]: ./samples/signal.c
[sample-report]: ./samples/report.c

[autotools]: ./dev/autotools
[find-module]: ./dev/FindCriterion.cmake

[irc-chan]: http://webchat.freenode.net/?channels=%23criterion&uio=MTY9dHJ1ZSYyPXRydWUmOT10cnVlJjExPTE5NQ4e
[gitter-chan]: https://gitter.im/Snaipe/Criterion?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge
[mailing-list]: http://www.freelists.org/list/criterion
