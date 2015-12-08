.. _assertions-ref:

Assertion reference
===================

This is an exhaustive list of all assertion macros that Criterion provides.

As each ``assert`` macros have an ``expect`` counterpart with the exact same
number of parameters and name suffix, there is no benefit in adding ``expect``
macros to this list. Hence only ``assert`` macros are represented here.

All ``assert`` macros may take an optional ``printf`` format string and
parameters.

Common Assertions
-----------------

=========================================================================== =========================================================================== ===========================================
Macro                                                                        Passes if and only if                                                       Notes
=========================================================================== =========================================================================== ===========================================
cr_assert(Condition, [FormatString, [Args...]])                              ``Condition`` is true.
--------------------------------------------------------------------------- --------------------------------------------------------------------------- -------------------------------------------
cr_assert_not(Condition, [FormatString, [Args...]])                          ``Condition`` is false.
--------------------------------------------------------------------------- --------------------------------------------------------------------------- -------------------------------------------
cr_assert_null(Value, [FormatString, [Args...]])                             ``Value`` is ``NULL``.
--------------------------------------------------------------------------- --------------------------------------------------------------------------- -------------------------------------------
cr_assert_not_null(Value, [FormatString, [Args...]])                         ``Value`` is not ``NULL``.
--------------------------------------------------------------------------- --------------------------------------------------------------------------- -------------------------------------------
cr_assert_eq(Actual, Expected, [FormatString, [Args...]])                    ``Actual`` is equal to ``Expected``.                                        Compatible with C++ operator overloading
--------------------------------------------------------------------------- --------------------------------------------------------------------------- -------------------------------------------
cr_assert_neq(Actual, Unexpected, [FormatString, [Args...]])                 ``Actual`` is not equal to ``Unexpected``.                                  Compatible with C++ operator overloading
--------------------------------------------------------------------------- --------------------------------------------------------------------------- -------------------------------------------
cr_assert_lt(Actual, Reference, [FormatString, [Args...]])                   ``Actual`` is less than ``Reference``.                                      Compatible with C++ operator overloading
--------------------------------------------------------------------------- --------------------------------------------------------------------------- -------------------------------------------
cr_assert_leq(Actual, Reference, [FormatString, [Args...]])                  ``Actual`` is less or equal to ``Reference``.                               Compatible with C++ operator overloading
--------------------------------------------------------------------------- --------------------------------------------------------------------------- -------------------------------------------
cr_assert_gt(Actual, Reference, [FormatString, [Args...]])                   ``Actual`` is greater than ``Reference``.                                   Compatible with C++ operator overloading
--------------------------------------------------------------------------- --------------------------------------------------------------------------- -------------------------------------------
cr_assert_geq(Actual, Reference, [FormatString, [Args...]])                  ``Actual`` is greater or equal to ``Reference``.                            Compatible with C++ operator overloading
--------------------------------------------------------------------------- --------------------------------------------------------------------------- -------------------------------------------
cr_assert_float_eq(Actual, Expected, Epsilon, [FormatString, [Args...]])     ``Actual`` is equal to ``Expected`` with a tolerance of ``Epsilon``.        Use this to test equality between floats
--------------------------------------------------------------------------- --------------------------------------------------------------------------- -------------------------------------------
cr_assert_float_neq(Actual, Unexpected, Epsilon, [FormatString, [Args...]])  ``Actual`` is not equal to ``Unexpected`` with a tolerance of ``Epsilon``.  Use this to test inequality between floats
=========================================================================== =========================================================================== ===========================================

String Assertions
-----------------

Note: these macros are meant to deal with *native* strings, i.e. char arrays.
Most of them won't work on ``std::string`` in C++, with some exceptions -- for
``std::string``, you should use regular comparison assersions, as listed above.

