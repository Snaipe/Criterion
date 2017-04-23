/*
 * The MIT License (MIT)
 *
 * Copyright © 2015-2016 Franklin "Snaipe" Mathieu <http://snai.pe/>
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

/**
 * @file
 * @brief Assertion API
 *****************************************************************************/
#ifndef CRITERION_ASSERT_H_
#define CRITERION_ASSERT_H_

#ifdef __cplusplus
# include <algorithm>
#endif

/**
 * @defgroup BaseAsserts Base assertions
 * @{
 */

/**
 * Fails always.
 *
 * The test is marked as failure and the execution of the function is aborted.
 *
 * The optional string is printed on failure.
 *
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_assert_fail(FormatString, ...)    internal

/**
 * Skips the test
 *
 * The test is marked as skipped and the execution of the function is aborted.
 *
 *****************************************************************************/
#define cr_skip_test(FormatString, ...)    internal

/**
 * Fails always.
 *
 * The test is marked as failure but the execution will continue.
 *
 * The optional string is printed on failure.
 *
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_expect_fail(FormatString, ...)    internal

/**
 * Passes if Condition is true
 *
 * Evaluates the condition and passes if it is true.
 * Otherwise the test is marked as failure and the execution of the function
 * is aborted.
 *
 * The optional string is printed on failure.
 *
 * @param[in] Condition Condition to test
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_assert(Condition, FormatString, ...)    internal

/**
 * Passes if Condition is true
 *
 * Evaluates the condition and passes if it is true.
 * Otherwise the test is marked as failure but the execution will continue.
 *
 * The optional string is printed on failure.
 *
 * @param[in] Condition Condition to test
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_expect(Condition, FormatString, ...)    internal

/**
 * Passes if Condition is false
 *
 * Evaluates the condition and passes if it is false.
 * Otherwise the test is marked as failure and the execution of the function
 * is aborted.
 *
 * The optional string is printed on failure.
 *
 * @param[in] Condition Condition to test
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_assert_not(Condition, FormatString, ...)    internal

/**
 * Passes if Condition is false
 *
 * Evaluates the condition and passes if it is false.
 * Otherwise the test is marked as failure but the execution will continue.
 *
 * The optional string is printed on failure.
 *
 * @param[in] Condition Condition to test
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_expect_not(Condition, FormatString, ...)    internal

/**@}*/

/**
 * @defgroup CommonBinAsserts Common binary assertions
 * @{
 */

/**
 * Passes if Actual is equal to Expected
 *
 * Passes if Actual is equal to Expected.
 * Otherwise the test is marked as failure and the execution of the function
 * is aborted.
 *
 * The optional string is printed on failure.
 *
 * @note Compatible with C++ operator overloading.
 *
 * @param[in] Actual Value to test
 * @param[in] Expected Expected value
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_assert_eq(Actual, Expected, FormatString, ...)    internal

/**
 * Passes if Actual is equal to Expected
 *
 * Passes if Actual is equal to Expected.
 * Otherwise the test is marked as failure but the execution will continue.
 *
 * The optional string is printed on failure.
 *
 * @note Compatible with C++ operator overloading.
 *
 * @param[in] Actual Value to test
 * @param[in] Expected Expected value
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_expect_eq(Actual, Expected, FormatString, ...)    internal

/**
 * Passes if Actual is not equal to Unexpected
 *
 * Passes if Actual is not equal to Unexpected
 * Otherwise the test is marked as failure and the execution of the function
 * is aborted.
 *
 * The optional string is printed on failure.
 *
 * @note Compatible with C++ operator overloading.
 *
 * @param[in] Actual Value to test
 * @param[in] Unexpected Unexpected Value
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_assert_neq(Actual, Unexpected, FormatString, ...)    internal

/**
 * Passes if Actual is not equal to Unexpected
 *
 * Passes if Actual is not equal to Unexpected.
 * Otherwise the test is marked as failure but the execution will continue.
 *
 * The optional string is printed on failure.
 *
 * @note Compatible with C++ operator overloading.
 *
 * @param[in] Actual Value to test
 * @param[in] Unexpected Unexpected Value
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_expect_neq(Actual, Unexpected, FormatString, ...)    internal

/**
 * Passes if Actual is less than Reference
 *
 * Passes if Actual is less than Reference.
 * Otherwise the test is marked as failure and the execution of the function
 * is aborted.
 *
 * The optional string is printed on failure.
 *
 * @note Compatible with C++ operator overloading.
 *
 * @param[in] Actual Value to test
 * @param[in] Reference Reference value
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_assert_lt(Actual, Reference, FormatString, ...)    internal

/**
 * Passes if Actual is less than Reference
 *
 * Passes if Actual is less than Reference.
 * Otherwise the test is marked as failure but the execution will continue.
 *
 * The optional string is printed on failure.
 *
 * @note Compatible with C++ operator overloading.
 *
 * @param[in] Actual Value to test
 * @param[in] Reference Reference value
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_expect_lt(Actual, Reference, FormatString, ...)    internal

/**
 * Passes if Actual is less or equal to Reference
 *
 * Passes if Actual is less or equal to Reference.
 * Otherwise the test is marked as failure and the execution of the function
 * is aborted.
 *
 * The optional string is printed on failure.
 *
 * @note Compatible with C++ operator overloading.
 *
 * @param[in] Actual Value to test
 * @param[in] Reference Reference value
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_assert_leq(Actual, Reference, FormatString, ...)    internal

/**
 * Passes if Actual is less or equal to Reference
 *
 * Passes if Actual is less or equal to Reference.
 * Otherwise the test is marked as failure but the execution will continue.
 *
 * The optional string is printed on failure.
 *
 * @note Compatible with C++ operator overloading.
 *
 * @param[in] Actual Value to test
 * @param[in] Reference Reference value
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_expect_leq(Actual, Reference, FormatString, ...)    internal

/**
 * Passes if Actual is greater than Reference
 *
 * Passes if Actual is greater than Reference.
 * Otherwise the test is marked as failure and the execution of the function
 * is aborted.
 *
 * The optional string is printed on failure.
 *
 * @note Compatible with C++ operator overloading.
 *
 * @param[in] Actual Value to test
 * @param[in] Reference Reference value
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_assert_gt(Actual, Reference, FormatString, ...)    internal

/**
 * Passes if Actual is greater than Reference
 *
 * Passes if Actual is greater than Reference.
 * Otherwise the test is marked as failure but the execution will continue.
 *
 * The optional string is printed on failure.
 *
 * @note Compatible with C++ operator overloading.
 *
 * @param[in] Actual Value to test
 * @param[in] Reference Reference value
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_expect_gt(Actual, Reference, FormatString, ...)    internal

/**
 * Passes if Actual is greater or equal to Reference
 *
 * Passes if Actual is greater or equal to Reference.
 * Otherwise the test is marked as failure and the execution of the function
 * is aborted.
 *
 * The optional string is printed on failure.
 *
 * @note Compatible with C++ operator overloading.
 *
 * @param[in] Actual Value to test
 * @param[in] Reference Reference value
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_assert_geq(Actual, Reference, FormatString, ...)    internal

/**
 * Passes if Actual is greater or equal to Reference
 *
 * Passes if Actual is greater or equal to Reference.
 * Otherwise the test is marked as failure but the execution will continue.
 *
 * The optional string is printed on failure.
 *
 * @note Compatible with C++ operator overloading.
 *
 * @param[in] Actual Value to test
 * @param[in] Reference Reference value
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_expect_geq(Actual, Reference, FormatString, ...)    internal

/**@}*/

