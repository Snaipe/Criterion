.. _assertions-ref:

Assertion reference
===================

This is an exhaustive list of all assertion macros that Criterion provides.

As each ``assert`` macros have an ``expect`` counterpart with the exact same
number of parameters and name suffix, there is no benefit in adding ``expect``
macros to this list. Hence only ``assert`` macros are represented here.

Common Assertions
-----------------

======================================================================= =========================================================================== ===========================================
Macro                                                                   Passes if and only if                                                       Notes
======================================================================= =========================================================================== ===========================================
cr_assert(Condition, [Message, [Args...]])                              ``Condition`` is true.
----------------------------------------------------------------------- --------------------------------------------------------------------------- -------------------------------------------
cr_assert_not(Condition, [Message, [Args...]])                          ``Condition`` is false.
----------------------------------------------------------------------- --------------------------------------------------------------------------- -------------------------------------------
cr_assert_null(Value, [Message, [Args...]])                             ``Value`` is ``NULL``.
----------------------------------------------------------------------- --------------------------------------------------------------------------- -------------------------------------------
cr_assert_not_null(Value, [Message, [Args...]])                         ``Value`` is not ``NULL``.
----------------------------------------------------------------------- --------------------------------------------------------------------------- -------------------------------------------
cr_assert_eq(Actual, Expected, [Message, [Args...]])                    ``Actual`` is equal to ``Expected``.                                        Compatible with C++ operator overloading
----------------------------------------------------------------------- --------------------------------------------------------------------------- -------------------------------------------
cr_assert_neq(Actual, Unexpected, [Message, [Args...]])                 ``Actual`` is not equal to ``Unexpected``.                                  Compatible with C++ operator overloading
----------------------------------------------------------------------- --------------------------------------------------------------------------- -------------------------------------------
cr_assert_lt(Actual, Reference, [Message, [Args...]])                   ``Actual`` is less than ``Reference``.                                      Compatible with C++ operator overloading
----------------------------------------------------------------------- --------------------------------------------------------------------------- -------------------------------------------
cr_assert_leq(Actual, Reference, [Message, [Args...]])                  ``Actual`` is less or equal to ``Reference``.                               Compatible with C++ operator overloading
----------------------------------------------------------------------- --------------------------------------------------------------------------- -------------------------------------------
cr_assert_gt(Actual, Reference, [Message, [Args...]])                   ``Actual`` is greater than ``Reference``.                                   Compatible with C++ operator overloading
----------------------------------------------------------------------- --------------------------------------------------------------------------- -------------------------------------------
cr_assert_geq(Actual, Reference, [Message, [Args...]])                  ``Actual`` is greater or equal to ``Reference``.                            Compatible with C++ operator overloading
----------------------------------------------------------------------- --------------------------------------------------------------------------- -------------------------------------------
cr_assert_float_eq(Actual, Expected, Epsilon, [Message, [Args...]])     ``Actual`` is equal to ``Expected`` with a tolerance of ``Epsilon``.        Use this to test equality between floats
----------------------------------------------------------------------- --------------------------------------------------------------------------- -------------------------------------------
cr_assert_float_neq(Actual, Unexpected, Epsilon, [Message, [Args...]])  ``Actual`` is not equal to ``Unexpected`` with a tolerance of ``Epsilon``.  Use this to test inequality between floats
======================================================================= =========================================================================== ===========================================

String Assertions
-----------------

Note: these macros are meant to deal with *native* strings, i.e. char arrays.
Most of them won't work on ``std::string`` in C++, with some exceptions -- for
``std::string``, you should use regular comparison assersions, as listed above.

