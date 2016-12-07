Debugging and Coverage information
==================================

.. _gdb-ref:


Debugging with GDB
------------------

In one terminal do:

.. code-block:: bash

    $ ./simple.c.bin --debug
    Process simple.c.bin created; pid = 20803
    Listening on port 1234

In another terminal connect to this debug session:

.. code-block:: bash

    $ gdb -q ./test
    Reading symbols from ./test...done.
    (gdb) target remote localhost:1234
    Remote debugging using localhost:1234
    0x00007ffff7dd9d90 in _start() from target:/lib64/ld-linux-x86-64.so.2
    (gdb) continue
    ...
    [Inferior 1 (process 25269) exited normally]
    (gdb) q

After ``continue`` the first test is run:

.. code-block:: bash

    Remote debugging from host 127.0.0.1
    [RUN ] misc::failing
    [----] /media/data/devel/Criterion/samples/simple.c:4: Assertion failed: The expression 0 is false.
    [FAIL] misc::failing: (0,00s)

    Child exited with status 0

And a new process is created for the next test:

.. code-block:: bash

    Process /media/data/devel/Criterion/build/samples/simple.c.bin created; pid = 26414
    Listening on port 1234

Connect your remote debugger to this test with ``remote target localhost:1234``
and run the test with ``continue``

To use a different port use ``--debug --debug-transport=<protocol>:<port>``

.. _coverage-ref:

Coverage of Criterion tests
---------------------------

To use gcov, you have to compile your tests with the two GCC Options
``-fprofile-arcs`` and ``–ftest-coverage``.