/**
 * @defgroup CommonUnaryAsserts Common unary assertions
 * @{
 */

/**
 * Passes if Value is NULL
 *
 * Passes if Value is NULL.
 * Otherwise the test is marked as failure and the execution of the function
 * is aborted.
 *
 * The optional string is printed on failure.
 *
 * @param[in] Actual Value to test
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_assert_null(Value, FormatString, ...)    internal

/**
 * Passes if Value is NULL
 *
 * Passes if Value is NULL.
 * Otherwise the test is marked as failure but the execution will continue.
 *
 * The optional string is printed on failure.
 *
 * @param[in] Actual Value to test
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_expect_null(Value, FormatString, ...)    internal

/**
 * Passes if Value is not NULL
 *
 * Passes if Value is not NULL.
 * Otherwise the test is marked as failure and the execution of the function
 * is aborted.
 *
 * The optional string is printed on failure.
 *
 * @param[in] Value Value to test
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_assert_not_null(Value, FormatString, ...)    internal

/**
 * Passes if Value is not NULL
 *
 * Passes if Value is not NULL.
 * Otherwise the test is marked as failure but the execution will continue.
 *
 * The optional string is printed on failure.
 *
 * @param[in] Value Value to test
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_expect_not_null(Value, FormatString, ...)    internal

/**@}*/

/**
 * @defgroup FloatAsserts Floating-point assertions
 * @{
 */

/**
 * Passes if Actual is equal to Expected with a tolerance of Epsilon
 *
 * Passes if Actual is equal to Expected with a tolerance of Epsilon.
 * Otherwise the test is marked as failure and the execution of the function
 * is aborted.
 *
 * The optional string is printed on failure.
 *
 * @note Use this to test equality between floats
 *
 * @param[in] Actual Value to test
 * @param[in] Expected Expected value
 * @param[in] Epsilon Tolerance between Actual and Expected
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_assert_float_eq(Actual, Expected, Epsilon, FormatString, ...)    internal

/**
 * Passes if Actual is equal to Expected with a tolerance of Epsilon
 *
 * Passes if Actual is equal to Expected with a tolerance of Epsilon.
 * Otherwise the test is marked as failure but the execution will continue.
 *
 * The optional string is printed on failure.
 *
 * @note Use this to test equality between floats
 *
 * @param[in] Actual Value to test
 * @param[in] Expected Expected value
 * @param[in] Epsilon Tolerance between Actual and Expected
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_expect_float_eq(Actual, Expected, Epsilon, FormatString, ...)    internal

/**
 * Passes if Actual is not equal to Unexpected with a tolerance of Epsilon
 *
 * Passes if Actual is not equal to Unexpected with a tolerance of Epsilon.
 * Otherwise the test is marked as failure and the execution of the function
 * is aborted.
 *
 * The optional string is printed on failure.
 *
 * @note Use this to test inequality between floats
 *
 * @param[in] Actual Value to test
 * @param[in] Unexpected Unexpected value
 * @param[in] Epsilon Tolerance between Actual and Expected
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_assert_float_neq(Actual, Unexpected, Epsilon, FormatString, ...)    internal

/**
 * Passes if Actual is not equal to Unexpected with a tolerance of Epsilon
 *
 * Passes if Actual is not equal to Unexpected with a tolerance of Epsilon.
 * Otherwise the test is marked as failure but the execution will continue.
 *
 * The optional string is printed on failure.
 *
 * @note Use this to test inequality between floats
 *
 * @param[in] Actual Value to test
 * @param[in] Unexpected Unexpected value
 * @param[in] Epsilon Tolerance between Actual and Expected
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_expect_float_neq(Actual, Unexpected, Epsilon, FormatString, ...)    internal

/**@}*/