=========================================================== =================================================================== ===========================================
Macro                                                       Passes if and only if                                               Notes
=========================================================== =================================================================== ===========================================
cr_assert_str_empty(Value, [Message, [Args...]])            ``Value`` is an empty string.                                       Also works on std::string
----------------------------------------------------------- ------------------------------------------------------------------- -------------------------------------------
cr_assert_str_not_empty(Value, [Message, [Args...]])        ``Value`` is not an empty string.                                   Also works on std::string
----------------------------------------------------------- ------------------------------------------------------------------- -------------------------------------------
cr_assert_str_eq(Actual, Expected, [Message, [Args...]])    ``Actual`` is lexicographically equal to ``Expected``.
----------------------------------------------------------- ------------------------------------------------------------------- -------------------------------------------
cr_assert_str_neq(Actual, Unexpected, [Message, [Args...]]) ``Actual`` is not lexicographically equal to ``Unexpected``.
----------------------------------------------------------- ------------------------------------------------------------------- -------------------------------------------
cr_assert_str_lt(Actual, Reference, [Message, [Args...]])   ``Actual`` is lexicographically less than ``Reference``.
----------------------------------------------------------- ------------------------------------------------------------------- -------------------------------------------
cr_assert_str_leq(Actual, Reference, [Message, [Args...]])  ``Actual`` is lexicographically less or equal to ``Reference``.
----------------------------------------------------------- ------------------------------------------------------------------- -------------------------------------------
cr_assert_str_gt(Actual, Reference, [Message, [Args...]])   ``Actual`` is lexicographically greater than ``Reference``.
----------------------------------------------------------- ------------------------------------------------------------------- -------------------------------------------
cr_assert_str_geq(Actual, Reference, [Message, [Args...]])  ``Actual`` is lexicographically greater or equal to ``Reference``.
=========================================================== =================================================================== ===========================================

Array Assertions
-----------------

=========================================================================== =========================================================================== ===========================================
Macro                                                                       Passes if and only if                                                       Notes
=========================================================================== =========================================================================== ===========================================
cr_assert_arr_eq(Actual, Expected, [Message, [Args...]])                    ``Actual`` is byte-to-byte equal to ``Expected``.                           This should not be used on struct arrays,
                                                                                                                                                        consider using ``cr_assert_arr_eq_cmp``
                                                                                                                                                        instead.
--------------------------------------------------------------------------- --------------------------------------------------------------------------- -------------------------------------------
cr_assert_arr_neq(Actual, Unexpected, [Message, [Args...]])                 ``Actual`` is not byte-to-byte equal to ``Unexpected``.                     This should not be used on struct arrays,
                                                                                                                                                        consider using ``cr_assert_arr_neq_cmp``
                                                                                                                                                        instead.
--------------------------------------------------------------------------- --------------------------------------------------------------------------- -------------------------------------------
cr_assert_arr_eq_cmp(Actual, Expected, Size, Cmp, [Message, [Args...]])     ``Actual`` is comparatively equal to ``Expected``                           Only available in C++ and GNU C99
--------------------------------------------------------------------------- --------------------------------------------------------------------------- -------------------------------------------
cr_assert_arr_neq_cmp(Actual, Unexpected, Size, Cmp, [Message, [Args...]])  ``Actual`` is not comparatively equal to ``Expected``                       Only available in C++ and GNU C99
--------------------------------------------------------------------------- --------------------------------------------------------------------------- -------------------------------------------
cr_assert_arr_lt_cmp(Actual, Reference, Size, Cmp, [Message, [Args...]])    ``Actual`` is comparatively less than ``Reference``                         Only available in C++ and GNU C99
--------------------------------------------------------------------------- --------------------------------------------------------------------------- -------------------------------------------
cr_assert_arr_leq_cmp(Actual, Reference, Size, Cmp, [Message, [Args...]])   ``Actual`` is comparatively less or equal to ``Reference``                  Only available in C++ and GNU C99
--------------------------------------------------------------------------- --------------------------------------------------------------------------- -------------------------------------------
cr_assert_arr_gt_cmp(Actual, Reference, Size, Cmp, [Message, [Args...]])    ``Actual`` is comparatively greater than ``Reference``                      Only available in C++ and GNU C99
--------------------------------------------------------------------------- --------------------------------------------------------------------------- -------------------------------------------
cr_assert_arr_geq_cmp(Actual, Reference, Size, Cmp, [Message, [Args...]])   ``Actual`` is comparatively greater or equal to ``Reference``               Only available in C++ and GNU C99
=========================================================================== =========================================================================== ===========================================

