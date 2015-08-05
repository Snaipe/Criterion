Setup
=====

Prerequisites
-------------

Currently, this library only works under \*nix systems.

To compile the static library and its dependencies, GCC 4.6+ is needed.

To use the static library, any GNU-C compatible compiler will suffice
(GCC, Clang/LLVM, ICC, MinGW-GCC, ...).

Installation
------------

.. code-block:: bash

    $ git clone https://github.com/Snaipe/Criterion.git && cd Criterion
    $ LOCAL_INSTALL=/usr .ci/install-libcsptr.sh
    $ mkdir build && cd $_ && cmake -DCMAKE_INSTALL_PATH=/usr ..
    $ make && sudo make install

Usage
-----

Given a test file named test.c, compile it with `-lcriterion`:

.. code-block:: bash

    $ gcc -o test test.c -lcriterion
