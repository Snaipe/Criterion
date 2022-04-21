Setup
=====

Prerequisites
-------------

The library is supported on Linux, macOS, FreeBSD, and Windows.

The following compilers are supported to compile both the library and the tests:

* GCC 4.9+ (Can be relaxed to GCC 4.6+ when not using C++)
* Clang 3.4+
* MSVC 14+ (Included in Visual Studio 2015 or later)

Building from source
--------------------

First, install dependencies:

* C/C++ compiler
* Meson, Ninja
* CMake (for subprojects)
* pkg-config
* libffi (libffi-dev)
* libgit2 (libgit2-dev)

Other runtime dependencies will be bundled if they are not available on the
system (BoxFort, debugbreak, klib, nanomsg, nanopb).

Clone this repository:

.. code-block:: bash

    $ git clone --recursive https://github.com/Snaipe/Criterion

Then, run the following commands to build Criterion:

.. code-block:: bash

    $ meson build
    $ ninja -C build

Installing the library and language files (Linux, macOS, FreeBSD)
----------------------------------------------------------------

Run with an elevated shell:

.. code-block:: bash

    $ ninja -C build install

On Linux systems, update the dynamic linker runtime bindings:

.. code-block:: bash

    $ ldconfig

Usage
-----

To compile your tests with Criterion, you need to make sure to:

1. Add the include directory to the header search path
2. Install the library to your library search path
3. Link Criterion to your executable.

This should be all you need.
