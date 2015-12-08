Writing tests reports in a custom format
========================================

Outputs providers are used to write tests reports in the format of your choice:
for instance, TAP and XML reporting are implemented with output providers.

Adding a custom output provider
-------------------------------

An output provider is a function with the following signature:

.. code-block:: c

    void func(FILE *out, struct criterion_global_stats *stats);

Once implemented, you then need to register it as an output provider:

.. code-block:: c

    criterion_register_output_provider("provider name", func);

This needs to be done before the test runner stops, so you may want to register
it either in a self-provided main, or in a PRE_ALL or POST_ALL report hook.

Writing to a file with an output provider
-----------------------------------------

To tell criterion to write a report to a specific file using the output provider
of your choice, you can either pass ``--output`` as a command-line
parameter:

.. code-block:: bash

    ./my_tests --output="provider name":/path/to/file

Or, you can do so directly by calling ``criterion_add_output`` before the
runner stops:

.. code-block:: c

    criterion_add_output("provider name", "/path/to/file");

The path may be relative. If ``"-"`` is passed as a filename, the report will
be written to ``stderr``.
