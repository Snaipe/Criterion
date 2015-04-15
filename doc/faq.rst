F.A.Q
=====

**Q. When running the test suite in Windows' cmd.exe, the test executable
prints weird characters, how do I fix that?**

A. Windows' ``cmd.exe`` is not an unicode ANSI-compatible terminal emulator.
There are plenty of ways to fix that behaviour:

* Pass ``--ascii`` to the test suite when executing.
* Define the ``CRITERION_USE_ASCII`` environment variable to ``1``.
* Get a better terminal emulator, such as the one shipped with Git or Cygwin.

**Q. I'm having an issue with the library, what can I do ?**

A. Open a new issue on the `github issue tracker <https://github.com/Snaipe/Criterion/issues>`_,
and describe the problem you are experiencing, along with the platform you are
running criterion on.
