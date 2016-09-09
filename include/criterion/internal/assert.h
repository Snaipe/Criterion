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
#ifndef CRITERION_INTERNAL_ASSERT_H_
#define CRITERION_INTERNAL_ASSERT_H_

#include "common.h"
#include "preprocess.h"
#include "asprintf-compat.h"
#include "designated-initializer-compat.h"

#ifdef __cplusplus
# include <cstring>
# include <cstdlib>
# include <cwchar>
#else
# include <string.h>
# include <stdlib.h>
# include <stdbool.h>
# include <wchar.h>
#endif
#include "../types.h"
#include "../stats.h"
#include "../hooks.h"
#include "../event.h"
#include "../abort.h"

struct criterion_assert_args {
    const char *msg;
    int sentinel_;

#ifdef __cplusplus
    constexpr criterion_assert_args(const char *msg) : msg(msg), sentinel_(0) {}
    constexpr criterion_assert_args(const char *msg, int sentinel_) : msg(msg), sentinel_(sentinel_) {}
#endif
};

/* Do NOT reorder unless you want to break the ABI */
enum criterion_assert_messages {
    CRITERION_ASSERT_MSG_FAIL,
    CRITERION_ASSERT_MSG_EXPR_FALSE,
    CRITERION_ASSERT_MSG_EXPR_AS_STRINGS_FALSE,
    CRITERION_ASSERT_MSG_IS_NULL,
    CRITERION_ASSERT_MSG_IS_NOT_NULL,
    CRITERION_ASSERT_MSG_IS_EMPTY,
    CRITERION_ASSERT_MSG_IS_NOT_EMPTY,
    CRITERION_ASSERT_MSG_FILE_STR_MATCH,
    CRITERION_ASSERT_MSG_FILE_MATCH,
    CRITERION_ASSERT_MSG_THROW,
    CRITERION_ASSERT_MSG_NO_THROW,
    CRITERION_ASSERT_MSG_ANY_THROW,
    CRITERION_ASSERT_MSG_NONE_THROW,
};

CR_BEGIN_C_API

CR_API char *cr_translate_assert_msg(int msg_index, ...);

CR_END_C_API

#define CR_GET_CONDITION(Condition, ...)        Condition
#define CR_GET_CONDITION_STR(Condition, ...)    #Condition
#define CR_VA_SKIP(_, ...)                      __VA_ARGS__

#define CR_TRANSLATE_DEF_MSG__(Arg) \
    CR_IDENTITY Arg

#define CR_TRANSLATE_DEF_MSG_(...)                                             \
    CR_EXPAND(cr_translate_assert_msg(                                         \
                CR_VA_HEAD(__VA_ARGS__),                                       \
                "" CR_TRANSLATE_DEF_MSG__(CR_VA_HEAD(CR_VA_TAIL(__VA_ARGS__))) \
                ))

#define CR_INIT_STATS_(MsgVar, Shifted, ...)                                 \
    CR_EXPAND(                                                               \
        do {                                                                 \
        char *cr_def_msg__ = CR_EXPAND(CR_TRANSLATE_DEF_MSG_(__VA_ARGS__));  \
        char *cr_fmt_msg__ = NULL;                                           \
        cr_asprintf(&cr_fmt_msg__, "x" CR_VA_TAIL(CR_VA_TAIL(__VA_ARGS__))); \
        if (cr_fmt_msg__ && cr_fmt_msg__[1]) {                               \
            MsgVar = cr_fmt_msg__ + 1;                                       \
            Shifted = 1;                                                     \
            cr_asprintf_free(cr_def_msg__);                                  \
        } else {                                                             \
            MsgVar = cr_def_msg__;                                           \
            cr_asprintf_free(cr_fmt_msg__);                                  \
        }                                                                    \
    } while (0))

#define CR_FAIL_ABORT_        criterion_abort_test
#define CR_FAIL_CONTINUES_    criterion_continue_test

#define cr_assert_impl(Fail, Condition, ...)             \
    do {                                                 \
        bool cr_passed__ = !!(Condition);                \
        char *cr_msg__ = NULL;                           \
        int cr_shifted__ = 0;                            \
                                                         \
        CR_EXPAND(CR_INIT_STATS_(cr_msg__, cr_shifted__, \
                CR_VA_TAIL(__VA_ARGS__)));               \
        struct criterion_assert_stats cr_stat__;         \
        cr_stat__.passed = cr_passed__;                  \
        cr_stat__.file = __FILE__;                       \
        cr_stat__.line = __LINE__;                       \
        cr_stat__.message = cr_msg__;                    \
        criterion_send_assert(&cr_stat__);               \
                                                         \
        cr_asprintf_free(cr_msg__ - cr_shifted__);       \
                                                         \
        if (!cr_passed__)                                \
            Fail();                                      \
    } while (0)

#define cr_fail(Fail, ...)                 \
    CR_EXPAND(cr_assert_impl(              \
                Fail,                      \
                0,                         \
                dummy,                     \
                CRITERION_ASSERT_MSG_FAIL, \
                (),                        \
                __VA_ARGS__                \
                ))

#define cr_skip(...) \
    criterion_skip_test("" __VA_ARGS__)