/**
 * @defgroup StringAsserts String assertions
 *
 * @note
 * These macros are meant to deal with *native* strings, i.e. char arrays.
 * Most of them won't work on ``std::string`` in C++, with some exceptions --
 * for ``std::string``, you should use regular comparison assersions.
 *
 * @{
 */

/**
 * Passes if Value is an empty string
 *
 * Passes if Value is an empty string.
 * Otherwise the test is marked as failure and the execution of the function
 * is aborted.
 *
 * The optional string is printed on failure.
 *
 * @note Also works on std::string.
 *
 * @param[in] Value String to test
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_assert_str_empty(Value, FormatString, ...)    internal

/**
 * Passes if Value is an empty string
 *
 * Passes if Value is an empty string.
 * Otherwise the test is marked as failure but the execution will continue.
 *
 * The optional string is printed on failure.
 *
 * @note Also works on std::string.
 *
 * @param[in] Value String to test
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_expect_str_empty(Value, FormatString, ...)    internal

/**
 * Passes if Value is not an empty string
 *
 * Passes if Value is not an empty string.
 * Otherwise the test is marked as failure and the execution of the function
 * is aborted.
 *
 * The optional string is printed on failure.
 *
 * @note Also works on std::string.
 *
 * @param[in] Value String to test
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_assert_str_not_empty(Value, FormatString, ...)    internal

/**
 * Passes if Value is not an empty string
 *
 * Passes if Value is not an empty string.
 * Otherwise the test is marked as failure but the execution will continue.
 *
 * The optional string is printed on failure.
 *
 * @note Also works on std::string.
 *
 * @param[in] Value String to test
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_expect_str_not_empty(Value, FormatString, ...)    internal

/**
 * Passes if Actual is lexicographically equal to Expected
 *
 * Passes if Actual is lexicographically equal to Expected.
 * Otherwise the test is marked as failure and the execution of the function
 * is aborted.
 *
 * The optional string is printed on failure.
 *
 * @param[in] Actual String to test
 * @param[in] Expected Expected String
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_assert_str_eq(Actual, Expected, FormatString, ...)    internal

/**
 * Passes if Actual is lexicographically equal to Expected
 *
 * Passes if Actual is lexicographically equal to Expected.
 * Otherwise the test is marked as failure but the execution will continue.
 *
 * The optional string is printed on failure.
 *
 * @param[in] Actual String to test
 * @param[in] Expected Expected String
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_expect_str_eq(Actual, Expected, FormatString, ...)    internal

/**
 * Passes if Actual is not lexicographically equal to Unexpected
 *
 * Passes if Actual is not lexicographically equal to Unexpected.
 * Otherwise the test is marked as failure and the execution of the function
 * is aborted.
 *
 * The optional string is printed on failure.
 *
 * @param[in] Actual String to test
 * @param[in] Unexpected Unexpected String
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_assert_str_neq(Actual, Unexpected, FormatString, ...)    internal

/**
 * Passes if Actual is not lexicographically equal to Unexpected
 *
 * Passes if Actual is not lexicographically equal to Unexpected.
 * Otherwise the test is marked as failure but the execution will continue.
 *
 * The optional string is printed on failure.
 *
 * @param[in] Actual String to test
 * @param[in] Unexpected Unexpected String
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_expect_str_neq(Actual, Unexpected, FormatString, ...)    internal

/**
 * Passes if Actual is lexicographically less than Reference
 *
 * Passes if Actual is lexicographically less than Reference.
 * Otherwise the test is marked as failure and the execution of the function
 * is aborted.
 *
 * The optional string is printed on failure.
 *
 * @param[in] Actual String to test
 * @param[in] Reference Reference String
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_assert_str_lt(Actual, Reference, FormatString, ...)    internal

/**
 * Passes if Actual is lexicographically less than Reference
 *
 * Passes if Actual is lexicographically less than Reference.
 * Otherwise the test is marked as failure but the execution will continue.
 *
 * The optional string is printed on failure.
 *
 * @param[in] Actual String to test
 * @param[in] Reference Reference String
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_expect_str_lt(Actual, Reference, FormatString, ...)    internal

/**
 * Passes if Actual is lexicographically less or equal to Reference
 *
 * Passes if Actual is lexicographically less or equal to Reference.
 * Otherwise the test is marked as failure and the execution of the function
 * is aborted.
 *
 * The optional string is printed on failure.
 *
 * @param[in] Actual String to test
 * @param[in] Reference Reference String
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_assert_str_leq(Actual, Reference, FormatString, ...)    internal

/**
 * Passes if Actual is lexicographically less or equal to Reference
 *
 * Passes if Actual is lexicographically less or equal to Reference.
 * Otherwise the test is marked as failure but the execution will continue.
 *
 * The optional string is printed on failure.
 *
 * @param[in] Actual String to test
 * @param[in] Reference Reference String
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_expect_str_leq(Actual, Reference, FormatString, ...)    internal

/**
 * Passes if Actual is lexicographically greater than Reference
 *
 * Passes if Actual is lexicographically greater than Reference.
 * Otherwise the test is marked as failure and the execution of the function
 * is aborted.
 *
 * The optional string is printed on failure.
 *
 * @param[in] Actual String to test
 * @param[in] Reference Reference String
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_assert_str_gt(Actual, Reference, FormatString, ...)    internal

/**
 * Passes if Actual is lexicographically greater than Reference
 *
 * Passes if Actual is lexicographically greater than Reference.
 * Otherwise the test is marked as failure but the execution will continue.
 *
 * The optional string is printed on failure.
 *
 * @param[in] Actual String to test
 * @param[in] Reference Reference String
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_expect_str_gt(Actual, Reference, FormatString, ...)    internal

/**
 * Passes if Actual is lexicographically greater or equal to Reference
 *
 * Passes if Actual is lexicographically greater or equal to Reference.
 * Otherwise the test is marked as failure and the execution of the function
 * is aborted.
 *
 * The optional string is printed on failure.
 *
 * @param[in] Actual String to test
 * @param[in] Reference Reference String
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_assert_str_geq(Actual, Reference, FormatString, ...)    internal

/**
 * Passes if Actual is lexicographically greater or equal to Reference
 *
 * Passes if Actual is lexicographically greater or equal to Reference.
 * Otherwise the test is marked as failure but the execution will continue.
 *
 * The optional string is printed on failure.
 *
 * @param[in] Actual String to test
 * @param[in] Reference Reference String
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_expect_str_geq(Actual, Reference, FormatString, ...)    internal

/**@}*/

