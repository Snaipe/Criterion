/*
 * The MIT License (MIT)
 *
 * Copyright © 2015 Franklin "Snaipe" Mathieu <http://snai.pe/>
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
# define CRITERION_INTERNAL_ASSERT_H_

# include "common.h"
# include "preprocess.h"
# include "asprintf-compat.h"
# include "designated-initializer-compat.h"

# ifdef __cplusplus
#  include <cstring>
#  include <cstdlib>
# else
#  include <string.h>
#  include <stdlib.h>
#  include <stdbool.h>
# endif
# include "../types.h"
# include "../stats.h"
# include "../hooks.h"
# include "../event.h"
# include "../abort.h"

struct criterion_assert_args {
    const char *msg;
    int sentinel_;

#ifdef __cplusplus
    constexpr criterion_assert_args(const char *msg) : msg(msg), sentinel_(0) {}
    constexpr criterion_assert_args(const char *msg, int sentinel_) : msg(msg), sentinel_(sentinel_) {}
#endif
};

// Do NOT reorder unless you want to break the ABI
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

# define CR_GET_CONDITION(Condition, ...) Condition
# define CR_GET_CONDITION_STR(Condition, ...) #Condition
# define CR_VA_SKIP(_, ...) __VA_ARGS__

# ifdef __cplusplus
#  define CR_STDN std::
# else
#  define CR_STDN
# endif

# define CR_TRANSLATE_DEF_MSG__(Arg) \
    CR_IDENTITY Arg

# define CR_TRANSLATE_DEF_MSG_(...)                                         \
    CR_EXPAND(cr_translate_assert_msg(                                      \
            CR_VA_HEAD(__VA_ARGS__),                                        \
            "" CR_TRANSLATE_DEF_MSG__(CR_VA_HEAD(CR_VA_TAIL(__VA_ARGS__)))  \
    ))

# define CR_INIT_STATS_(BufSize, MsgVar, ...) CR_EXPAND(                       \
    do {                                                                       \
        char *cr_def_msg__ = CR_EXPAND(CR_TRANSLATE_DEF_MSG_(__VA_ARGS__));    \
        char *cr_fmt_msg__ = NULL;                                             \
        int cr_shifted__ = 0;                                                  \
        int cr_msglen__ = cr_asprintf(&cr_fmt_msg__,                           \
                "x" CR_VA_TAIL(CR_VA_TAIL(__VA_ARGS__)));                      \
        if (cr_fmt_msg__ && cr_fmt_msg__[1]) {                                 \
            MsgVar = cr_fmt_msg__ + 1;                                         \
            cr_shifted__ = 1;                                                  \
            --cr_msglen__;                                                     \
            CR_STDN free(cr_def_msg__);                                        \
        } else {                                                               \
            MsgVar = cr_def_msg__;                                             \
            cr_msglen__ = strlen(cr_def_msg__);                                \
            CR_STDN free(cr_fmt_msg__);                                        \
        }                                                                      \
                                                                               \
        BufSize = sizeof(struct criterion_assert_stats)                        \
                  + sizeof (size_t) + cr_msglen__ + 1;                         \
                                                                               \
        char *cr_buf__ = (char*) CR_STDN malloc(BufSize);                      \
        cr_stat__ = (struct criterion_assert_stats*) cr_buf__;                 \
        CR_STDN memset(cr_buf__, 0, sizeof (struct criterion_assert_stats));   \
        cr_buf__ += sizeof (struct criterion_assert_stats);                    \
        *((size_t*) cr_buf__) = cr_msglen__ + 1;                               \
        cr_buf__ += sizeof (size_t);                                           \
        CR_STDN strcpy(cr_buf__, MsgVar);                                      \
        CR_STDN free(MsgVar - cr_shifted__);                                   \
    } while (0))

# define CR_FAIL_ABORT_ criterion_abort_test
# define CR_FAIL_CONTINUES_ criterion_continue_test

# define cr_assert_impl(Fail, Condition, ...)                               \
    do {                                                                    \
        bool cr_passed__ = !!(Condition);                                   \
                                                                            \
        char *cr_msg__ = NULL;                                              \
        size_t cr_bufsize__;                                                \
                                                                            \
        struct criterion_assert_stats *cr_stat__;                           \
        CR_EXPAND(CR_INIT_STATS_(cr_bufsize__, cr_msg__,                    \
                    CR_VA_TAIL(__VA_ARGS__)));                              \
        cr_stat__->passed = cr_passed__;                                    \
        cr_stat__->file = __FILE__;                                         \
        cr_stat__->line = __LINE__;                                         \
                                                                            \
        criterion_send_event(ASSERT, cr_stat__, cr_bufsize__);              \
        CR_STDN free(cr_stat__);                                            \
                                                                            \
        if (!cr_passed__)                                                   \
            Fail();                                                         \
    } while (0)

# define cr_fail(Fail, ...)                                     \
    CR_EXPAND(cr_assert_impl(                                   \
            Fail,                                               \
            0,                                                  \
            dummy,                                              \
            CRITERION_ASSERT_MSG_FAIL,                          \
            (),                                                 \
            __VA_ARGS__                                         \
    ))

# define cr_assert_(...)                                                    \
    CR_EXPAND(cr_assert_impl(                                               \
            CR_FAIL_ABORT_,                                                 \
            CR_VA_HEAD(__VA_ARGS__),                                        \
            dummy,                                                          \
            CRITERION_ASSERT_MSG_EXPR_FALSE,                                \
            (CR_STR(CR_VA_HEAD(__VA_ARGS__))),                              \
            CR_VA_TAIL(__VA_ARGS__)                                         \
    ))

# define cr_expect_(...)                                                    \
    CR_EXPAND(cr_assert_impl(                                               \
            CR_FAIL_CONTINUES_,                                             \
            CR_VA_HEAD(__VA_ARGS__),                                        \
            dummy,                                                          \
            CRITERION_ASSERT_MSG_EXPR_FALSE,                                \
            (CR_STR(CR_VA_HEAD(__VA_ARGS__))),                              \
            CR_VA_TAIL(__VA_ARGS__)                                         \
    ))

# define cr_assert_not_(...)                                                \
    CR_EXPAND(cr_assert_impl(                                               \
            CR_FAIL_ABORT_,                                                 \
            !(CR_VA_HEAD(__VA_ARGS__)),                                     \
            dummy,                                                          \
            CRITERION_ASSERT_MSG_EXPR_FALSE,                                \
            (CR_STR(!(CR_VA_HEAD(__VA_ARGS__)))),                           \
            CR_VA_TAIL(__VA_ARGS__)                                         \
    ))

# define cr_expect_not_(...)                                                 \
    CR_EXPAND(cr_assert_impl(                                               \
            CR_FAIL_CONTINUES_,                                             \
            !(CR_VA_HEAD(__VA_ARGS__)),                                     \
            dummy,                                                          \
            CRITERION_ASSERT_MSG_EXPR_FALSE,                                \
            (CR_STR(!(CR_VA_HEAD(__VA_ARGS__)))),                           \
            CR_VA_TAIL(__VA_ARGS__)                                         \
    ))


// Binary

# define cr_assert_op_(Fail, Op, Actual, Expected, ...)                     \
    CR_EXPAND(cr_assert_impl(                                               \
            Fail,                                                           \
            (Actual) Op (Expected),                                         \
            dummy,                                                          \
            CRITERION_ASSERT_MSG_EXPR_FALSE,                                \
            (CR_STR((Actual) Op (Expected))),                               \
            __VA_ARGS__                                                     \
    ))

# define cr_assert_op_va_(Fail, Op, ...)                                    \
    CR_EXPAND(cr_assert_op_(                                                \
            Fail,                                                           \
            Op,                                                             \
            CR_VA_HEAD(__VA_ARGS__),                                        \
            CR_VA_HEAD(CR_VA_TAIL(__VA_ARGS__)),                            \
            CR_VA_TAIL(CR_VA_TAIL(__VA_ARGS__))                             \
    ))

// Unary

# define cr_assert_null_op_(Fail, Op, Msg, Value, ...)  \
    CR_EXPAND(cr_assert_impl(                           \
            Fail,                                       \
            (Value) Op NULL,                            \
            dummy,                                      \
            Msg,                                        \
            (CR_STR(Value)),                            \
            __VA_ARGS__                                 \
    ))

# define cr_assert_null_op_va_(Fail, Op, Msg, ...)      \
    CR_EXPAND(cr_assert_null_op_(                       \
            Fail,                                       \
            Op,                                         \
            Msg,                                        \
            CR_VA_HEAD(__VA_ARGS__),                    \
            CR_VA_TAIL(__VA_ARGS__)                     \
    ))

// Floating point

# define cr_assert_float_eq_op_(Actual, Expected, Epsilon) \
    (Expected) - (Actual) <= (Epsilon) && (Actual) - (Expected) <= (Epsilon)

# define cr_assert_float_neq_op_(Actual, Expected, Epsilon) \
    (Expected) - (Actual) > (Epsilon) || (Actual) - (Expected) > (Epsilon)

# define cr_assert_float_op_(Fail, Op, Actual, Expected, Epsilon, ...)         \
    CR_EXPAND(cr_assert_impl(                                                  \
            Fail,                                                              \
            Op(Actual, Expected, Epsilon),                                     \
            dummy,                                                             \
            CRITERION_ASSERT_MSG_EXPR_FALSE,                                   \
            (CR_STR(Op(Actual, Expected, Epsilon))),                           \
            __VA_ARGS__                                                        \
    ))

# define cr_assert_float_op_va_(Fail, Op, ...)                  \
    CR_EXPAND(cr_assert_float_op_(                              \
            Fail,                                               \
            Op,                                                 \
            CR_VA_HEAD(__VA_ARGS__),                            \
            CR_VA_HEAD(CR_VA_TAIL(__VA_ARGS__)),                \
            CR_VA_HEAD(CR_VA_TAIL(CR_VA_TAIL(__VA_ARGS__))),    \
            CR_VA_TAIL(CR_VA_TAIL(CR_VA_TAIL(__VA_ARGS__)))     \
    ))

// String

# define cr_assert_str_op_empty_(Fail, Op, Msg, Value, ...) \
    CR_EXPAND(cr_assert_impl(                               \
            Fail,                                           \
            (Value)[0] Op '\0',                             \
            dummy,                                          \
            Msg,                                            \
            (CR_STR(Value)),                                \
            __VA_ARGS__                                     \
    ))

# define cr_assert_str_op_empty_va_(Fail, Op, Msg, ...) \
    CR_EXPAND(cr_assert_str_op_empty_(                  \
            Fail,                                       \
            Op,                                         \
            Msg,                                        \
            CR_VA_HEAD(__VA_ARGS__),                    \
            CR_VA_TAIL(__VA_ARGS__)                     \
    ))

# define cr_assert_str_op_(Fail, Op, Actual, Expected, ...) \
    CR_EXPAND(cr_assert_impl(                               \
            Fail,                                           \
            CR_STDN strcmp((Actual), (Expected)) Op 0,      \
            dummy,                                          \
            CRITERION_ASSERT_MSG_EXPR_AS_STRINGS_FALSE,     \
            (CR_STR((Actual) Op (Expected))),               \
            __VA_ARGS__                                     \
    ))

# define cr_assert_str_op_va_(Fail, Op, ...)        \
    CR_EXPAND(cr_assert_str_op_(                    \
            Fail,                                   \
            Op,                                     \
            CR_VA_HEAD(__VA_ARGS__),                \
            CR_VA_HEAD(CR_VA_TAIL(__VA_ARGS__)),    \
            CR_VA_TAIL(CR_VA_TAIL(__VA_ARGS__))     \
    ))

// Array

# define cr_assert_mem_op_(Fail, Op, Actual, Expected, Size, ...)       \
    CR_EXPAND(cr_assert_impl(                                           \
            Fail,                                                       \
            CR_STDN memcmp((Actual), (Expected), (Size)) Op 0,          \
            dummy,                                                      \
            CRITERION_ASSERT_MSG_EXPR_FALSE,                            \
            (CR_STR((Actual)[0 .. Size] Op (Expected)[0 .. Size])),     \
            __VA_ARGS__                                                 \
    ))

# define cr_assert_mem_op_va_(Fail, Op, ...)                    \
    CR_EXPAND(cr_assert_mem_op_(                                \
            Fail,                                               \
            Op,                                                 \
            CR_VA_HEAD(__VA_ARGS__),                            \
            CR_VA_HEAD(CR_VA_TAIL(__VA_ARGS__)),                \
            CR_VA_HEAD(CR_VA_TAIL(CR_VA_TAIL(__VA_ARGS__))),    \
            CR_VA_TAIL(CR_VA_TAIL(CR_VA_TAIL(__VA_ARGS__)))     \
    ))

// Array comparisons

#  ifdef __cplusplus
#   define CR_ARR_COMPARE_(A, B, Size, Cmp, Result)                     \
    int Result = std::lexicographical_compare((A), (A) + Size, (B), (B) + Size, Cmp)
#  else
#   define CR_ARR_COMPARE_(A, B, Size, Cmp, Result)                     \
        __typeof__(&(A)[0]) first = (A);                                \
        __typeof__(&(B)[0]) second = (B);                               \
        int Result = 0;                                                 \
        size_t i, size;                                                 \
        for (i = 0, size = (Size); !Result && i < size; ++i)            \
            Result = Cmp(first + i, second + i)
#  endif

#  define cr_assert_arr_op_cmp_(Fail, Op, Actual, Expected, Size, Cmp, ...) \
    do {                                                                    \
        CR_ARR_COMPARE_(Actual, Expected, Size, Cmp, order);                \
        CR_EXPAND(cr_assert_impl(                                           \
                Fail,                                                       \
                order Op 0,                                                 \
                dummy,                                                      \
                CRITERION_ASSERT_MSG_EXPR_FALSE,                            \
                (CR_STR((Actual)[0 .. Size] Op (Expected)[0 .. Size])),     \
                __VA_ARGS__                                                 \
        ));                                                                 \
    } while (0)

#  define cr_assert_arr_op_cmp_va_(Fail, Op, ...)                           \
    CR_EXPAND(cr_assert_arr_op_cmp_(                                        \
            Fail,                                                           \
            Op,                                                             \
            CR_VA_HEAD(__VA_ARGS__),                                        \
            CR_VA_HEAD(CR_VA_TAIL(__VA_ARGS__)),                            \
            CR_VA_HEAD(CR_VA_TAIL(CR_VA_TAIL(__VA_ARGS__))),                \
            CR_VA_HEAD(CR_VA_TAIL(CR_VA_TAIL(CR_VA_TAIL(__VA_ARGS__)))),    \
            CR_VA_TAIL(CR_VA_TAIL(CR_VA_TAIL(CR_VA_TAIL(__VA_ARGS__))))     \
    ))

// Exceptions

#  define cr_assert_throw_abort_(Fail, Msg, MsgArgs, ...)               \
    CR_EXPAND(cr_assert_impl(                                           \
            Fail,                                                       \
            0,                                                          \
            dummy,                                                      \
            Msg,                                                        \
            MsgArgs,                                                    \
            CR_VA_TAIL(__VA_ARGS__)                                     \
    ))

#  define cr_assert_throw_(Fail, Statement, Exception, ...)             \
    try {                                                               \
        Statement;                                                      \
    } catch (Exception const &) {                                       \
    } catch (...) {                                                     \
        CR_EXPAND(cr_assert_throw_abort_(                               \
                Fail,                                                   \
                CRITERION_ASSERT_MSG_NO_THROW,                          \
                (CR_STR(Statement), CR_STR(Exception)),                 \
                __VA_ARGS__));                                          \
    }

#  define cr_assert_throw_va_(...)                                      \
    CR_EXPAND(cr_assert_throw_(                                         \
            CR_VA_HEAD(__VA_ARGS__),                                    \
            CR_VA_HEAD(CR_VA_TAIL(__VA_ARGS__)),                        \
            CR_VA_HEAD(CR_VA_TAIL(CR_VA_TAIL(__VA_ARGS__))),            \
            dummy,                                                      \
            CR_VA_TAIL(CR_VA_TAIL(CR_VA_TAIL(__VA_ARGS__)))             \
    ))

#  define cr_assert_no_throw_(Fail, Statement, Exception, ...)          \
    try {                                                               \
        Statement;                                                      \
    } catch (Exception const &) {                                       \
        CR_EXPAND(cr_assert_throw_abort_(                               \
                Fail,                                                   \
                CRITERION_ASSERT_MSG_THROW,                             \
                (CR_STR(Statement), CR_STR(Exception)),                 \
                __VA_ARGS__));                                          \
    }

#  define cr_assert_no_throw_va_(...)                                   \
    CR_EXPAND(cr_assert_no_throw_(                                      \
            CR_VA_HEAD(__VA_ARGS__),                                    \
            CR_VA_HEAD(CR_VA_TAIL(__VA_ARGS__)),                        \
            CR_VA_HEAD(CR_VA_TAIL(CR_VA_TAIL(__VA_ARGS__))),            \
            dummy,                                                      \
            CR_VA_TAIL(CR_VA_TAIL(CR_VA_TAIL(__VA_ARGS__)))             \
    ))

#  define cr_assert_any_throw_(Fail, Statement, ...)                    \
    try {                                                               \
        Statement;                                                      \
        CR_EXPAND(cr_assert_throw_abort_(                               \
                Fail,                                                   \
                CRITERION_ASSERT_MSG_ANY_THROW,                         \
                (CR_STR(Statement)),                                    \
                __VA_ARGS__));                                          \
    } catch (...) {}

#  define cr_assert_any_throw_va_(...)                                  \
    CR_EXPAND(cr_assert_any_throw_(                                     \
            CR_VA_HEAD(__VA_ARGS__),                                    \
            CR_VA_HEAD(CR_VA_TAIL(__VA_ARGS__)),                        \
            dummy,                                                      \
            CR_VA_TAIL(CR_VA_TAIL(__VA_ARGS__))                         \
    ))

#  define cr_assert_none_throw_(Fail, Statement, ...)                   \
    try {                                                               \
        Statement;                                                      \
    } catch (...) {                                                     \
        CR_EXPAND(cr_assert_throw_abort_(                               \
                Fail,                                                   \
                CRITERION_ASSERT_MSG_NONE_THROW,                        \
                (CR_STR(Statement)),                                    \
                __VA_ARGS__));                                          \
    }

#  define cr_assert_none_throw_va_(...)                                 \
    CR_EXPAND(cr_assert_none_throw_(                                    \
            CR_VA_HEAD(__VA_ARGS__),                                    \
            CR_VA_HEAD(CR_VA_TAIL(__VA_ARGS__)),                        \
            dummy,                                                      \
            CR_VA_TAIL(CR_VA_TAIL(__VA_ARGS__))                         \
    ))

// Messages

#  define CRITERION_GNUC_WARN__(Msg) \
    _Pragma(#Msg)

#  define CRITERION_GNUC_WARN_(Name) CRITERION_GNUC_WARN__(             \
        message                                                         \
        "The `" #Name "` macro is only available on GNU C compilers."   \
    )

#endif /* !CRITERION_INTERNAL_ASSERT_H_ */
