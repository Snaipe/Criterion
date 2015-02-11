Setup
=====

Prerequisites
-------------

Currently, this library only works under \*nix systems.

To compile the static library and its dependencies, GCC 4.9+ is needed.

To use the static library, GCC or Clang are needed.

Installation
------------

.. code-block:: bash

    $ git clone https://github.com/Snaipe/Criterion.git
    $ cd Criterion
    $ ./autogen.sh && ./configure && make && sudo make install

Usage
-----

Given a test file named test.c, compile it with `-lcriterion`:

.. code-block:: bash

    $ gcc -o test test.c -lcriterion