/**
 * @defgroup WideStringAsserts Wide String Assertions
 *
 * @note
 * These macros are meant to deal with *native* wide character strings, i.e.
 * wchar_t arrays.  Most of them won't work on ``std::wstring`` in C++, with
 * some exceptions -- for ``std::wstring``, you should use regular comparison
 * assertions.
 *
 * @{
 */

/**
 * Passes if Value is an empty wide string
 *
 * Passes if Value is an empty wide string.
 * Otherwise the test is marked as failure and the execution of the function
 * is aborted.
 *
 * The optional (non-wide) string is printed on failure.
 *
 * @note Also works on std::wstring.
 *
 * @param[in] Value Wide string to test
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_assert_wcs_empty(Value, FormatString, ...)    internal

/**
 * Passes if Value is an empty wide string
 *
 * Passes if Value is an empty wide string.
 * Otherwise the test is marked as failure but the execution will continue.
 *
 * The optional (non-wide) string is printed on failure.
 *
 * @note Also works on std::wstring.
 *
 * @param[in] Value Wide string to test
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_expect_wcs_empty(Value, FormatString, ...)    internal

/**
 * Passes if Value is not an empty wide string
 *
 * Passes if Value is not an empty wide string.
 * Otherwise the test is marked as failure and the execution of the function
 * is aborted.
 *
 * The optional (non-wide) string is printed on failure.
 *
 * @note Also works on std::string.
 *
 * @param[in] Value Wide string to test
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_assert_wcs_not_empty(Value, FormatString, ...)    internal

/**
 * Passes if Value is not an empty wide string
 *
 * Passes if Value is not an empty wide string.
 * Otherwise the test is marked as failure but the execution will continue.
 *
 * The optional (non-wide) string is printed on failure.
 *
 * @note Also works on std::string.
 *
 * @param[in] Value Wide string to test
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_expect_wcs_not_empty(Value, FormatString, ...)    internal

/**
 * Passes if Actual is lexicographically equal to Expected
 *
 * Passes if Actual is lexicographically equal to Expected.
 * Otherwise the test is marked as failure and the execution of the function
 * is aborted.
 *
 * The optional (non-wide) string is printed on failure.
 *
 * @param[in] Actual Wide string to test
 * @param[in] Expected Expected wide string
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_assert_wcs_eq(Actual, Expected, FormatString, ...)    internal

/**
 * Passes if Actual is lexicographically equal to Expected
 *
 * Passes if Actual is lexicographically equal to Expected.
 * Otherwise the test is marked as failure but the execution will continue.
 *
 * The optional (non-wide) string is printed on failure.
 *
 * @param[in] Actual Wide string to test
 * @param[in] Expected Expected wide string
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_expect_wcs_eq(Actual, Expected, FormatString, ...)    internal

/**
 * Passes if Actual is not lexicographically equal to Unexpected
 *
 * Passes if Actual is not lexicographically equal to Unexpected.
 * Otherwise the test is marked as failure and the execution of the function
 * is aborted.
 *
 * The optional (non-wide) string is printed on failure.
 *
 * @param[in] Actual Wide string to test
 * @param[in] Unexpected Unexpected wide string
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_assert_wcs_neq(Actual, Unexpected, FormatString, ...)    internal

/**
 * Passes if Actual is not lexicographically equal to Unexpected
 *
 * Passes if Actual is not lexicographically equal to Unexpected.
 * Otherwise the test is marked as failure but the execution will continue.
 *
 * The optional (non-wide) string is printed on failure.
 *
 * @param[in] Actual Wide string to test
 * @param[in] Unexpected Unexpected wide string
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_expect_wcs_neq(Actual, Unexpected, FormatString, ...)    internal

/**
 * Passes if Actual is lexicographically less than Reference
 *
 * Passes if Actual is lexicographically less than Reference.
 * Otherwise the test is marked as failure and the execution of the function
 * is aborted.
 *
 * The optional (non-wide) string is printed on failure.
 *
 * @param[in] Actual Wide string to test
 * @param[in] Reference Reference wide string
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_assert_wcs_lt(Actual, Reference, FormatString, ...)    internal

/**
 * Passes if Actual is lexicographically less than Reference
 *
 * Passes if Actual is lexicographically less than Reference.
 * Otherwise the test is marked as failure but the execution will continue.
 *
 * The optional (non-wide) string is printed on failure.
 *
 * @param[in] Actual Wide string to test
 * @param[in] Reference Reference wide string
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_expect_wcs_lt(Actual, Reference, FormatString, ...)    internal

/**
 * Passes if Actual is lexicographically less or equal to Reference
 *
 * Passes if Actual is lexicographically less or equal to Reference.
 * Otherwise the test is marked as failure and the execution of the function
 * is aborted.
 *
 * The optional (non-wide) string is printed on failure.
 *
 * @param[in] Actual Wide string to test
 * @param[in] Reference Reference wide string
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_assert_wcs_leq(Actual, Reference, FormatString, ...)    internal

/**
 * Passes if Actual is lexicographically less or equal to Reference
 *
 * Passes if Actual is lexicographically less or equal to Reference.
 * Otherwise the test is marked as failure but the execution will continue.
 *
 * The optional (non-wide) string is printed on failure.
 *
 * @param[in] Actual Wide string to test
 * @param[in] Reference Reference wide string
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_expect_wcs_leq(Actual, Reference, FormatString, ...)    internal

/**
 * Passes if Actual is lexicographically greater than Reference
 *
 * Passes if Actual is lexicographically greater than Reference.
 * Otherwise the test is marked as failure and the execution of the function
 * is aborted.
 *
 * The optional (non-wide) string is printed on failure.
 *
 * @param[in] Actual Wide string to test
 * @param[in] Reference Reference wide string
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_assert_wcs_gt(Actual, Reference, FormatString, ...)    internal

/**
 * Passes if Actual is lexicographically greater than Reference
 *
 * Passes if Actual is lexicographically greater than Reference.
 * Otherwise the test is marked as failure but the execution will continue.
 *
 * The optional (non-wide) string is printed on failure.
 *
 * @param[in] Actual Wide string to test
 * @param[in] Reference Reference wide string
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_expect_wcs_gt(Actual, Reference, FormatString, ...)    internal

/**
 * Passes if Actual is lexicographically greater or equal to Reference
 *
 * Passes if Actual is lexicographically greater or equal to Reference.
 * Otherwise the test is marked as failure and the execution of the function
 * is aborted.
 *
 * The optional (non-wide) string is printed on failure.
 *
 * @param[in] Actual Wide string to test
 * @param[in] Reference Reference wide string
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_assert_wcs_geq(Actual, Reference, FormatString, ...)    internal

/**
 * Passes if Actual is lexicographically greater or equal to Reference
 *
 * Passes if Actual is lexicographically greater or equal to Reference.
 * Otherwise the test is marked as failure but the execution will continue.
 *
 * The optional (non-wide) string is printed on failure.
 *
 * @param[in] Actual Wide string to test
 * @param[in] Reference Reference wide string
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_expect_wcs_geq(Actual, Reference, FormatString, ...)    internal

/**@}*/

