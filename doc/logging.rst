Logging messages
================

Sometimes, it might be useful to print some output from within a test
or fixture -- and while this can be done trivially with a ``printf``,
it doesn't integrate well with the current output, nor does it work
*at all* when the process is testing a redirected stdout.

For these cases, Criterion exposes a logging facility:

.. code-block:: c

    #include <criterion/criterion.h>
    #include <criterion/logging.h>

    Test(suite_name, test_name) {
        cr_log_info("This is an informational message. They are not displayed "
            "by default.");
        cr_log_warn("This is a warning. They indicate some possible malfunction "
            "or misconfiguration in the test.");
        cr_log_error("This is an error. They indicate serious problems and "
            "are usually shown before the test is aborted.");
    }

``cr_log_info``, ``cr_log_warn`` and ``cr_log_error`` are all macros expanding
to a call to the ``cr_log`` function. All of them take a mandatory format string,
followed by optional parameters; for instance:

.. code-block:: c

    cr_log_info("%d + %d = %d", 1, 2, 3);

If using C++, the output stream objects ``info``, ``warn`` and ``error`` are
defined within the ``criterion::logging`` namespace, and can be used in
conjunction with ``operator<<``:

.. code-block:: c++

    #include <criterion/criterion.h>
    #include <criterion/logging.h>

    using criterion::logging::info;
    using criterion::logging::warn;
    using criterion::logging::error;

    Test(suite_name, test_name) {
        info << "This is an informational message. "
             << "They are not displayed by default."
             << std::flush;
        warn << "This is a warning. "
             << "They indicate some possible malfunction "
             << "or misconfiguration in the test."
             << std::flush;
        error << "This is an error. "
             << "They indicate serious problems and "
             << "are usually shown before the test is aborted."
             << std::flush;
    }

Note that empty messages are ignored, and newlines in the log message splits
the passed string into as many messages are there are lines.