#define cr_assert_(...)                            \
    CR_EXPAND(cr_assert_impl(                      \
                CR_FAIL_ABORT_,                    \
                CR_VA_HEAD(__VA_ARGS__),           \
                dummy,                             \
                CRITERION_ASSERT_MSG_EXPR_FALSE,   \
                (CR_STR(CR_VA_HEAD(__VA_ARGS__))), \
                CR_VA_TAIL(__VA_ARGS__)            \
                ))

#define cr_expect_(...)                            \
    CR_EXPAND(cr_assert_impl(                      \
                CR_FAIL_CONTINUES_,                \
                CR_VA_HEAD(__VA_ARGS__),           \
                dummy,                             \
                CRITERION_ASSERT_MSG_EXPR_FALSE,   \
                (CR_STR(CR_VA_HEAD(__VA_ARGS__))), \
                CR_VA_TAIL(__VA_ARGS__)            \
                ))

#define cr_assert_not_(...)                           \
    CR_EXPAND(cr_assert_impl(                         \
                CR_FAIL_ABORT_,                       \
                !(CR_VA_HEAD(__VA_ARGS__)),           \
                dummy,                                \
                CRITERION_ASSERT_MSG_EXPR_FALSE,      \
                (CR_STR(!(CR_VA_HEAD(__VA_ARGS__)))), \
                CR_VA_TAIL(__VA_ARGS__)               \
                ))

#define cr_expect_not_(...)                           \
    CR_EXPAND(cr_assert_impl(                         \
                CR_FAIL_CONTINUES_,                   \
                !(CR_VA_HEAD(__VA_ARGS__)),           \
                dummy,                                \
                CRITERION_ASSERT_MSG_EXPR_FALSE,      \
                (CR_STR(!(CR_VA_HEAD(__VA_ARGS__)))), \
                CR_VA_TAIL(__VA_ARGS__)               \
                ))

/* Binary */

#define cr_assert_op_(Fail, Op, Actual, Expected, ...) \
    CR_EXPAND(cr_assert_impl(                          \
                Fail,                                  \
                (Actual) Op (Expected),                \
                dummy,                                 \
                CRITERION_ASSERT_MSG_EXPR_FALSE,       \
                (CR_STR((Actual) Op (Expected))),      \
                __VA_ARGS__                            \
                ))

#define cr_assert_op_va_(Fail, Op, ...)              \
    CR_EXPAND(cr_assert_op_(                         \
                Fail,                                \
                Op,                                  \
                CR_VA_HEAD(__VA_ARGS__),             \
                CR_VA_HEAD(CR_VA_TAIL(__VA_ARGS__)), \
                CR_VA_TAIL(CR_VA_TAIL(__VA_ARGS__))  \
                ))

/* Unary */

#define cr_assert_null_op_(Fail, Op, Msg, Value, ...) \
    CR_EXPAND(cr_assert_impl(                         \
                Fail,                                 \
                (Value) Op NULL,                      \
                dummy,                                \
                Msg,                                  \
                (CR_STR(Value)),                      \
                __VA_ARGS__                           \
                ))

#define cr_assert_null_op_va_(Fail, Op, Msg, ...) \
    CR_EXPAND(cr_assert_null_op_(                 \
                Fail,                             \
                Op,                               \
                Msg,                              \
                CR_VA_HEAD(__VA_ARGS__),          \
                CR_VA_TAIL(__VA_ARGS__)           \
                ))

/* Floating point */

#define cr_assert_float_eq_op_(Actual, Expected, Epsilon) \
    (Expected) - (Actual) <= (Epsilon) && (Actual) - (Expected) <= (Epsilon)

#define cr_assert_float_neq_op_(Actual, Expected, Epsilon) \
    (Expected) - (Actual) > (Epsilon) || (Actual) - (Expected) > (Epsilon)

#define cr_assert_float_op_(Fail, Op, Actual, Expected, Epsilon, ...) \
    CR_EXPAND(cr_assert_impl(                                         \
                Fail,                                                 \
                Op (Actual, Expected, Epsilon),                       \
                dummy,                                                \
                CRITERION_ASSERT_MSG_EXPR_FALSE,                      \
                (CR_STR(Op (Actual, Expected, Epsilon))),             \
                __VA_ARGS__                                           \
                ))

#define cr_assert_float_op_va_(Fail, Op, ...)                    \
    CR_EXPAND(cr_assert_float_op_(                               \
                Fail,                                            \
                Op,                                              \
                CR_VA_HEAD(__VA_ARGS__),                         \
                CR_VA_HEAD(CR_VA_TAIL(__VA_ARGS__)),             \
                CR_VA_HEAD(CR_VA_TAIL(CR_VA_TAIL(__VA_ARGS__))), \
                CR_VA_TAIL(CR_VA_TAIL(CR_VA_TAIL(__VA_ARGS__)))  \
                ))

/* String */

#define cr_assert_str_op_empty_(Fail, Op, Msg, Value, ...)                           \
    do {                                                                             \
        const char *cr_str_value__ = (Value);                                        \
        CR_EXPAND(cr_assert_impl(                                                    \
                    Fail,                                                            \
                    ((cr_str_value__) != NULL) && (cr_str_value__)[0] Op '\0',       \
                    dummy,                                                           \
                    ((cr_str_value__) != NULL) ? Msg : CRITERION_ASSERT_MSG_IS_NULL, \
                    (CR_STR(Value), cr_str_value__),                                 \
                    __VA_ARGS__                                                      \
                    ));                                                              \
    } while (0)