/**
 * @defgroup ArrayAsserts Array assertions
 * @{
 */

/**
 * Passes if Actual is byte-to-byte equal to Expected
 *
 * Passes if Actual is byte-to-byte equal to Expected.
 * Otherwise the test is marked as failure and the execution of the function
 * is aborted.
 *
 * The optional string is printed on failure.
 *
 * @warning This should not be used on struct arrays, consider using \c
 * cr_assert_arr_eq_cmp() instead.
 *
 * @param[in] Actual Array to test
 * @param[in] Expected Expected array
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_assert_arr_eq(Actual, Expected, FormatString, ...)    internal

/**
 * Passes if Actual is byte-to-byte equal to Expected
 *
 * Passes if Actual is byte-to-byte equal to Expected.
 * Otherwise the test is marked as failure but the execution will continue.
 *
 * The optional string is printed on failure.
 *
 * @warning This should not be used on struct arrays, consider using \c
 * cr_expect_arr_eq_cmp() instead.
 *
 * @param[in] Actual Array to test
 * @param[in] Expected Expected array
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_expect_arr_eq(Actual, Expected, FormatString, ...)    internal

/**
 * Passes if Actual is not byte-to-byte equal to Expected
 *
 * Passes if Actual is not byte-to-byte equal to Unexpected.
 * Otherwise the test is marked as failure and the execution of the function
 * is aborted.
 *
 * The optional string is printed on failure.
 *
 * @warning This should not be used on struct arrays, consider using \c
 * cr_assert_arr_neq_cmp() instead.
 *
 * @param[in] Actual Array to test
 * @param[in] Unexpected Unexpected array
 * @param[in] Size Number of bytes to check
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_assert_arr_neq(Actual, Unexpected, Size, FormatString, ...)    internal

/**
 * Passes if Actual is not byte-to-byte equal to Unexpected
 *
 * Passes if Actual is not byte-to-byte equal to Expected.
 * Otherwise the test is marked as failure but the execution will continue.
 *
 * The optional string is printed on failure.
 *
 * @warning This should not be used on struct arrays, consider using \c
 * cr_expect_arr_neq_cmp() instead.
 *
 * @param[in] Actual Array to test
 * @param[in] Unexpected Unexpected array
 * @param[in] Size Number of bytes to check
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_expect_arr_neq(Actual, Unexpected, Size, FormatString, ...)    internal

/**@}*/

/**
 * @defgroup SafeArrCmpAsserts Safe array comparison assertions
 * @brief C++ / GNU C only!
 *
 * These macros are only available on C++ compilers.
 * @{
 */

