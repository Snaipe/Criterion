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
 * @brief Redirect functions and file asserts
 *****************************************************************************/
#ifndef CRITERION_REDIRECT_H_
#define CRITERION_REDIRECT_H_

#include "internal/common.h"

#ifdef __cplusplus
# include <cstdio>
#else
# include <stdio.h>
#endif

CR_BEGIN_C_API

/**
 *  Redirect stdout for testing.
 */
CR_API void cr_redirect_stdout(void);

/**
 *  Redirect stderr for testing.
 */
CR_API void cr_redirect_stderr(void);

/**
 *  Redirect stdin for testing.
 *  This is implicitely called before each test.
 */
CR_API void cr_redirect_stdin(void);

/**
 *  Get a file handle representing the read-end of the redirected stdout.
 *
 *  @returns the file handle.
 */
CR_API CR_STDN FILE *cr_get_redirected_stdout(void);

/**
 *  Get a file handle representing the read-end of the redirected stderr.
 *
 *  @returns the file handle.
 */
CR_API CR_STDN FILE *cr_get_redirected_stderr(void);

/**
 *  Get a file handle representing the write-end of the redirected stdin.
 *
 *  @returns the file handle.
 */
CR_API CR_STDN FILE *cr_get_redirected_stdin(void);

/**
 *  Compare the contents of a file with a string.
 *
 *  @param[in] f    The file to compare the contents to.
 *  @param[in] str  The string to compare the contents to.
 *  @returns 1 if the contents of the file is equal to the string, 0 otherwise.
 */
CR_API int cr_file_match_str(CR_STDN FILE *f, const char *str);

/**
 *  Compare the contents of a file with the contents of another file.
 *
 *  @param[in] f    The first file to compare the contents to.
 *  @param[in] ref  The second file to compare the contents to.
 *  @returns 1 if the contents of the files are equal, 0 otherwise.
 */
CR_API int cr_file_match_file(CR_STDN FILE *f, CR_STDN FILE *ref);

/**
 *  Create a file mock.
 *
 *  @param[in] max_size The maximum size in bytes of the file mock.
 *  @returns the file handle representing the mock.
 */
CR_API CR_STDN FILE *cr_mock_file_size(size_t max_size);

CR_END_C_API

/**
 * @defgroup FileAsserts File content assertions
 * @{
 */

/**
 * Passes if the contents of \c File are equal to the string \c ExpectedContents
 *
 * Passes if the contents of \c File are equal to the string
 * \c ExpectedContents.
 * Otherwise the test is marked as failure and the execution of the function
 * is aborted.
 *
 * The optional string is printed on failure.
 *
 * @param[in] File Pointer to a FILE object that specifies an input stream
 * @param[in] ExpectedContents C string with the ExpectedContents
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_assert_file_contents_eq_str(File, ExpectedContents, FormatString, ...)       internal

/**
 * Passes if the contents of \c File are equal to the string \c ExpectedContents
 *
 * Passes if the contents of \c File are equal to the string
 * \c ExpectedContents.
 * Otherwise the test is marked as failure but the execution will continue.
 *
 * The optional string is printed on failure.
 *
 * @param[in] File Pointer to a FILE object that specifies an input stream
 * @param[in] ExpectedContents C string with the ExpectedContents
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_expect_file_contents_eq_str(File, ExpectedContents, FormatString, ...)       internal

/**
 * Passes if the contents of \c File are not equal to the string
 * \c UnexpectedContents
 *
 * Passes if the contents of \c File are not equal to the string
 * \c UnexpectedContents.
 * Otherwise the test is marked as failure and the execution of the function
 * is aborted.
 *
 * The optional string is printed on failure.
 *
 * @param[in] File Pointer to a FILE object that specifies an input stream
 * @param[in] UnexpectedContents C string with the UnexpectedContents
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_assert_file_contents_neq_str(File, UnexpectedContents, FormatString, ...)    internal

/**
 * Passes if the contents of \c File are not equal to the string
 * \c UnexpectedContents
 *
 * Passes if the contents of \c File are not equal to the string
 * \c UnexpectedContents.
 * Otherwise the test is marked as failure but the execution will continue.
 *
 * The optional string is printed on failure.
 *
 * @param[in] File Pointer to a FILE object that specifies an input stream
 * @param[in] UnexpectedContents C string with the UnexpectedContents
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_expect_file_contents_neq_str(File, UnexpectedContents, FormatString, ...)    internal

/**
 * Passes if the contents of \c File are equal to the contents of \c RefFile
 *
 * Passes if the contents of \c File are equal to the contents of \c RefFile.
 * Otherwise the test is marked as failure and the execution of the function
 * is aborted.
 *
 * The optional string is printed on failure.
 *
 * @param[in] File Pointer to a FILE object that specifies an input stream
 * @param[in] RefFile Pointer to a FILE object that specifies an input stream
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_assert_file_contents_eq(File, RefFile, FormatString, ...)                    internal

/**
 * Passes if the contents of \c File are equal to the contents of \c RefFile
 *
 * Passes if the contents of \c File are equal to the contents of \c RefFile.
 * Otherwise the test is marked as failure but the execution will continue.
 *
 * The optional string is printed on failure.
 *
 * @param[in] File Pointer to a FILE object that specifies an input stream
 * @param[in] RefFile Pointer to a FILE object that specifies an input stream
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_expect_file_contents_eq(File, RefFile, FormatString, ...)                    internal

/**
 * Passes if the contents of \c File are not equal to the contents of
 * \c RefFile
 *
 * Passes if the contents of \c File are not equal to the contents of
 * \c RefFile.
 * Otherwise the test is marked as failure and the execution of the function
 * is aborted.
 *
 * The optional string is printed on failure.
 *
 * @param[in] File Pointer to a FILE object that specifies an input stream
 * @param[in] RefFile Pointer to a FILE object that specifies an input stream
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_assert_file_contents_neq(File, RefFile, FormatString, ...)                   internal

/**
 * Passes if the contents of \c File are not equal to the contents of
 * \c RefFile
 *
 * Passes if the contents of \c File are not equal to the contents of
 * \c RefFile.
 * Otherwise the test is marked as failure but the execution will continue.
 *
 * The optional string is printed on failure.
 *
 * @param[in] File Pointer to a FILE object that specifies an input stream
 * @param[in] RefFile Pointer to a FILE object that specifies an input stream
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_expect_file_contents_neq(File, RefFile, FormatString, ...)                   internal

/**@}*/