#define cr_assert_str_op_empty_va_(Fail, Op, Msg, ...) \
    CR_EXPAND(cr_assert_str_op_empty_(                 \
                Fail,                                  \
                Op,                                    \
                Msg,                                   \
                CR_VA_HEAD(__VA_ARGS__),               \
                CR_VA_TAIL(__VA_ARGS__)                \
                ))

#define cr_assert_str_op_(Fail, Op, Actual, Expected, ...)                                \
    do {                                                                                  \
        const char *cr_str_actual__ = (Actual);                                           \
        const char *cr_str_expected__ = (Expected);                                       \
        CR_EXPAND(cr_assert_impl(                                                         \
                    Fail,                                                                 \
                    ((cr_str_actual__) != NULL) && ((cr_str_expected__) != NULL)          \
                    && CR_STDN strcmp((cr_str_actual__), (cr_str_expected__)) Op 0,       \
                    dummy,                                                                \
                    CRITERION_ASSERT_MSG_EXPR_AS_STRINGS_FALSE,                           \
                    (CR_STR((Actual) Op (Expected)), cr_str_actual__, cr_str_expected__), \
                    __VA_ARGS__                                                           \
                    ));                                                                   \
    } while (0)

#define cr_assert_str_op_va_(Fail, Op, ...)          \
    CR_EXPAND(cr_assert_str_op_(                     \
                Fail,                                \
                Op,                                  \
                CR_VA_HEAD(__VA_ARGS__),             \
                CR_VA_HEAD(CR_VA_TAIL(__VA_ARGS__)), \
                CR_VA_TAIL(CR_VA_TAIL(__VA_ARGS__))  \
                ))

/* Wide String */

#define cr_assert_wcs_op_empty_(Fail, Op, Msg, Value, ...)                           \
    do {                                                                             \
        const wchar_t *cr_wcs_value__ = (Value);                                     \
        CR_EXPAND(cr_assert_impl(                                                    \
                    Fail,                                                            \
                    ((cr_wcs_value__) != NULL) && (cr_wcs_value__)[0] Op L'\0',      \
                    dummy,                                                           \
                    ((cr_wcs_value__) != NULL) ? Msg : CRITERION_ASSERT_MSG_IS_NULL, \
                    (CR_STR(Value), cr_wcs_value__),                                 \
                    __VA_ARGS__                                                      \
                    ));                                                              \
    } while (0)

#define cr_assert_wcs_op_empty_va_(Fail, Op, Msg, ...) \
    CR_EXPAND(cr_assert_wcs_op_empty_(                 \
                Fail,                                  \
                Op,                                    \
                Msg,                                   \
                CR_VA_HEAD(__VA_ARGS__),               \
                CR_VA_TAIL(__VA_ARGS__)                \
                ))

#define cr_assert_wcs_op_(Fail, Op, Actual, Expected, ...)                          \
    do {                                                                            \
        const wchar_t *cr_wcs_actual__ = (Actual);                                  \
        const wchar_t *cr_wcs_expected__ = (Expected);                              \
        CR_EXPAND(cr_assert_impl(                                                   \
                    Fail,                                                           \
                    ((cr_wcs_actual__) != NULL) && ((cr_wcs_expected__) != NULL)    \
                    && CR_STDN wcscmp((cr_wcs_actual__), (cr_wcs_expected__)) Op 0, \
                    dummy,                                                          \
                    CRITERION_ASSERT_MSG_EXPR_FALSE,                                \
                    (CR_STR((Actual) Op (Expected))),                               \
                    __VA_ARGS__                                                     \
                    ));                                                             \
    } while (0)

#define cr_assert_wcs_op_va_(Fail, Op, ...)          \
    CR_EXPAND(cr_assert_wcs_op_(                     \
                Fail,                                \
                Op,                                  \
                CR_VA_HEAD(__VA_ARGS__),             \
                CR_VA_HEAD(CR_VA_TAIL(__VA_ARGS__)), \
                CR_VA_TAIL(CR_VA_TAIL(__VA_ARGS__))  \
                ))

/* Array */

#define cr_assert_mem_op_(Fail, Op, Actual, Expected, Size, ...)    \
    CR_EXPAND(cr_assert_impl(                                       \
                Fail,                                               \
                CR_STDN memcmp((Actual), (Expected), (Size)) Op 0,  \
                dummy,                                              \
                CRITERION_ASSERT_MSG_EXPR_FALSE,                    \
                (CR_STR((Actual)[0..Size] Op (Expected)[0..Size])), \
                __VA_ARGS__                                         \
                ))

#define cr_assert_mem_op_va_(Fail, Op, ...)                      \
    CR_EXPAND(cr_assert_mem_op_(                                 \
                Fail,                                            \
                Op,                                              \
                CR_VA_HEAD(__VA_ARGS__),                         \
                CR_VA_HEAD(CR_VA_TAIL(__VA_ARGS__)),             \
                CR_VA_HEAD(CR_VA_TAIL(CR_VA_TAIL(__VA_ARGS__))), \
                CR_VA_TAIL(CR_VA_TAIL(CR_VA_TAIL(__VA_ARGS__)))  \
                ))

/* Array comparisons */

#ifdef __cplusplus
# define CR_ARR_COMPARE_(A, B, Size, Cmp, Result) \
    int Result = std::lexicographical_compare((A), (A) + Size, (B), (B) + Size, Cmp)