/**
 * Passes if Actual is comparatively equal to Expected (C++ / GNU C99 only)
 *
 * Passes if Actual is comparatively equal to Expected.
 * Otherwise the test is marked as failure and the execution of the function
 * is aborted.
 *
 * The macro takes a `int (*comparator)(typeof(Actual) a, typeof(Expected) b)`
 * function pointer, that returns -1, 0, or 1 when `a` is respectively less,
 * equal to, or greater than `b`.
 *
 * The optional string is printed on failure.
 *
 * @note This macro is only available on C++ and GNU C compilers.
 *
 * @param[in] Actual Array to test
 * @param[in] Expected Expected array
 * @param[in] Size Number of bytes to check
 * @param[in] Cmp The comparator to use
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_assert_arr_eq_cmp(Actual, Expected, Size, Cmp, FormatString, ...)    internal

/**
 * Passes if Actual is comparatively equal to Expected (C++ / GNU C99 only)
 *
 * Passes if Actual is comparatively equal to Expected.
 * Otherwise the test is marked as failure but the execution will continue.
 *
 * The macro takes a `int (*comparator)(typeof(Actual) a, typeof(Expected) b)`
 * function pointer, that returns -1, 0, or 1 when `a` is respectively less,
 * equal to, or greater than `b`.
 *
 * The optional string is printed on failure.
 *
 * @note This macro is only available on C++ and GNU C compilers.
 *
 * @param[in] Actual Array to test
 * @param[in] Expected Expected array
 * @param[in] Size Number of bytes to check
 * @param[in] Cmp The comparator to use
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_expect_arr_eq_cmp(Actual, Expected, Size, Cmp, FormatString, ...)    internal

/**
 * Passes if Actual is not comparatively equal to Unexpected (C++ / GNU C99
 * only)
 *
 * Passes if Actual is not comparatively equal to Unexpected.
 * Otherwise the test is marked as failure and the execution of the function
 * is aborted.
 *
 * The macro takes a `int (*comparator)(typeof(Actual) a, typeof(Expected) b)`
 * function pointer, that returns -1, 0, or 1 when `a` is respectively less,
 * equal to, or greater than `b`.
 *
 * The optional string is printed on failure.
 *
 * @note This macro is only available on C++ and GNU C compilers.
 *
 * @param[in] Actual Array to test
 * @param[in] Unexpected Unexpected array
 * @param[in] Size Number of bytes to check
 * @param[in] Cmp The comparator to use
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_assert_arr_neq_cmp(Actual, Unexpected, Size, Cmp, FormatString, ...)    internal

/**
 * Passes if Actual is not comparatively equal to Unexpected (C++ / GNU C99
 * only)
 *
 * Passes if Actual is not comparatively equal to Unexpected.
 * Otherwise the test is marked as failure but the execution will continue.
 *
 * The macro takes a `int (*comparator)(typeof(Actual) a, typeof(Expected) b)`
 * function pointer, that returns -1, 0, or 1 when `a` is respectively less,
 * equal to, or greater than `b`.
 *
 * The optional string is printed on failure.
 *
 * @note This macro is only available on C++ and GNU C compilers.
 *
 * @param[in] Actual Array to test
 * @param[in] Unexpected Unexpected array
 * @param[in] Size Number of bytes to check
 * @param[in] Cmp The comparator to use
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_expect_arr_neq_cmp(Actual, Unexpected, Size, Cmp, FormatString, ...)    internal

/**
 * Passes if Actual is comparatively less than Reference (C++ / GNU C99 only)
 *
 * Passes if Actual is  comparatively less than Reference.
 * Otherwise the test is marked as failure and the execution of the function
 * is aborted.
 *
 * The macro takes a `int (*comparator)(typeof(Actual) a, typeof(Expected) b)`
 * function pointer, that returns -1, 0, or 1 when `a` is respectively less,
 * equal to, or greater than `b`.
 *
 * The optional string is printed on failure.
 *
 * @note This macro is only available on C++ and GNU C compilers.
 *
 * @param[in] Actual Array to test
 * @param[in] Reference Reference array
 * @param[in] Size Number of bytes to check
 * @param[in] Cmp The comparator to use
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_assert_arr_lt_cmp(Actual, Reference, Size, Cmp, FormatString, ...)    internal

/**
 * Passes if Actual is comparatively less than Reference (C++ / GNU C99 only)
 *
 * Passes if Actual is  comparatively less than Reference.
 * Otherwise the test is marked as failure but the execution will continue.
 *
 * The macro takes a `int (*comparator)(typeof(Actual) a, typeof(Expected) b)`
 * function pointer, that returns -1, 0, or 1 when `a` is respectively less,
 * equal to, or greater than `b`.
 *
 * The optional string is printed on failure.
 *
 * @note This macro is only available on C++ and GNU C compilers.
 *
 * @param[in] Actual Array to test
 * @param[in] Reference Reference array
 * @param[in] Size Number of bytes to check
 * @param[in] Cmp The comparator to use
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_expect_arr_lt_cmp(Actual, Reference, Size, Cmp, FormatString, ...)    internal

/**
 * Passes if Actual is comparatively less or equal to Reference (C++ / GNU C99
 * only)
 *
 * Passes if Actual is  comparatively less or equal to Reference.
 * Otherwise the test is marked as failure and the execution of the function
 * is aborted.
 *
 * The macro takes a `int (*comparator)(typeof(Actual) a, typeof(Expected) b)`
 * function pointer, that returns -1, 0, or 1 when `a` is respectively less,
 * equal to, or greater than `b`.
 *
 * The optional string is printed on failure.
 *
 * @note This macro is only available on C++ and GNU C compilers.
 *
 * @param[in] Actual Array to test
 * @param[in] Reference Reference array
 * @param[in] Size Number of bytes to check
 * @param[in] Cmp The comparator to use
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_assert_arr_leq_cmp(Actual, Reference, Size, Cmp, FormatString, ...)    internal

/**
 * Passes if Actual is comparatively less or equal to Reference (C++ / GNU C99
 * only)
 *
 * Passes if Actual is  comparatively less or equal to Reference.
 * Otherwise the test is marked as failure but the execution will continue.
 *
 * The macro takes a `int (*comparator)(typeof(Actual) a, typeof(Expected) b)`
 * function pointer, that returns -1, 0, or 1 when `a` is respectively less,
 * equal to, or greater than `b`.
 *
 * The optional string is printed on failure.
 *
 * @note This macro is only available on C++ and GNU C compilers.
 *
 * @param[in] Actual Array to test
 * @param[in] Reference Reference array
 * @param[in] Size Number of bytes to check
 * @param[in] Cmp The comparator to use
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_expect_arr_leq_cmp(Actual, Reference, Size, Cmp, FormatString, ...)    internal

/**
 * Passes if Actual is comparatively greater than Reference (C++ / GNU C99 only)
 *
 * Passes if Actual is  comparatively greater than Reference.
 * Otherwise the test is marked as failure and the execution of the function
 * is aborted.
 *
 * The macro takes a `int (*comparator)(typeof(Actual) a, typeof(Expected) b)`
 * function pointer, that returns -1, 0, or 1 when `a` is respectively less,
 * equal to, or greater than `b`.
 *
 * The optional string is printed on failure.
 *
 * @note This macro is only available on C++ and GNU C compilers.
 *
 * @param[in] Actual Array to test
 * @param[in] Reference Reference array
 * @param[in] Size Number of bytes to check
 * @param[in] Cmp The comparator to use
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_assert_arr_gt_cmp(Actual, Reference, Size, Cmp, FormatString, ...)    internal

/**
 * Passes if Actual is comparatively greater than Reference (C++ / GNU C99 only)
 *
 * Passes if Actual is  comparatively greater than Reference.
 * Otherwise the test is marked as failure but the execution will continue.
 *
 * The macro takes a `int (*comparator)(typeof(Actual) a, typeof(Expected) b)`
 * function pointer, that returns -1, 0, or 1 when `a` is respectively less,
 * equal to, or greater than `b`.
 *
 * The optional string is printed on failure.
 *
 * @note This macro is only available on C++ and GNU C compilers.
 *
 * @param[in] Actual Array to test
 * @param[in] Reference Reference array
 * @param[in] Size Number of bytes to check
 * @param[in] Cmp The comparator to use
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_expect_arr_gt_cmp(Actual, Reference, Size, Cmp, FormatString, ...)    internal

/**
 * Passes if Actual is comparatively greater or equal to Reference (C++ / GNU
 * C99 only)
 *
 * Passes if Actual is  comparatively greater or equal to Reference.
 * Otherwise the test is marked as failure and the execution of the function
 * is aborted.
 *
 * The macro takes a `int (*comparator)(typeof(Actual) a, typeof(Expected) b)`
 * function pointer, that returns -1, 0, or 1 when `a` is respectively less,
 * equal to, or greater than `b`.
 *
 * The optional string is printed on failure.
 *
 * @note This macro is only available on C++ and GNU C compilers.
 *
 * @param[in] Actual Array to test
 * @param[in] Reference Reference array
 * @param[in] Size Number of bytes to check
 * @param[in] Cmp The comparator to use
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_assert_arr_geq_cmp(Actual, Reference, Size, Cmp, FormatString, ...)    internal

/**
 * Passes if Actual is comparatively greater or equal to Reference (C++ / GNU
 * C99 only)
 *
 * Passes if Actual is  comparatively greater or equal to Reference.
 * Otherwise the test is marked as failure but the execution will continue.
 *
 * The macro takes a `int (*comparator)(typeof(Actual) a, typeof(Expected) b)`
 * function pointer, that returns -1, 0, or 1 when `a` is respectively less,
 * equal to, or greater than `b`.
 *
 * The optional string is printed on failure.
 *
 * @note This macro is only available on C++ and GNU C compilers.
 *
 * @param[in] Actual Array to test
 * @param[in] Reference Reference array
 * @param[in] Size Number of bytes to check
 * @param[in] Cmp The comparator to use
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_expect_arr_geq_cmp(Actual, Reference, Size, Cmp, FormatString, ...)    internal

/**@}*/

