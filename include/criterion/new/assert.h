/*
 * The MIT License (MIT)
 *
 * Copyright © 2017 Franklin "Snaipe" Mathieu <http://snai.pe/>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#ifndef CRITERION_NEW_ASSERT_H_
#define CRITERION_NEW_ASSERT_H_
#ifdef CRITERION_DOCGEN

/**
 * Assertion API
 *
 * @defgroup AssertAPI Assertion API
 * @{
 */

/**
 * Mark the test as failed.
 *
 * The test is marked as a failure, printing the formatted string if provided,
 * and the execution continues.
 *
 * @param[in] Format (optional) Printf-like format string
 * @param[in] ...               Format string parameters
 */
#define cr_fail(Format, ...)

/**
 * Abort and mark the test as failed.
 *
 * The test is marked as a failure, printing the formatted string if provided,
 * and the execution of the test is aborted.
 *
 * @param[in] Format (optional) Printf-like format string
 * @param[in] ...               Format string parameters
 */
#define cr_fatal(Format, ...)

/**
 * Abort and mark the test as skipped.
 *
 * The test is marked as skipped, printing the formatted string if provided,
 * and the execution of the test is aborted.
 *
 * @param[in] Format (optional) Printf-like format string
 * @param[in] ...               Format string parameters
 */
#define cr_skip(Format, ...)

/**
 * Assert that a criterion is true and abort if it is not.
 *
 * cr_assert evaluates the passed criterion and passes if it has a non-zero
 * value.
 *
 * The criterion may be any C expression of non-void type, in which case the
 * assertion value will be `!!Criterion`.
 * Alternatively, the criterion may be any of the valid pseudo-functions
 * described in the [Criteria list](@ref Criteria).
 *
 * If the evaluated criterion is zero, then cr_fatal(Format, ...) is called.
 *
 * @param[in] Criterion         The Criterion to evaluate
 * @param[in] Format (optional) Printf-like format string
 * @param[in] ...               Format string parameters
 */
#define cr_assert(Criterion, Format, ...)

/**
 * Expect that a criterion is true and fail if it is not.
 *
 * cr_expect evaluates the passed criterion and passes if it has a non-zero
 * value.
 *
 * The criterion may be any C expression of non-void type, in which case the
 * assertion value will be `!!Criterion`.
 * Alternatively, the criterion may be any of the valid pseudo-functions
 * described in the [Criteria list](@ref Criteria).
 *
 * If the evaluated criterion is zero, then cr_fail(Format, ...) is called.
 *
 * @param[in] Criterion         The Criterion to evaluate
 * @param[in] Format (optional) Printf-like format string
 * @param[in] ...               Format string parameters
 */
#define cr_expect(Criterion, Format, ...)

/**
 * cr_assert_user is an utility macro to help users implement their own
 * assertions easily.
 *
 * Users may pass file and line information. The function behaves like
 * cr_assert and cr_expect, in that it evaluates the criterion to determine
 * whether a test fails or not.
 *
 * When the criterion evaluates to zero, a failed assertion event is raised
 * back to the runner, and then FailFunc is called without parameters.
 *
 * @param[in] File              The file in which the assertion has been
 *                              called.
 * @param[in] Line              The line number at which the assertion has been
 *                              called.
 * @param[in] FailFunc          The function to call on a failed assertion.
 * @param[in] Criterion         The Criterion to evaluate.
 * @param[in] Format (optional) Printf-like format string.
 * @param[in] ...               Format string parameters.
 */
#define cr_assert_user(File, Line, FailFunc, Criterion, Format, ...)

/**@}*/

/**
 * Criteria are pseudo-functions that evaluate to a boolean value.
 *
 * Using criteria is recommended over standard C operator as they allow
 * value pretty printing and other diagnostics on assertion failure.
 *
 * @note Criteria are neither symbols or macros, but pseudo-functions. They
 * are only valid in the context of the assertion API when explicitely
 * allowed and cannot be called alone.
 *
 * @defgroup Criteria Criteria
 * @{
 */

/**
 *  Evaluates to `!Criterion`.
 *
 *  @param[in] Criterion The criterion to negate
 */
#define not(Criterion)

/**
 *  Evaluates to true if all its arguments are true.
 *
 *  all() evaluates a sequence of criteria, and combines them into
 *  a single value with the logical and operator (&&).
 *
 *  @param[in] ... A sequence of criteria to evaluate
 */
#define all(...)

/**
 *  Evaluates to true if any of its arguments is true.
 *
 *  any() evaluates a sequence of criteria, and combines them into
 *  a single value with the logical or operator (||).
 *
 *  @param[in] ... A sequence of criteria to evaluate
 */
#define any(...)