#else
# define CR_ARR_COMPARE_(A, B, Size, Cmp, Result)        \
    __typeof__ (&(A)[0])first = (A);                     \
    __typeof__ (&(B)[0])second = (B);                    \
    int Result = 0;                                      \
    size_t i, size;                                      \
    for (i = 0, size = (Size); !Result && i < size; ++i) \
        Result = Cmp(first + i, second + i)
#endif

#define cr_assert_arr_op_cmp_(Fail, Op, Actual, Expected, Size, Cmp, ...) \
    do {                                                                  \
        CR_ARR_COMPARE_(Actual, Expected, Size, Cmp, order);              \
        CR_EXPAND(cr_assert_impl(                                         \
                    Fail,                                                 \
                    order Op 0,                                           \
                    dummy,                                                \
                    CRITERION_ASSERT_MSG_EXPR_FALSE,                      \
                    (CR_STR((Actual)[0..Size] Op (Expected)[0..Size])),   \
                    __VA_ARGS__                                           \
                    ));                                                   \
    } while (0)

#define cr_assert_arr_op_cmp_va_(Fail, Op, ...)                              \
    CR_EXPAND(cr_assert_arr_op_cmp_(                                         \
                Fail,                                                        \
                Op,                                                          \
                CR_VA_HEAD(__VA_ARGS__),                                     \
                CR_VA_HEAD(CR_VA_TAIL(__VA_ARGS__)),                         \
                CR_VA_HEAD(CR_VA_TAIL(CR_VA_TAIL(__VA_ARGS__))),             \
                CR_VA_HEAD(CR_VA_TAIL(CR_VA_TAIL(CR_VA_TAIL(__VA_ARGS__)))), \
                CR_VA_TAIL(CR_VA_TAIL(CR_VA_TAIL(CR_VA_TAIL(__VA_ARGS__))))  \
                ))

/* Exceptions */

#define cr_assert_throw_abort_(Fail, Msg, MsgArgs, ...) \
    CR_EXPAND(cr_assert_impl(                           \
                Fail,                                   \
                0,                                      \
                dummy,                                  \
                Msg,                                    \
                MsgArgs,                                \
                CR_VA_TAIL(__VA_ARGS__)                 \
                ))

#define cr_assert_throw_(Fail, Statement, Exception, ...)   \
    try {                                                   \
        Statement;                                          \
    } catch (Exception const &) {                           \
    } catch (...) {                                         \
        CR_EXPAND(cr_assert_throw_abort_(                   \
                    Fail,                                   \
                    CRITERION_ASSERT_MSG_NO_THROW,          \
                    (CR_STR(Statement), CR_STR(Exception)), \
                    __VA_ARGS__));                          \
    }

#define cr_assert_throw_va_(...)                                 \
    CR_EXPAND(cr_assert_throw_(                                  \
                CR_VA_HEAD(__VA_ARGS__),                         \
                CR_VA_HEAD(CR_VA_TAIL(__VA_ARGS__)),             \
                CR_VA_HEAD(CR_VA_TAIL(CR_VA_TAIL(__VA_ARGS__))), \
                dummy,                                           \
                CR_VA_TAIL(CR_VA_TAIL(CR_VA_TAIL(__VA_ARGS__)))  \
                ))

#define cr_assert_no_throw_(Fail, Statement, Exception, ...) \
    try {                                                    \
        Statement;                                           \
    } catch (Exception const &) {                            \
        CR_EXPAND(cr_assert_throw_abort_(                    \
                    Fail,                                    \
                    CRITERION_ASSERT_MSG_THROW,              \
                    (CR_STR(Statement), CR_STR(Exception)),  \
                    __VA_ARGS__));                           \
    }

#define cr_assert_no_throw_va_(...)                              \
    CR_EXPAND(cr_assert_no_throw_(                               \
                CR_VA_HEAD(__VA_ARGS__),                         \
                CR_VA_HEAD(CR_VA_TAIL(__VA_ARGS__)),             \
                CR_VA_HEAD(CR_VA_TAIL(CR_VA_TAIL(__VA_ARGS__))), \
                dummy,                                           \
                CR_VA_TAIL(CR_VA_TAIL(CR_VA_TAIL(__VA_ARGS__)))  \
                ))

#define cr_assert_any_throw_(Fail, Statement, ...)  \
    try {                                           \
        Statement;                                  \
        CR_EXPAND(cr_assert_throw_abort_(           \
                    Fail,                           \
                    CRITERION_ASSERT_MSG_ANY_THROW, \
                    (CR_STR(Statement)),            \
                    __VA_ARGS__));                  \
    } catch (...) {}

#define cr_assert_any_throw_va_(...)                 \
    CR_EXPAND(cr_assert_any_throw_(                  \
                CR_VA_HEAD(__VA_ARGS__),             \
                CR_VA_HEAD(CR_VA_TAIL(__VA_ARGS__)), \
                dummy,                               \
                CR_VA_TAIL(CR_VA_TAIL(__VA_ARGS__))  \
                ))

#define cr_assert_none_throw_(Fail, Statement, ...)  \
    try {                                            \
        Statement;                                   \
    } catch (...) {                                  \
        CR_EXPAND(cr_assert_throw_abort_(            \
                    Fail,                            \
                    CRITERION_ASSERT_MSG_NONE_THROW, \
                    (CR_STR(Statement)),             \
                    __VA_ARGS__));                   \
    }