#ifdef __cplusplus

/**
 * @defgroup ExceptionAsserts Exception asserts
 * @brief C++ only!
 *
 * These macros are only available on C++ compilers.
 * @{
 */

/**
 * Passes if Statement throws an instance of Exception (C++ only)
 *
 * Passes if Statement throws an instance of Exception.
 * Otherwise the test is marked as failure and the execution of the function
 * is aborted.
 *
 * The optional string is printed on failure.
 *
 * @note This macro is only available on C++ compilers.
 *
 * @param[in] Statement Statement to be executed
 * @param[in] Exception Expected exception
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
# define cr_assert_throw(Statement, Exception, FormatString, ...)    internal

/**
 * Passes if Statement throws an instance of Exception (C++ only)
 *
 * Passes if Statement throws an instance of Exception.
 * Otherwise the test is marked as failure but the execution will continue.
 *
 * The optional string is printed on failure.
 *
 * @note This macro is only available on C++ compilers.
 *
 * @param[in] Statement Statement to be executed
 * @param[in] Exception Expected exception
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
# define cr_expect_throw(Statement, Exception, FormatString, ...)    internal

/**
 * Passes if Statement does not throws an instance of Exception (C++ only)
 *
 * Passes if Statement does not throws an instance of Exception.
 * Otherwise the test is marked as failure and the execution of the function
 * is aborted.
 *
 * The optional string is printed on failure.
 *
 * @note This macro is only available on C++ compilers.
 *
 * @param[in] Statement Statement to be executed
 * @param[in] Exception Unexpected exception
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
# define cr_assert_no_throw(Statement, Exception, FormatString, ...)    internal

/**
 * Passes if Statement does not throws an instance of Exception (C++ only)
 *
 * Passes if Statement does not throws an instance of Exception.
 * Otherwise the test is marked as failure but the execution will continue.
 *
 * The optional string is printed on failure.
 *
 * @note This macro is only available on C++ compilers.
 *
 * @param[in] Statement Statement to be executed
 * @param[in] Exception Unexpected exception
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
# define cr_expect_no_throw(Statement, Exception, FormatString, ...)    internal

/**
 * Passes if Statement throws any kind of exception (C++ only)
 *
 * Passes if Statement throws any kind of exception
 * Otherwise the test is marked as failure and the execution of the function
 * is aborted.
 *
 * The optional string is printed on failure.
 *
 * @note This macro is only available on C++ compilers.
 *
 * @param[in] Statement Statement to be executed
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
# define cr_assert_any_throw(Statement, FormatString, ...)    internal

/**
 * Passes if Statement throws any kind of exception (C++ only)
 *
 * Passes if Statement throws any kind of exception
 * Otherwise the test is marked as failure but the execution will continue.
 *
 * The optional string is printed on failure.
 *
 * @note This macro is only available on C++ compilers.
 *
 * @param[in] Statement Statement to be executed
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
# define cr_expect_any_throw(Statement, FormatString, ...)    internal

/**
 * Passes if Statement does not throws any kind of exception (C++ only)
 *
 * Passes if Statement does not throws any kind of exception
 * Otherwise the test is marked as failure and the execution of the function
 * is aborted.
 *
 * The optional string is printed on failure.
 *
 * @note This macro is only available on C++ compilers.
 *
 * @param[in] Statement Statement to be executed
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
# define cr_assert_none_throw(Statement, FormatString, ...)    internal

/**
 * Passes if Statement does not throws any kind of exception (C++ only)
 *
 * Passes if Statement does not throws any kind of exception
 * Otherwise the test is marked as failure but the execution will continue.
 *
 * The optional string is printed on failure.
 *
 * @note This macro is only available on C++ compilers.
 *
 * @param[in] Statement Statement to be executed
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
# define cr_expect_none_throw(Statement, FormatString, ...)    internal

#endif
/**@}*/