================================================================ =================================================================== ===========================================
Macro                                                            Passes if and only if                                               Notes
================================================================ =================================================================== ===========================================
cr_assert_str_empty(Value, [FormatString, [Args...]])            ``Value`` is an empty string.                                       Also works on std::string
---------------------------------------------------------------- ------------------------------------------------------------------- -------------------------------------------
cr_assert_str_not_empty(Value, [FormatString, [Args...]])        ``Value`` is not an empty string.                                   Also works on std::string
---------------------------------------------------------------- ------------------------------------------------------------------- -------------------------------------------
cr_assert_str_eq(Actual, Expected, [FormatString, [Args...]])    ``Actual`` is lexicographically equal to ``Expected``.
---------------------------------------------------------------- ------------------------------------------------------------------- -------------------------------------------
cr_assert_str_neq(Actual, Unexpected, [FormatString, [Args...]]) ``Actual`` is not lexicographically equal to ``Unexpected``.
---------------------------------------------------------------- ------------------------------------------------------------------- -------------------------------------------
cr_assert_str_lt(Actual, Reference, [FormatString, [Args...]])   ``Actual`` is lexicographically less than ``Reference``.
---------------------------------------------------------------- ------------------------------------------------------------------- -------------------------------------------
cr_assert_str_leq(Actual, Reference, [FormatString, [Args...]])  ``Actual`` is lexicographically less or equal to ``Reference``.
---------------------------------------------------------------- ------------------------------------------------------------------- -------------------------------------------
cr_assert_str_gt(Actual, Reference, [FormatString, [Args...]])   ``Actual`` is lexicographically greater than ``Reference``.
---------------------------------------------------------------- ------------------------------------------------------------------- -------------------------------------------
cr_assert_str_geq(Actual, Reference, [FormatString, [Args...]])  ``Actual`` is lexicographically greater or equal to ``Reference``.
================================================================ =================================================================== ===========================================

Array Assertions
-----------------

=============================================================================== =========================================================================== ===========================================
Macro                                                                            Passes if and only if                                                       Notes
=============================================================================== =========================================================================== ===========================================
cr_assert_arr_eq(Actual, Expected, [FormatString, [Args...]])                    ``Actual`` is byte-to-byte equal to ``Expected``.                           This should not be used on struct arrays,
                                                                                                                                                             consider using ``cr_assert_arr_eq_cmp``
                                                                                                                                                             instead.
------------------------------------------------------------------------------- --------------------------------------------------------------------------- -------------------------------------------
cr_assert_arr_neq(Actual, Unexpected, [FormatString, [Args...]])                 ``Actual`` is not byte-to-byte equal to ``Unexpected``.                     This should not be used on struct arrays,
                                                                                                                                                             consider using ``cr_assert_arr_neq_cmp``
                                                                                                                                                             instead.
------------------------------------------------------------------------------- --------------------------------------------------------------------------- -------------------------------------------
cr_assert_arr_eq_cmp(Actual, Expected, Size, Cmp, [FormatString, [Args...]])     ``Actual`` is comparatively equal to ``Expected``                           Only available in C++ and GNU C99
------------------------------------------------------------------------------- --------------------------------------------------------------------------- -------------------------------------------
cr_assert_arr_neq_cmp(Actual, Unexpected, Size, Cmp, [FormatString, [Args...]])  ``Actual`` is not comparatively equal to ``Expected``                       Only available in C++ and GNU C99
------------------------------------------------------------------------------- --------------------------------------------------------------------------- -------------------------------------------
cr_assert_arr_lt_cmp(Actual, Reference, Size, Cmp, [FormatString, [Args...]])    ``Actual`` is comparatively less than ``Reference``                         Only available in C++ and GNU C99
------------------------------------------------------------------------------- --------------------------------------------------------------------------- -------------------------------------------
cr_assert_arr_leq_cmp(Actual, Reference, Size, Cmp, [FormatString, [Args...]])   ``Actual`` is comparatively less or equal to ``Reference``                  Only available in C++ and GNU C99
------------------------------------------------------------------------------- --------------------------------------------------------------------------- -------------------------------------------
cr_assert_arr_gt_cmp(Actual, Reference, Size, Cmp, [FormatString, [Args...]])    ``Actual`` is comparatively greater than ``Reference``                      Only available in C++ and GNU C99
------------------------------------------------------------------------------- --------------------------------------------------------------------------- -------------------------------------------
cr_assert_arr_geq_cmp(Actual, Reference, Size, Cmp, [FormatString, [Args...]])   ``Actual`` is comparatively greater or equal to ``Reference``               Only available in C++ and GNU C99
=============================================================================== =========================================================================== ===========================================

Exception Assertions
--------------------