#define cr_assert_none_throw_va_(...)                \
    CR_EXPAND(cr_assert_none_throw_(                 \
                CR_VA_HEAD(__VA_ARGS__),             \
                CR_VA_HEAD(CR_VA_TAIL(__VA_ARGS__)), \
                dummy,                               \
                CR_VA_TAIL(CR_VA_TAIL(__VA_ARGS__))  \
                ))

/* Messages */

#define CRITERION_GNUC_WARN__(Msg) \
    _Pragma(#Msg)

#define CRITERION_GNUC_WARN_(Name)                                    \
    CRITERION_GNUC_WARN__(                                            \
        message                                                       \
        "The `" #Name "` macro is only available on GNU C compilers." \
        )

/* Actual implementation */

#undef cr_skip_test
#define cr_skip_test(...)               CR_EXPAND(cr_skip(__VA_ARGS__))
#undef cr_assert_fail
#define cr_assert_fail(...)             CR_EXPAND(cr_fail(CR_FAIL_ABORT_, __VA_ARGS__))
#undef cr_expect_fail
#define cr_expect_fail(...)             CR_EXPAND(cr_fail(CR_FAIL_CONTINUES_, __VA_ARGS__))
#undef cr_assert
#define cr_assert(...)                  CR_EXPAND(cr_assert_(__VA_ARGS__))
#undef cr_expect
#define cr_expect(...)                  CR_EXPAND(cr_expect_(__VA_ARGS__))
#undef cr_assert_not
#define cr_assert_not(...)              CR_EXPAND(cr_assert_not_(__VA_ARGS__))
#undef cr_expect_not
#define cr_expect_not(...)              CR_EXPAND(cr_expect_not_(__VA_ARGS__))
#undef cr_assert_eq
#define cr_assert_eq(...)               CR_EXPAND(cr_assert_op_va_(CR_FAIL_ABORT_, ==, __VA_ARGS__))
#undef cr_expect_eq
#define cr_expect_eq(...)               CR_EXPAND(cr_assert_op_va_(CR_FAIL_CONTINUES_, ==, __VA_ARGS__))
#undef cr_assert_neq
#define cr_assert_neq(...)              CR_EXPAND(cr_assert_op_va_(CR_FAIL_ABORT_, !=, __VA_ARGS__))
#undef cr_expect_neq
#define cr_expect_neq(...)              CR_EXPAND(cr_assert_op_va_(CR_FAIL_CONTINUES_, !=, __VA_ARGS__))
#undef cr_assert_lt
#define cr_assert_lt(...)               CR_EXPAND(cr_assert_op_va_(CR_FAIL_ABORT_, <, __VA_ARGS__))
#undef cr_expect_lt
#define cr_expect_lt(...)               CR_EXPAND(cr_assert_op_va_(CR_FAIL_CONTINUES_, <, __VA_ARGS__))
#undef cr_assert_leq
#define cr_assert_leq(...)              CR_EXPAND(cr_assert_op_va_(CR_FAIL_ABORT_, <=, __VA_ARGS__))
#undef cr_expect_leq
#define cr_expect_leq(...)              CR_EXPAND(cr_assert_op_va_(CR_FAIL_CONTINUES_, <=, __VA_ARGS__))
#undef cr_assert_gt
#define cr_assert_gt(...)               CR_EXPAND(cr_assert_op_va_(CR_FAIL_ABORT_, >, __VA_ARGS__))
#undef cr_expect_gt
#define cr_expect_gt(...)               CR_EXPAND(cr_assert_op_va_(CR_FAIL_CONTINUES_, >, __VA_ARGS__))
#undef cr_assert_geq
#define cr_assert_geq(...)              CR_EXPAND(cr_assert_op_va_(CR_FAIL_ABORT_, >=, __VA_ARGS__))
#undef cr_expect_geq
#define cr_expect_geq(...)              CR_EXPAND(cr_assert_op_va_(CR_FAIL_CONTINUES_, >=, __VA_ARGS__))
#undef cr_assert_null
#define cr_assert_null(...)             CR_EXPAND(cr_assert_null_op_va_(CR_FAIL_ABORT_, ==, CRITERION_ASSERT_MSG_IS_NOT_NULL, __VA_ARGS__))
#undef cr_expect_null
#define cr_expect_null(...)             CR_EXPAND(cr_assert_null_op_va_(CR_FAIL_CONTINUES_, ==, CRITERION_ASSERT_MSG_IS_NOT_NULL, __VA_ARGS__))
#undef cr_assert_not_null
#define cr_assert_not_null(...)         CR_EXPAND(cr_assert_null_op_va_(CR_FAIL_ABORT_, !=, CRITERION_ASSERT_MSG_IS_NULL, __VA_ARGS__))
#undef cr_expect_not_null
#define cr_expect_not_null(...)         CR_EXPAND(cr_assert_null_op_va_(CR_FAIL_CONTINUES_, !=, CRITERION_ASSERT_MSG_IS_NULL, __VA_ARGS__))
#undef cr_assert_float_eq
#define cr_assert_float_eq(...)         CR_EXPAND(cr_assert_float_op_va_(CR_FAIL_ABORT_, cr_assert_float_eq_op_, __VA_ARGS__))
#undef cr_expect_float_eq
#define cr_expect_float_eq(...)         CR_EXPAND(cr_assert_float_op_va_(CR_FAIL_CONTINUES_, cr_assert_float_eq_op_, __VA_ARGS__))
#undef cr_assert_float_neq
#define cr_assert_float_neq(...)        CR_EXPAND(cr_assert_float_op_va_(CR_FAIL_ABORT_, cr_assert_float_neq_op_, __VA_ARGS__))
#undef cr_expect_float_neq
#define cr_expect_float_neq(...)        CR_EXPAND(cr_assert_float_op_va_(CR_FAIL_CONTINUES_, cr_assert_float_neq_op_, __VA_ARGS__))
#undef cr_assert_str_empty
#define cr_assert_str_empty(...)        CR_EXPAND(cr_assert_str_op_empty_va_(CR_FAIL_ABORT_, ==, CRITERION_ASSERT_MSG_IS_NOT_EMPTY, __VA_ARGS__))
#undef cr_expect_str_empty
#define cr_expect_str_empty(...)        CR_EXPAND(cr_assert_str_op_empty_va_(CR_FAIL_CONTINUES_, ==, CRITERION_ASSERT_MSG_IS_NOT_EMPTY, __VA_ARGS__))
#undef cr_assert_str_not_empty
#define cr_assert_str_not_empty(...)    CR_EXPAND(cr_assert_str_op_empty_va_(CR_FAIL_ABORT_, !=, CRITERION_ASSERT_MSG_IS_EMPTY, __VA_ARGS__))
#undef cr_expect_str_not_empty
#define cr_expect_str_not_empty(...)    CR_EXPAND(cr_assert_str_op_empty_va_(CR_FAIL_CONTINUES_, !=, CRITERION_ASSERT_MSG_IS_EMPTY, __VA_ARGS__))
#undef cr_assert_str_eq
#define cr_assert_str_eq(...)           CR_EXPAND(cr_assert_str_op_va_(CR_FAIL_ABORT_, ==, __VA_ARGS__))
#undef cr_expect_str_eq
#define cr_expect_str_eq(...)           CR_EXPAND(cr_assert_str_op_va_(CR_FAIL_CONTINUES_, ==, __VA_ARGS__))
#undef cr_assert_str_neq
#define cr_assert_str_neq(...)          CR_EXPAND(cr_assert_str_op_va_(CR_FAIL_ABORT_, !=, __VA_ARGS__))
#undef cr_expect_str_neq
#define cr_expect_str_neq(...)          CR_EXPAND(cr_assert_str_op_va_(CR_FAIL_CONTINUES_, !=, __VA_ARGS__))
#undef cr_assert_str_lt
#define cr_assert_str_lt(...)           CR_EXPAND(cr_assert_str_op_va_(CR_FAIL_ABORT_, <, __VA_ARGS__))
#undef cr_expect_str_lt
#define cr_expect_str_lt(...)           CR_EXPAND(cr_assert_str_op_va_(CR_FAIL_CONTINUES_, <, __VA_ARGS__))
#undef cr_assert_str_leq
#define cr_assert_str_leq(...)          CR_EXPAND(cr_assert_str_op_va_(CR_FAIL_ABORT_, <=, __VA_ARGS__))
#undef cr_expect_str_leq
#define cr_expect_str_leq(...)          CR_EXPAND(cr_assert_str_op_va_(CR_FAIL_CONTINUES_, <=, __VA_ARGS__))
#undef cr_assert_str_gt
#define cr_assert_str_gt(...)           CR_EXPAND(cr_assert_str_op_va_(CR_FAIL_ABORT_, >, __VA_ARGS__))
#undef cr_expect_str_gt
#define cr_expect_str_gt(...)           CR_EXPAND(cr_assert_str_op_va_(CR_FAIL_CONTINUES_, >, __VA_ARGS__))
#undef cr_assert_str_geq
#define cr_assert_str_geq(...)          CR_EXPAND(cr_assert_str_op_va_(CR_FAIL_ABORT_, >=, __VA_ARGS__))
#undef cr_expect_str_geq
#define cr_expect_str_geq(...)          CR_EXPAND(cr_assert_str_op_va_(CR_FAIL_CONTINUES_, >=, __VA_ARGS__))
#undef cr_assert_wcs_empty
#define cr_assert_wcs_empty(...)        CR_EXPAND(cr_assert_wcs_op_empty_va_(CR_FAIL_ABORT_, ==, CRITERION_ASSERT_MSG_IS_NOT_EMPTY, __VA_ARGS__))
#undef cr_expect_wcs_empty
#define cr_expect_wcs_empty(...)        CR_EXPAND(cr_assert_wcs_op_empty_va_(CR_FAIL_CONTINUES_, ==, CRITERION_ASSERT_MSG_IS_NOT_EMPTY, __VA_ARGS__))
#undef cr_assert_wcs_not_empty
#define cr_assert_wcs_not_empty(...)    CR_EXPAND(cr_assert_wcs_op_empty_va_(CR_FAIL_ABORT_, !=, CRITERION_ASSERT_MSG_IS_EMPTY, __VA_ARGS__))
#undef cr_expect_wcs_not_empty
#define cr_expect_wcs_not_empty(...)    CR_EXPAND(cr_assert_wcs_op_empty_va_(CR_FAIL_CONTINUES_, !=, CRITERION_ASSERT_MSG_IS_EMPTY, __VA_ARGS__))
#undef cr_assert_wcs_eq
#define cr_assert_wcs_eq(...)           CR_EXPAND(cr_assert_wcs_op_va_(CR_FAIL_ABORT_, ==, __VA_ARGS__))
#undef cr_expect_wcs_eq
#define cr_expect_wcs_eq(...)           CR_EXPAND(cr_assert_wcs_op_va_(CR_FAIL_CONTINUES_, ==, __VA_ARGS__))
#undef cr_assert_wcs_neq
#define cr_assert_wcs_neq(...)          CR_EXPAND(cr_assert_wcs_op_va_(CR_FAIL_ABORT_, !=, __VA_ARGS__))
#undef cr_expect_wcs_neq
#define cr_expect_wcs_neq(...)          CR_EXPAND(cr_assert_wcs_op_va_(CR_FAIL_CONTINUES_, !=, __VA_ARGS__))
#undef cr_assert_wcs_lt
#define cr_assert_wcs_lt(...)           CR_EXPAND(cr_assert_wcs_op_va_(CR_FAIL_ABORT_, <, __VA_ARGS__))
#undef cr_expect_wcs_lt
#define cr_expect_wcs_lt(...)           CR_EXPAND(cr_assert_wcs_op_va_(CR_FAIL_CONTINUES_, <, __VA_ARGS__))
#undef cr_assert_wcs_leq
#define cr_assert_wcs_leq(...)          CR_EXPAND(cr_assert_wcs_op_va_(CR_FAIL_ABORT_, <=, __VA_ARGS__))
#undef cr_expect_wcs_leq
#define cr_expect_wcs_leq(...)          CR_EXPAND(cr_assert_wcs_op_va_(CR_FAIL_CONTINUES_, <=, __VA_ARGS__))
#undef cr_assert_wcs_gt
#define cr_assert_wcs_gt(...)           CR_EXPAND(cr_assert_wcs_op_va_(CR_FAIL_ABORT_, >, __VA_ARGS__))
#undef cr_expect_wcs_gt
#define cr_expect_wcs_gt(...)           CR_EXPAND(cr_assert_wcs_op_va_(CR_FAIL_CONTINUES_, >, __VA_ARGS__))
#undef cr_assert_wcs_geq
#define cr_assert_wcs_geq(...)          CR_EXPAND(cr_assert_wcs_op_va_(CR_FAIL_ABORT_, >=, __VA_ARGS__))
#undef cr_expect_wcs_geq
#define cr_expect_wcs_geq(...)          CR_EXPAND(cr_assert_wcs_op_va_(CR_FAIL_CONTINUES_, >=, __VA_ARGS__))
#undef cr_assert_arr_eq
#define cr_assert_arr_eq(...)           CR_EXPAND(cr_assert_mem_op_va_(CR_FAIL_ABORT_, ==, __VA_ARGS__))
#undef cr_expect_arr_eq
#define cr_expect_arr_eq(...)           CR_EXPAND(cr_assert_mem_op_va_(CR_FAIL_CONTINUES_, ==, __VA_ARGS__))
#undef cr_assert_arr_neq
#define cr_assert_arr_neq(...)          CR_EXPAND(cr_assert_mem_op_va_(CR_FAIL_ABORT_, !=, __VA_ARGS__))
#undef cr_expect_arr_neq
#define cr_expect_arr_neq(...)          CR_EXPAND(cr_assert_mem_op_va_(CR_FAIL_CONTINUES_, !=, __VA_ARGS__))

#if defined (__GNUC__) || defined (__clang__) || defined (__cplusplus)

# undef cr_assert_arr_eq_cmp
# define cr_assert_arr_eq_cmp(...)      CR_EXPAND(cr_assert_arr_op_cmp_va_(CR_FAIL_ABORT_, ==, __VA_ARGS__))
# undef cr_expect_arr_eq_cmp
# define cr_expect_arr_eq_cmp(...)      CR_EXPAND(cr_assert_arr_op_cmp_va_(CR_FAIL_CONTINUES_, ==, __VA_ARGS__))
# undef cr_assert_arr_neq_cmp
# define cr_assert_arr_neq_cmp(...)     CR_EXPAND(cr_assert_arr_op_cmp_va_(CR_FAIL_ABORT_, !=, __VA_ARGS__))
# undef cr_expect_arr_neq_cmp
# define cr_expect_arr_neq_cmp(...)     CR_EXPAND(cr_assert_arr_op_cmp_va_(CR_FAIL_CONTINUES_, !=, __VA_ARGS__))
# undef cr_assert_arr_lt_cmp
# define cr_assert_arr_lt_cmp(...)      CR_EXPAND(cr_assert_arr_op_cmp_va_(CR_FAIL_ABORT_, <, __VA_ARGS__))
# undef cr_expect_arr_lt_cmp
# define cr_expect_arr_lt_cmp(...)      CR_EXPAND(cr_assert_arr_op_cmp_va_(CR_FAIL_CONTINUES_, <, __VA_ARGS__))
# undef cr_assert_arr_leq_cmp
# define cr_assert_arr_leq_cmp(...)     CR_EXPAND(cr_assert_arr_op_cmp_va_(CR_FAIL_ABORT_, <=, __VA_ARGS__))
# undef cr_expect_arr_leq_cmp
# define cr_expect_arr_leq_cmp(...)     CR_EXPAND(cr_assert_arr_op_cmp_va_(CR_FAIL_CONTINUES_, <=, __VA_ARGS__))
# undef cr_assert_arr_gt_cmp
# define cr_assert_arr_gt_cmp(...)      CR_EXPAND(cr_assert_arr_op_cmp_va_(CR_FAIL_ABORT_, >, __VA_ARGS__))
# undef cr_expect_arr_gt_cmp
# define cr_expect_arr_gt_cmp(...)      CR_EXPAND(cr_assert_arr_op_cmp_va_(CR_FAIL_CONTINUES_, >, __VA_ARGS__))
# undef cr_assert_arr_geq_cmp
# define cr_assert_arr_geq_cmp(...)     CR_EXPAND(cr_assert_arr_op_cmp_va_(CR_FAIL_ABORT_, >=, __VA_ARGS__))
# undef cr_expect_arr_geq_cmp
# define cr_expect_arr_geq_cmp(...)     CR_EXPAND(cr_assert_arr_op_cmp_va_(CR_FAIL_CONTINUES_, >=, __VA_ARGS__))

#else

# undef cr_assert_arr_eq_cmp
# define cr_assert_arr_eq_cmp(...)      CRITERION_GNUC_WARN_(cr_assert_arr_eq_cmp) CR_NOOP
# undef cr_expect_arr_eq_cmp
# define cr_expect_arr_eq_cmp(...)      CRITERION_GNUC_WARN_(cr_expect_arr_eq_cmp) CR_NOOP
# undef cr_assert_arr_neq_cmp
# define cr_assert_arr_neq_cmp(...)     CRITERION_GNUC_WARN_(cr_assert_arr_neq_cmp) CR_NOOP
# undef cr_expect_arr_neq_cmp
# define cr_expect_arr_neq_cmp(...)     CRITERION_GNUC_WARN_(cr_expect_arr_neq_cmp) CR_NOOP
# undef cr_assert_arr_lt_cmp
# define cr_assert_arr_lt_cmp(...)      CRITERION_GNUC_WARN_(cr_assert_arr_lt_cmp) CR_NOOP
# undef cr_expect_arr_lt_cmp
# define cr_expect_arr_lt_cmp(...)      CRITERION_GNUC_WARN_(cr_expect_arr_lt_cmp) CR_NOOP
# undef cr_assert_arr_leq_cmp
# define cr_assert_arr_leq_cmp(...)     CRITERION_GNUC_WARN_(cr_assert_arr_leq_cmp) CR_NOOP
# undef cr_expect_arr_leq_cmp
# define cr_expect_arr_leq_cmp(...)     CRITERION_GNUC_WARN_(cr_expect_arr_leq_cmp) CR_NOOP
# undef cr_assert_arr_gt_cmp
# define cr_assert_arr_gt_cmp(...)      CRITERION_GNUC_WARN_(cr_assert_arr_gt_cmp) CR_NOOP
# undef cr_expect_arr_gt_cmp
# define cr_expect_arr_gt_cmp(...)      CRITERION_GNUC_WARN_(cr_expect_arr_gt_cmp) CR_NOOP
# undef cr_assert_arr_geq_cmp
# define cr_assert_arr_geq_cmp(...)     CRITERION_GNUC_WARN_(cr_assert_arr_geq_cmp) CR_NOOP
# undef cr_expect_arr_geq_cmp
# define cr_expect_arr_geq_cmp(...)     CRITERION_GNUC_WARN_(cr_expect_arr_geq_cmp) CR_NOOP

#endif

#undef cr_assert_throw
#define cr_assert_throw(...)            CR_EXPAND(cr_assert_throw_va_(CR_FAIL_ABORT_, __VA_ARGS__))
#undef cr_expect_throw
#define cr_expect_throw(...)            CR_EXPAND(cr_assert_throw_va_(CR_FAIL_CONTINUES_, __VA_ARGS__))
#undef cr_assert_no_throw
#define cr_assert_no_throw(...)         CR_EXPAND(cr_assert_no_throw_va_(CR_FAIL_ABORT_, __VA_ARGS__))
#undef cr_expect_no_throw
#define cr_expect_no_throw(...)         CR_EXPAND(cr_assert_no_throw_va_(CR_FAIL_CONTINUES_, __VA_ARGS__))
#undef cr_assert_any_throw
#define cr_assert_any_throw(...)        CR_EXPAND(cr_assert_any_throw_va_(CR_FAIL_ABORT_, __VA_ARGS__))
#undef cr_expect_any_throw
#define cr_expect_any_throw(...)        CR_EXPAND(cr_assert_any_throw_va_(CR_FAIL_CONTINUES_, __VA_ARGS__))
#undef cr_assert_none_throw
#define cr_assert_none_throw(...)       CR_EXPAND(cr_assert_none_throw_va_(CR_FAIL_ABORT_, __VA_ARGS__))
#undef cr_expect_none_throw
#define cr_expect_none_throw(...)       CR_EXPAND(cr_assert_none_throw_va_(CR_FAIL_CONTINUES_, __VA_ARGS__))

#endif /* !CRITERION_INTERNAL_ASSERT_H_ */
