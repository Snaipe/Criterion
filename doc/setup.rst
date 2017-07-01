Setup
=====

Prerequisites
-------------

The library is supported on Linux, OS X, FreeBSD, and Windows.

The following compilers are supported to compile both the library and the tests:

* GCC 4.9+ (Can be relaxed to GCC 4.6+ when not using C++)
* Clang 3.4+
* MSVC 14+ (Included in Visual Studio 2015 or later)

Building from source
--------------------

First, clone this repository:

.. code-block:: bash

    $ git clone --recursive https://github.com/Snaipe/Criterion

Then, run the following commands to build Criterion:

.. code-block:: bash

    $ mkdir build
    $ cd build
    $ cmake ..
    $ cmake --build .

Installing the library and language files (Linux, OS X, FreeBSD)
----------------------------------------------------------------

From the build directory created above, run with an elevated shell:

.. code-block:: bash

    $ make install

Usage
-----

To compile your tests with Criterion, you need to make sure to:

1. Add the include directory to the header search path
2. Install the library to your library search path
3. Link Criterion to your executable.

This should be all you need.
