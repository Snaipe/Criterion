Debugging and Coverage information
==================================

.. _gdb-ref:

.. note::

    The following section assumes you have the relevant debugging server
    installed on your machine. For instance, if you're debugging with gdb,
    you'll need to have ``gdbserver`` installed and available in your PATH.

Debugging with GDB
------------------

In one terminal do:

.. code-block:: bash

    $ ./test --debug=gdb
    Process test created; pid = 20803
    Listening on port 1234

.. note::

    If no argument is passed to --debug, criterion will fall back to the
    appropriate debugging server for your compiler: gdbserver with gcc,
    lldb-server with clang, windbg with msvc.

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
    [----] /path/to/test.c:4: Assertion failed: The expression 0 is false.
    [FAIL] misc::failing: (0,00s)

    Child exited with status 0

And a new process is created for the next test:

.. code-block:: bash

    Process /path/to/test created; pid = 26414
    Listening on port 1234

Connect your remote debugger to this test with ``target remote localhost:1234``
and run the test with ``continue``

To use a different port use ``--debug --debug-transport=<protocol>:<port>``

Debugging with an unsupported debugger
--------------------------------------

If you want to use a debugger that criterion doesn't natively support,
you may use the ``idle`` debugging mode: In this mode, the PID of the
test will be printed, and the test itself will suspend all operations
until a debugger resumes it.

.. code-block:: bash

    $ ./test --debug=idle
    <snip>
    [----] misc::failing: Started test has PID 30587.

On unices, once attached, simply signal the process with SIGCONT to resume
it.

.. code-block:: bash

    $ sudo gdb ./test -p 30587
    Attaching to process 30587
    0x00007f9ea3780f3d in raise () from /usr/lib/libpthread.so.0
    (gdb) signal SIGCONT
    Continuing with signal SIGCONT.

    Program received signal SIGCONT, Continued.
    0x00007f9ea3780f5f in raise () from /usr/lib/libpthread.so.0
    (gdb) c
    ...
    (gdb) q

.. _coverage-ref:

Coverage of Criterion tests
---------------------------

To use gcov, you have to compile your tests with the two GCC Options
``-fprofile-arcs`` and ``–ftest-coverage``.

Using Valgrind with Criterion
-----------------------------

Valgrind works out of the box. However, note that for all valgrind tools, you
must pass ``--trace-children=yes``, as criterion fork/execs test workers.

If you're using callgrind and ``--callgrind-out-file``, make sure you specify
``%p`` in the filename, as it will get substituted by the worker PID. If you
don't, all the test workers will overwrite the same file over and over, and
you will only get the results for the last running test.