/**
 *  Evaluates to true if none of its arguments is true.
 *
 *  none() evaluates a sequence of criteria, and combines their negation into
 *  a single value with the logical and operator (&&).
 *
 *  @param[in] ... A sequence of criteria to evaluate
 */
#define none(...)

/**
 * Tagged Criteria are special criteria that take an optional
 * type tag as their first argument.
 *
 * Unless otherwise specified, all tagged criteria generally support
 * any of the [supported tags](@ref Tags)
 *
 * @defgroup TaggedCriteria Tagged Criteria
 * @{
 */

/**
 *  Evaluates to `Actual == Expected`.
 *
 *  While this operator works for `flt`, `dbl`, and `ldbl`, the chance of having
 *  the values being exactly equal to each other is astronomically low due to
 *  round-off errors. Instead, please use as appropriate @ref ieee_ulp_eq and
 *  @ref epsilon_eq
 *
 *  @param[in] "Tag (optional)" The type tag of the parameters
 *  @param[in] Actual the actual value
 *  @param[in] Expected the expected value
 */
#define eq(Tag, Actual, Expected)

/**
 *  Evaluates to `Actual != Unexpected`.
 *
 *  While this operator works for `flt`, `dbl`, and `ldbl`, the chance of having
 *  the values being exactly equal to each other is astronomically low due to
 *  round-off errors. Instead, please use as appropriate @ref ieee_ulp_ne and
 *  @ref epsilon_ne
 *
 *  @param[in] "Tag (optional)" The type tag of the parameters
 *  @param[in] Actual the actual value
 *  @param[in] Unexpected the unexpected value
 */
#define ne(Tag, Actual, Unexpected)

/**
 *  Evaluates to `Actual < Reference`.
 *
 *  @param[in] "Tag (optional)" The type tag of the parameters
 *  @param[in] Actual the actual value
 *  @param[in] Reference the reference value
 */
#define lt(Tag, Actual, Reference)

/**
 *  Evaluates to `Actual <= Reference`.
 *
 *  @param[in] "Tag (optional)" The type tag of the parameters
 *  @param[in] Actual the actual value
 *  @param[in] Reference the reference value
 */
#define le(Tag, Actual, Reference)

/**
 *  Evaluates to `Actual > Reference`.
 *
 *  @param[in] "Tag (optional)" The type tag of the parameters
 *  @param[in] Actual the actual value
 *  @param[in] Reference the reference value
 */
#define gt(Tag, Actual, Reference)

/**
 *  Evaluates to `Actual >= Reference`.
 *
 *  @param[in] "Tag (optional)" The type tag of the parameters
 *  @param[in] Actual the actual value
 *  @param[in] Reference the reference value
 */
#define ge(Tag, Actual, Reference)

/**
 *  Evaluates to true if the IEEE 754 floating point numbers `Actual` and
 *  `Expected` are almost equal, by being within `Ulp` units from each other.
 *
 *  This method of comparison is more accurate when comparing two IEEE 754
 *  floating point values when `Expected` is non-zero.
 *  When comparing against zero, please use [epsilon_eq](@ref epsilon_eq)
 *  instead.
 *
 *  This tagged criterion only supports the `flt`, `dbl` and `ldbl` tags.
 *
 *  A good general-purpose value for Ulp is `4`.
 *
 *  @param[in] "Tag (optional)" The type tag of the parameters
 *  @param[in] Actual the actual value
 *  @param[in] Expected the reference value
 *  @param[in] Ulp the number of units in the last place used in the comparison
 */
#define ieee_ulp_eq(Tag, Actual, Expected, Ulp)

/**
 *  Evaluates to true if the IEEE 754 floating point numbers `Actual` and
 *  `Expected` are different, by not being within `Ulp` units from each other.
 *
 *  This method of comparison is more accurate when comparing two IEEE 754
 *  floating point values when `Expected` is non-zero.
 *  When comparing against zero, please use [epsilon_ne](@ref epsilon_ne)
 *  instead.
 *
 *  This tagged criterion only supports the `flt`, `dbl` and `ldbl` tags.
 *
 *  A good general-purpose value for Ulp is `4`.
 *
 *  @param[in] "Tag (optional)" The type tag of the parameters
 *  @param[in] Actual the actual value
 *  @param[in] Expected the reference value
 *  @param[in] Ulp the number of units in the last place used in the comparison
 */
#define ieee_ulp_ne(Tag, Actual, Expected, Ulp)