The following assertion macros are only defined for C++.

=============================================================================== =========================================================================== ===========================================
Macro                                                                           Passes if and only if                                                       Notes
=============================================================================== =========================================================================== ===========================================
cr_assert_throw(Statement, Exception, [FormatString, [Args...]])                ``Statement`` throws an instance of ``Exception``.
------------------------------------------------------------------------------- --------------------------------------------------------------------------- -------------------------------------------
cr_assert_no_throw(Statement, Exception, [FormatString, [Args...]])             ``Statement`` does not throws an instance of ``Exception``.
------------------------------------------------------------------------------- --------------------------------------------------------------------------- -------------------------------------------
cr_assert_any_throw(Statement, [FormatString, [Args...]])                       ``Statement`` throws any kind of exception.
------------------------------------------------------------------------------- --------------------------------------------------------------------------- -------------------------------------------
cr_assert_none_throw(Statement, [FormatString, [Args...]])                      ``Statement`` does not throw any exception.
=============================================================================== =========================================================================== ===========================================

File Assertions
---------------

=================================================================================== ============================================================================ ===========================================
Macro                                                                               Passes if and only if                                                        Notes
=================================================================================== ============================================================================ ===========================================
cr_assert_file_contents_eq_str(File, ExpectedContents, [FormatString, [Args...]])   The contents of ``File`` are equal to the string ``ExpectedContents``.
----------------------------------------------------------------------------------- ---------------------------------------------------------------------------- -------------------------------------------
cr_assert_file_contents_neq_str(File, ExpectedContents, [FormatString, [Args...]])  The contents of ``File`` are not equal to the string ``ExpectedContents``.
----------------------------------------------------------------------------------- ---------------------------------------------------------------------------- -------------------------------------------
cr_assert_stdout_eq_str(ExpectedContents, [FormatString, [Args...]])                The contents of ``stdout`` are equal to the string ``ExpectedContents``.
----------------------------------------------------------------------------------- ---------------------------------------------------------------------------- -------------------------------------------
cr_assert_stdout_neq_str(ExpectedContents, [FormatString, [Args...]])               The contents of ``stdout`` are not equal to the string ``ExpectedContents``.
----------------------------------------------------------------------------------- ---------------------------------------------------------------------------- -------------------------------------------
cr_assert_stderr_eq_str(ExpectedContents, [FormatString, [Args...]])                The contents of ``stderr`` are equal to the string ``ExpectedContents``.
----------------------------------------------------------------------------------- ---------------------------------------------------------------------------- -------------------------------------------
cr_assert_stderr_neq_str(ExpectedContents, [FormatString, [Args...]])               The contents of ``stderr`` are not equal to the string ``ExpectedContents``.
----------------------------------------------------------------------------------- ---------------------------------------------------------------------------- -------------------------------------------
cr_assert_file_contents_eq(File, RefFile, [FormatString, [Args...]])                The contents of ``File`` are equal to the contents of ``RefFile``.
----------------------------------------------------------------------------------- ---------------------------------------------------------------------------- -------------------------------------------
cr_assert_file_contents_neq(File, RefFile, [FormatString, [Args...]])               The contents of ``File`` are not equal to the contents of ``RefFile``.
----------------------------------------------------------------------------------- ---------------------------------------------------------------------------- -------------------------------------------
cr_assert_stdout_eq(RefFile, [FormatString, [Args...]])                             The contents of ``stdout`` are equal to the contents of ``RefFile``.
----------------------------------------------------------------------------------- ---------------------------------------------------------------------------- -------------------------------------------
cr_assert_stdout_neq(RefFile, [FormatString, [Args...]])                            The contents of ``stdout`` are not equal to the contents of ``RefFile``.
----------------------------------------------------------------------------------- ---------------------------------------------------------------------------- -------------------------------------------
cr_assert_stderr_eq(RefFile, [FormatString, [Args...]])                             The contents of ``stderr`` are equal to the contents of ``RefFile``.
----------------------------------------------------------------------------------- ---------------------------------------------------------------------------- -------------------------------------------
cr_assert_stderr_neq(RefFile, [FormatString, [Args...]])                            The contents of ``stderr`` are not equal to the contents of ``RefFile``.
=================================================================================== ============================================================================ ===========================================