/** @cond CRITERION_DOC_DEPRECATED */

/* The section below is here for backward compatibility purposes.
   It shall be removed in the next major version of Criterion */
#ifndef CRITERION_NO_COMPAT

# define CRITERION_ASSERT_DEPRECATED_B(Name, Newname) \
    CRITERION_ASSERT_DEPRECATED__(                    \
        message                                       \
            ("The `" #Name "` macro is deprecated, "  \
        "please use `" #Newname "` instead.")         \
        )

# ifdef _MSC_VER
#  define CRITERION_ASSERT_DEPRECATED__(Msg) \
    __pragma(Msg)
# else
#  define CRITERION_ASSERT_DEPRECATED__(Msg) \
    _Pragma(#Msg)
# endif

/* scheduled for removal after 2.0 */
# define cr_abort_test(Message)           CRITERION_ASSERT_DEPRECATED_B(cr_abort_test, cr_assert_fail) cr_assert_fail(Message)
# define cr_assert_strings_eq(...)        CRITERION_ASSERT_DEPRECATED_B(cr_assert_strings_eq, cr_assert_str_eq) cr_assert_str_eq(__VA_ARGS__)
# define cr_assert_strings_neq(...)       CRITERION_ASSERT_DEPRECATED_B(cr_assert_strings_neq, cr_assert_str_neq) cr_assert_str_neq(__VA_ARGS__)
# define cr_assert_strings_lt(...)        CRITERION_ASSERT_DEPRECATED_B(cr_assert_strings_lt, cr_assert_str_lt) cr_assert_str_lt(__VA_ARGS__)
# define cr_assert_strings_leq(...)       CRITERION_ASSERT_DEPRECATED_B(cr_assert_strings_leq, cr_assert_str_leq) cr_assert_str_leq(__VA_ARGS__)
# define cr_assert_strings_gt(...)        CRITERION_ASSERT_DEPRECATED_B(cr_assert_strings_gt, cr_assert_str_gt) cr_assert_str_gt(__VA_ARGS__)
# define cr_assert_strings_geq(...)       CRITERION_ASSERT_DEPRECATED_B(cr_assert_strings_geq, cr_assert_str_geq) cr_assert_str_geq(__VA_ARGS__)

# define cr_assert_arrays_eq(...)         CRITERION_ASSERT_DEPRECATED_B(cr_assert_arrays_eq, cr_assert_arr_eq) cr_assert_arr_eq(__VA_ARGS__)
# define cr_assert_arrays_neq(...)        CRITERION_ASSERT_DEPRECATED_B(cr_assert_arrays_neq, cr_assert_arr_neq) cr_assert_arr_neq(__VA_ARGS__)

# define cr_assert_arrays_eq_cmp(...)     CRITERION_ASSERT_DEPRECATED_B(cr_assert_arrays_eq_cmp, cr_assert_arr_eq_cmp) cr_assert_arr_eq_cmp(__VA_ARGS__)
# define cr_assert_arrays_neq_cmp(...)    CRITERION_ASSERT_DEPRECATED_B(cr_assert_arrays_neq_cmp, cr_assert_arr_neq_cmp) cr_assert_arr_neq_cmp(__VA_ARGS__)

#endif

/** @endcond */

#include "internal/assert.h"

#endif /* !CRITERION_ASSERT_H_ */