/**
 *  Evaluates to true if the floating point numbers `Actual` and `Expected` 
 *  are almost equal, by being within an absolute `Epsilon` from each other
 *  (In other words, if `fabs(Actual + Expected) <= Epsilon`).
 *
 *  This method of comparison is more accurate when comparing two IEEE 754
 *  floating point values that are near zero.
 *  When comparing against values that aren't near zero, please use
 *  [ieee_ulp_eq](@ref ieee_ulp_eq) instead.
 *
 *  This tagged criterion only supports the `flt`, `dbl` and `ldbl` tags.
 *
 *  It is recommended to have `Epsilon` be equal to a small multiple of the
 *  type epsilon (`FLT_EPSILON`, `DBL_EPSILON`, `LDBL_EPSILON`) and the
 *  input parameters.
 *
 *  @param[in] "Tag (optional)" The type tag of the parameters
 *  @param[in] Actual the actual value
 *  @param[in] Expected the reference value
 *  @param[in] Epsilon the epsilon used in the comparison
 */
#define epsilon_eq(Tag, Actual, Expected, Epsilon)

/**
 *  Evaluates to true if the floating point numbers `Actual` and `Expected` 
 *  are different, by not being within an absolute `Epsilon` from each other
 *  (In other words, if `fabs(Actual + Expected) > Epsilon`).
 *
 *  This method of comparison is more accurate when comparing two IEEE 754
 *  floating point values that are near zero.
 *  When comparing against values that aren't near zero, please use
 *  [ieee_ulp_eq](@ref ieee_ulp_eq) instead.
 *
 *  This tagged criterion only supports the `flt`, `dbl` and `ldbl` tags.
 *
 *  It is recommended to have `Epsilon` be equal to a small multiple of the
 *  type epsilon (`FLT_EPSILON`, `DBL_EPSILON`, `LDBL_EPSILON`) and the
 *  input parameters.
 *
 *  @param[in] "Tag (optional)" The type tag of the parameters
 *  @param[in] Actual the actual value
 *  @param[in] Expected the reference value
 *  @param[in] Epsilon the epsilon used in the comparison
 */
#define epsilon_ne(Tag, Actual, Expected, Epsilon)

/**@}*/
/**@}*/

/**
 * Tags are special tokens representing a type, that allow Criterion to infer
 * type information on parameters for better diagnostics on assertion failure.
 *
 * Any tag may also use the square-bracket array subscript notation to denote
 * an array type tag, like `i8[16]` or `type(struct user)[2]`, in which case
 * the criterion will apply on each element of this array.
 *
 * @note A tag is a special, Criterion-specific language token. It it neither
 * a symbol nor a macro, and cannot be used in any other context than when
 * explicitely allowed.
 *
 * @defgroup Tags
 * @{
 */

#define i8 int8_t
#define i16 int16_t
#define i32 int32_t
#define i64 int64_t
#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t
#define u64 uint64_t
#define sz size_t
#define ptr void *
#define iptr intptr_t
#define uptr uintptr_t
#define chr char
#define int int
#define uint unsigned int
#define long long
#define ulong unsigned long
#define llong long long
#define ullong unsigned long long
#define flt float
#define dbl double
#define ldbl long double
#define cx_flt complex float
#define cx_dbl complex double
#define cx_ldbl complex long double
#define mem struct cr_mem
#define str const char *
#define wcs const wchar_t *
#define tcs const TCHAR *

/**
 *  Represent an user-defined type.
 *
 *  The user type must be printable, and as such must implement a "to-string"
 *  operation:
 *
 *      (C only) char *cr_mem_<type>_tostr(const <type> *val);
 *      (C++ only) std::ostream &operator<<(std::ostream &os, const <type> &val);
 *
 *  Additionally, the user must implement the following functions to use
 *  various general-purpose criteria:
 *
 *  [eq](@ref eq), [ne](@ref ne), [le](@ref le), [ge](@ref ge):
 *
 *      (C only) int cr_mem_<type>_eq(const <type> *lhs, const <type> *rhs);
 *      (C++ only) bool operator==(const <type> &lhs, const <type> &rhs);
 *
 *  [lt](@ref lt), [le](@ref le), [gt](@ref gt), [ge](@ref ge):
 *
 *      (C only) int cr_mem_<type>_lt(const <type> *lhs, const <type> *rhs);
 *      (C++ only) bool operator<(const <type> &lhs, const <type> &rhs);
 *
 *  Due to implementation restrictions, UserType must either be a structure,
 *  an union, an enum, or a typedef.
 *
 *  For instance, these are fine:
 *
 *      type(foo)
 *      type(struct foo)
 *
 *  and these are not:
 *
 *      type(foo *)
 *      type(int (&foo)(void))
 *
 *  in these cases, use a typedef to alias those types to a single-word token.
 *
 */
#define type(UserType) UserType

/**@}*/

#endif /* !CRITERION_DOCGEN */

#include "memory.h"
#include "stream.h"

/* Internals */

#include "../internal/new_asserts.h"

#endif /* !CRITERION_NEW_ASSERT_H_ */