/**
 * @defgroup StreamAsserts Standard stream assertions
 * @{
 */

/**
 * Passes if the contents of \c stdout are equal to the contents of the string
 * \c ExpectedContents
 *
 * Passes if the contents of \c stdout are equal to the contents of the string
 * \c ExpectedContents.
 * Otherwise the test is marked as failure and the execution of the function
 * is aborted.
 *
 * The optional string is printed on failure.
 *
 * @param[in] ExpectedContents C string with the ExpectedContents
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_assert_stdout_eq_str(ExpectedContents, FormatString, ...)                    internal

/**
 * Passes if the contents of \c stdout are equal to the contents of the string
 * \c ExpectedContents
 *
 * Passes if the contents of \c stdout are equal to the contents of the string
 * \c ExpectedContents.
 * Otherwise the test is marked as failure but the execution will continue.
 *
 * The optional string is printed on failure.
 *
 * @param[in] ExpectedContents C string with the ExpectedContents
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_expect_stdout_eq_str(ExpectedContents, FormatString, ...)                    internal

/**
 * Passes if the contents of \c stdout are not equal to the contents of the
 * string \c UnexpectedContents
 *
 * Passes if the contents of \c stdout are not equal to the contents of the
 * string \c UnexpectedContents.
 * Otherwise the test is marked as failure and the execution of the function
 * is aborted.
 *
 * The optional string is printed on failure.
 *
 * @param[in] UnexpectedContents C string with the UnexpectedContents
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_assert_stdout_neq_str(UnexpectedContents, FormatString, ...)                 internal

/**
 * Passes if the contents of \c stdout are not equal to the contents of the
 * string \c UnexpectedContents
 *
 * Passes if the contents of \c stdout are not equal to the contents of the
 * string \c UnexpectedContents.
 * Otherwise the test is marked as failure but the execution will continue.
 *
 * The optional string is printed on failure.
 *
 * @param[in] UnexpectedContents C string with the UnexpectedContents
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_expect_stdout_neq_str(UnexpectedContents, FormatString, ...)                 internal

/**
 * Passes if the contents of \c stderr are equal to the contents of the string
 * \c ExpectedContents
 *
 * Passes if the contents of \c stderr are equal to the contents of the string
 * \c ExpectedContents.
 * Otherwise the test is marked as failure and the execution of the function
 * is aborted.
 *
 * The optional string is printed on failure.
 *
 * @param[in] ExpectedContents C string with the ExpectedContents
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_assert_stderr_eq_str(ExpectedContents, FormatString, ...)                    internal

/**
 * Passes if the contents of \c stderr are equal to the contents of the string
 * \c ExpectedContents
 *
 * Passes if the contents of \c stderr are equal to the contents of the string
 * \c ExpectedContents.
 * Otherwise the test is marked as failure but the execution will continue.
 *
 * The optional string is printed on failure.
 *
 * @param[in] ExpectedContents C string with the ExpectedContents
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_expect_stderr_eq_str(ExpectedContents, FormatString, ...)                    internal

/**
 * Passes if the contents of \c stderr are not equal to the contents of the
 * string \c UnexpectedContents
 *
 * Passes if the contents of \c stderr are not equal to the contents of the
 * string \c UnexpectedContents.
 * Otherwise the test is marked as failure and the execution of the function
 * is aborted.
 *
 * The optional string is printed on failure.
 *
 * @param[in] UnexpectedContents C string with the UnexpectedContents
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_assert_stderr_neq_str(UnexpectedContents, FormatString, ...)                 internal

/**
 * Passes if the contents of \c stderr are not equal to the contents of the
 * string \c UnexpectedContents
 *
 * Passes if the contents of \c stderr are not equal to the contents of the
 * string \c UnexpectedContents.
 * Otherwise the test is marked as failure but the execution will continue.
 *
 * The optional string is printed on failure.
 *
 * @param[in] UnexpectedContents C string with the UnexpectedContents
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_expect_stderr_neq_str(UnexpectedContents, FormatString, ...)                 internal

/**
 * Passes if the contents of \c stdout are equal to the contents of \c RefFile
 *
 * Passes if the contents of \c stdout are equal to the contents of \c RefFile.
 * Otherwise the test is marked as failure and the execution of the function
 * is aborted.
 *
 * The optional string is printed on failure.
 *
 * @param[in] RefFile Pointer to a FILE object that specifies an input stream
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_assert_stdout_eq(RefFile, FormatString, ...)                                 internal

/**
 * Passes if the contents of \c stdout are equal to the contents of \c RefFile
 *
 * Passes if the contents of \c stdout are equal to the contents of \c RefFile.
 * Otherwise the test is marked as failure but the execution will continue.
 *
 * The optional string is printed on failure.
 *
 * @param[in] RefFile Pointer to a FILE object that specifies an input stream
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_expect_stdout_eq(RefFile, FormatString, ...)                                 internal

/**
 * Passes if the contents of \c stdout are not equal to the contents of \c
 * RefFile
 *
 * Passes if the contents of \c stdout are not equal to the contents of \c
 * RefFile.
 * Otherwise the test is marked as failure and the execution of the function
 * is aborted.
 *
 * The optional string is printed on failure.
 *
 * @param[in] RefFile Pointer to a FILE object that specifies an input stream
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_assert_stdout_neq(RefFile, FormatString, ...)                                internal

/**
 * Passes if the contents of \c stdout are not equal to the contents of \c
 * RefFile
 *
 * Passes if the contents of \c stdout are not equal to the contents of \c
 * RefFile.
 * Otherwise the test is marked as failure but the execution will continue.
 *
 * The optional string is printed on failure.
 *
 * @param[in] RefFile Pointer to a FILE object that specifies an input stream
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_expect_stdout_neq(RefFile, FormatString, ...)                                internal

/**
 * Passes if the contents of \c stderr are equal to the contents of \c RefFile
 *
 * Passes if the contents of \c stderr are equal to the contents of \c RefFile.
 * Otherwise the test is marked as failure and the execution of the function
 * is aborted.
 *
 * The optional string is printed on failure.
 *
 * @param[in] RefFile Pointer to a FILE object that specifies an input stream
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_assert_stderr_eq(RefFile, FormatString, ...)                                 internal

/**
 * Passes if the contents of \c stderr are equal to the contents of \c RefFile
 *
 * Passes if the contents of \c stderr are equal to the contents of \c RefFile.
 * Otherwise the test is marked as failure but the execution will continue.
 *
 * The optional string is printed on failure.
 *
 * @param[in] RefFile Pointer to a FILE object that specifies an input stream
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_expect_stderr_eq(RefFile, FormatString, ...)                                 internal

/**
 * Passes if the contents of \c stderr are not equal to the contents of \c
 * RefFile
 *
 * Passes if the contents of \c stderr are not equal to the contents of \c
 * RefFile.
 * Otherwise the test is marked as failure and the execution of the function
 * is aborted.
 *
 * The optional string is printed on failure.
 *
 * @param[in] RefFile Pointer to a FILE object that specifies an input stream
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_assert_stderr_neq(RefFile, FormatString, ...)                                internal

/**
 * Passes if the contents of \c stderr are not equal to the contents of \c
 * RefFile
 *
 * Passes if the contents of \c stderr are not equal to the contents of \c
 * RefFile.
 * Otherwise the test is marked as failure but the execution will continue.
 *
 * The optional string is printed on failure.
 *
 * @param[in] RefFile Pointer to a FILE object that specifies an input stream
 * @param[in] FormatString (optional) printf-like format string
 * @param[in] ... (optional) format string parameters
 *
 *****************************************************************************/
#define cr_expect_stderr_neq(RefFile, FormatString, ...)                                internal

/**@}*/

#ifdef __cplusplus
# include "internal/stream.hxx"

namespace criterion
{
typedef internal::basic_ofstream<char> ofstream;
typedef internal::basic_ifstream<char> ifstream;
typedef internal::basic_fstream<char> fstream;

static inline ofstream &get_redirected_cin(void)
{
    return internal::get_redirected_out_stream_::call(cr_get_redirected_stdin());
}

static inline ifstream &get_redirected_cout(void)
{
    return internal::get_redirected_in_stream_::call(cr_get_redirected_stdout());
}

static inline ifstream &get_redirected_cerr(void)
{
    return internal::get_redirected_in_stream_::call(cr_get_redirected_stderr());
}

# if __GNUC__ >= 5
static inline fstream mock_file(size_t max_size)
{
    return fstream(cr_mock_file_size(max_size));
}

static inline fstream mock_file(void)
{
    return mock_file(4096);
}
# endif
}
#endif

#include "internal/redirect.h"

#endif /* !CRITERION_REDIRECT_H_ */
