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
#ifndef CRITERION_ASSERT_H_
# define CRITERION_ASSERT_H_

# include "preprocess.h"
# include "asprintf-compat.h"

# ifdef __cplusplus
#  include <cstring>
#  include <cstdlib>
#  include <algorithm>
# else
#  include <string.h>
#  include <stdlib.h>
#  include <stdbool.h>
# endif
# include "designated-initializer-compat.h"
# include "types.h"
# include "stats.h"
# include "hooks.h"
# include "event.h"
# include "abort.h"

struct criterion_assert_args {
    const char *msg;
    int sentinel_;

#ifdef __cplusplus
    constexpr criterion_assert_args(const char *msg) : msg(msg), sentinel_(0) {}
    constexpr criterion_assert_args(const char *msg, int sentinel_) : msg(msg), sentinel_(sentinel_) {}
#endif
};

enum criterion_assert_messages {
    CRITERION_ASSERT_MSG_FAIL,
    CRITERION_ASSERT_MSG_EXPR_FALSE,
    CRITERION_ASSERT_MSG_EXPR_AS_STRINGS_FALSE,
    CRITERION_ASSERT_MSG_IS_NULL,
    CRITERION_ASSERT_MSG_IS_NOT_NULL,
    CRITERION_ASSERT_MSG_IS_EMPTY,
    CRITERION_ASSERT_MSG_IS_NOT_EMPTY,
};

CR_BEGIN_C_API

CR_API char *translate_assert_msg(int msg_index, ...);

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
    CR_EXPAND Arg

# define CR_TRANSLATE_DEF_MSG_(...)                                         \
    CR_EXPAND(translate_assert_msg(                                         \
            CR_VA_HEAD(__VA_ARGS__),                                        \
            "" CR_TRANSLATE_DEF_MSG__(CR_VA_HEAD(CR_VA_TAIL(__VA_ARGS__)))  \
    ))

# define CR_INIT_STATS_(BufSize, MsgVar, ...) CR_EXPAND(                       \
    do {                                                                       \
        char *def_msg = CR_EXPAND(CR_TRANSLATE_DEF_MSG_(__VA_ARGS__));         \
        char *formatted_msg = NULL;                                            \
        int msglen = cr_asprintf(&formatted_msg,                               \
                "" CR_VA_TAIL(CR_VA_TAIL(__VA_ARGS__)));                       \
        if (formatted_msg && *formatted_msg) {                                 \
            MsgVar = formatted_msg;                                            \
            CR_STDN free(def_msg);                                             \
        } else {                                                               \
            MsgVar = def_msg;                                                  \
            msglen = strlen(def_msg);                                          \
            CR_STDN free(formatted_msg);                                       \
        }                                                                      \
                                                                               \
        BufSize = sizeof(struct criterion_assert_stats)                        \
                  + sizeof (size_t) + msglen + 1;                              \
                                                                               \
        char *buf = (char*) CR_STDN malloc(BufSize);                           \
        stat = (struct criterion_assert_stats*) buf;                           \
        CR_STDN memset(buf, 0, sizeof (struct criterion_assert_stats));        \
        buf += sizeof (struct criterion_assert_stats);                         \
        *((size_t*) buf) = msglen + 1;                                         \
        buf += sizeof (size_t);                                                \
        CR_STDN strcpy(buf, MsgVar);                                           \
        CR_STDN free(MsgVar);                                                  \
    } while (0))

# define CR_FAIL_ABORT_ criterion_abort_test
# define CR_FAIL_CONTINUES_ criterion_continue_test

# ifdef __GNUC__
// We disable the format-zero-length warning because we use the validity of
// asprintf(out, "") for empty assertion messages
#  pragma GCC diagnostic ignored "-Wformat-zero-length"
# endif

# define cr_assert_impl(Fail, Condition, ...)                               \
    do {                                                                    \
        bool passed = !!(Condition);                                        \
                                                                            \
        char *msg = NULL;                                                   \
        size_t bufsize;                                                     \
                                                                            \
        struct criterion_assert_stats *stat;                                \
        CR_EXPAND(CR_INIT_STATS_(bufsize, msg, CR_VA_TAIL(__VA_ARGS__)));   \
        stat->passed = passed;                                              \
        stat->file = __FILE__;                                              \
        stat->line = __LINE__;                                              \
                                                                            \
        send_event(ASSERT, stat, bufsize);                                  \
        CR_STDN free(stat);                                                 \
                                                                            \
        if (!passed)                                                        \
            Fail();                                                         \
    } while (0)

// Base assertions

# define cr_fail(Fail, ...)                                     \
    CR_EXPAND(cr_assert_impl(                                   \
            Fail,                                               \
            0,                                                  \
            dummy,                                              \
            CRITERION_ASSERT_MSG_FAIL,                          \
            (),                                                 \
            __VA_ARGS__                                         \
    ))

# define cr_assert_fail(...) CR_EXPAND(cr_fail(CR_FAIL_ABORT_,     __VA_ARGS__))
# define cr_expect_fail(...) CR_EXPAND(cr_fail(CR_FAIL_CONTINUES_, __VA_ARGS__))

# define cr_assert(...)                                                     \
    CR_EXPAND(cr_assert_impl(                                               \
            CR_FAIL_ABORT_,                                                 \
            CR_VA_HEAD(__VA_ARGS__),                                        \
            dummy,                                                          \
            CRITERION_ASSERT_MSG_EXPR_FALSE,                                \
            (CR_STR(CR_VA_HEAD(__VA_ARGS__))),                              \
            CR_VA_TAIL(__VA_ARGS__)                                         \
    ))

# define cr_expect(...)                                                     \
    CR_EXPAND(cr_assert_impl(                                               \
            CR_FAIL_CONTINUES_,                                             \
            CR_VA_HEAD(__VA_ARGS__),                                        \
            dummy,                                                          \
            CRITERION_ASSERT_MSG_EXPR_FALSE,                                \
            (CR_STR(CR_VA_HEAD(__VA_ARGS__))),                              \
            CR_VA_TAIL(__VA_ARGS__)                                         \
    ))

# define cr_assert_not(...)                                                    \
    CR_EXPAND(cr_assert_impl(                                                  \
            CR_FAIL_ABORT_,                                                    \
            !(CR_VA_HEAD(__VA_ARGS__)),                                        \
            dummy,                                                             \
            CRITERION_ASSERT_MSG_EXPR_FALSE,                                   \
            (CR_STR(!(CR_VA_HEAD(__VA_ARGS__)))),                              \
            CR_VA_TAIL(__VA_ARGS__)                                            \
    ))

# define cr_expect_not(...)                                                    \
    CR_EXPAND(cr_assert_impl(                                                  \
            CR_FAIL_CONTINUES_,                                                \
            !(CR_VA_HEAD(__VA_ARGS__)),                                        \
            dummy,                                                             \
            CRITERION_ASSERT_MSG_EXPR_FALSE,                                   \
            (CR_STR(!(CR_VA_HEAD(__VA_ARGS__)))),                              \
            CR_VA_TAIL(__VA_ARGS__)                                            \
    ))

// Common binary assertions

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

# define cr_assert_eq(...) CR_EXPAND(cr_assert_op_va_(CR_FAIL_ABORT_,      ==, __VA_ARGS__))
# define cr_expect_eq(...) CR_EXPAND(cr_assert_op_va_(CR_FAIL_CONTINUES_,  ==, __VA_ARGS__))

# define cr_assert_neq(...) CR_EXPAND(cr_assert_op_va_(CR_FAIL_ABORT_,     !=, __VA_ARGS__))
# define cr_expect_neq(...) CR_EXPAND(cr_assert_op_va_(CR_FAIL_CONTINUES_, !=, __VA_ARGS__))

# define cr_assert_lt(...) CR_EXPAND(cr_assert_op_va_(CR_FAIL_ABORT_,      <, __VA_ARGS__))
# define cr_expect_lt(...) CR_EXPAND(cr_assert_op_va_(CR_FAIL_CONTINUES_,  <, __VA_ARGS__))

# define cr_assert_leq(...) CR_EXPAND(cr_assert_op_va_(CR_FAIL_ABORT_,     <=, __VA_ARGS__))
# define cr_expect_leq(...) CR_EXPAND(cr_assert_op_va_(CR_FAIL_CONTINUES_, <=, __VA_ARGS__))

# define cr_assert_gt(...) CR_EXPAND(cr_assert_op_va_(CR_FAIL_ABORT_,      >, __VA_ARGS__))
# define cr_expect_gt(...) CR_EXPAND(cr_assert_op_va_(CR_FAIL_CONTINUES_,  >, __VA_ARGS__))

# define cr_assert_geq(...) CR_EXPAND(cr_assert_op_va_(CR_FAIL_ABORT_,     >=, __VA_ARGS__))
# define cr_expect_geq(...) CR_EXPAND(cr_assert_op_va_(CR_FAIL_CONTINUES_, >=, __VA_ARGS__))

// Common unary assertions

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

# define cr_assert_null(...) CR_EXPAND(cr_assert_null_op_va_(CR_FAIL_ABORT_,     ==, CRITERION_ASSERT_MSG_IS_NOT_NULL, __VA_ARGS__))
# define cr_expect_null(...) CR_EXPAND(cr_assert_null_op_va_(CR_FAIL_CONTINUES_, ==, CRITERION_ASSERT_MSG_IS_NOT_NULL, __VA_ARGS__))

# define cr_assert_not_null(...) CR_EXPAND(cr_assert_null_op_va_(CR_FAIL_ABORT_,     !=, CRITERION_ASSERT_MSG_IS_NULL, __VA_ARGS__))
# define cr_expect_not_null(...) CR_EXPAND(cr_assert_null_op_va_(CR_FAIL_CONTINUES_, !=, CRITERION_ASSERT_MSG_IS_NULL, __VA_ARGS__))

// Floating-point assertions

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

# define cr_assert_float_eq(...) CR_EXPAND(cr_assert_float_op_va_(CR_FAIL_ABORT_,     cr_assert_float_eq_op_, __VA_ARGS__))
# define cr_expect_float_eq(...) CR_EXPAND(cr_assert_float_op_va_(CR_FAIL_CONTINUES_, cr_assert_float_eq_op_, __VA_ARGS__))

# define cr_assert_float_neq(...) CR_EXPAND(cr_assert_float_op_va_(CR_FAIL_ABORT_,     cr_assert_float_neq_op_, __VA_ARGS__))
# define cr_expect_float_neq(...) CR_EXPAND(cr_assert_float_op_va_(CR_FAIL_CONTINUES_, cr_assert_float_neq_op_, __VA_ARGS__))

// String assertions

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

# define cr_assert_str_empty(...) CR_EXPAND(cr_assert_str_op_empty_va_(CR_FAIL_ABORT_, ==, CRITERION_ASSERT_MSG_IS_NOT_EMPTY, __VA_ARGS__))
# define cr_expect_str_empty(...) CR_EXPAND(cr_assert_str_op_empty_va_(CR_FAIL_CONTINUES_, ==, CRITERION_ASSERT_MSG_IS_NOT_EMPTY, __VA_ARGS__))

# define cr_assert_str_not_empty(...) CR_EXPAND(cr_assert_str_op_empty_va_(CR_FAIL_ABORT_, !=, CRITERION_ASSERT_MSG_IS_EMPTY, __VA_ARGS__))
# define cr_expect_str_not_empty(...) CR_EXPAND(cr_assert_str_op_empty_va_(CR_FAIL_CONTINUES_, !=, CRITERION_ASSERT_MSG_IS_EMPTY, __VA_ARGS__))

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

# define cr_assert_str_eq(...) CR_EXPAND(cr_assert_str_op_va_(CR_FAIL_ABORT_,      ==, __VA_ARGS__))
# define cr_expect_str_eq(...) CR_EXPAND(cr_assert_str_op_va_(CR_FAIL_CONTINUES_,  ==, __VA_ARGS__))

# define cr_assert_str_neq(...) CR_EXPAND(cr_assert_str_op_va_(CR_FAIL_ABORT_,     !=, __VA_ARGS__))
# define cr_expect_str_neq(...) CR_EXPAND(cr_assert_str_op_va_(CR_FAIL_CONTINUES_, !=, __VA_ARGS__))

# define cr_assert_str_lt(...) CR_EXPAND(cr_assert_str_op_va_(CR_FAIL_ABORT_,      <, __VA_ARGS__))
# define cr_expect_str_lt(...) CR_EXPAND(cr_assert_str_op_va_(CR_FAIL_CONTINUES_,  <, __VA_ARGS__))

# define cr_assert_str_leq(...) CR_EXPAND(cr_assert_str_op_va_(CR_FAIL_ABORT_,     <=, __VA_ARGS__))
# define cr_expect_str_leq(...) CR_EXPAND(cr_assert_str_op_va_(CR_FAIL_CONTINUES_, <=, __VA_ARGS__))

# define cr_assert_str_gt(...) CR_EXPAND(cr_assert_str_op_va_(CR_FAIL_ABORT_,      >, __VA_ARGS__))
# define cr_expect_str_gt(...) CR_EXPAND(cr_assert_str_op_va_(CR_FAIL_CONTINUES_,  >, __VA_ARGS__))

# define cr_assert_str_geq(...) CR_EXPAND(cr_assert_str_op_va_(CR_FAIL_ABORT_,     >=, __VA_ARGS__))
# define cr_expect_str_geq(...) CR_EXPAND(cr_assert_str_op_va_(CR_FAIL_CONTINUES_, >=, __VA_ARGS__))

// Array assertions

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

# define cr_assert_arr_eq(...) CR_EXPAND(cr_assert_mem_op_va_(CR_FAIL_ABORT_,      ==, __VA_ARGS__))
# define cr_expect_arr_eq(...) CR_EXPAND(cr_assert_mem_op_va_(CR_FAIL_CONTINUES_,  ==, __VA_ARGS__))

# define cr_assert_arr_neq(...) CR_EXPAND(cr_assert_mem_op_va_(CR_FAIL_ABORT_,     !=, __VA_ARGS__))
# define cr_expect_arr_neq(...) CR_EXPAND(cr_assert_mem_op_va_(CR_FAIL_CONTINUES_, !=, __VA_ARGS__))

// Safe array comparison assertions

# if defined(__GNUC__) || defined(__cplusplus)

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

#  define cr_assert_arr_eq_cmp(...) CR_EXPAND(cr_assert_arr_op_cmp_va_(CR_FAIL_ABORT_,      ==, __VA_ARGS__))
#  define cr_expect_arr_eq_cmp(...) CR_EXPAND(cr_assert_arr_op_cmp_va_(CR_FAIL_CONTINUES_,  ==, __VA_ARGS__))

#  define cr_assert_arr_neq_cmp(...) CR_EXPAND(cr_assert_arr_op_cmp_va_(CR_FAIL_ABORT_,     !=, __VA_ARGS__))
#  define cr_expect_arr_neq_cmp(...) CR_EXPAND(cr_assert_arr_op_cmp_va_(CR_FAIL_CONTINUES_, !=, __VA_ARGS__))

#  define cr_assert_arr_lt_cmp(...) CR_EXPAND(cr_assert_arr_op_cmp_va_(CR_FAIL_ABORT_,      <, __VA_ARGS__))
#  define cr_expect_arr_lt_cmp(...) CR_EXPAND(cr_assert_arr_op_cmp_va_(CR_FAIL_CONTINUES_,  <, __VA_ARGS__))

#  define cr_assert_arr_leq_cmp(...) CR_EXPAND(cr_assert_arr_op_cmp_va_(CR_FAIL_ABORT_,     <=, __VA_ARGS__))
#  define cr_expect_arr_leq_cmp(...) CR_EXPAND(cr_assert_arr_op_cmp_va_(CR_FAIL_CONTINUES_, <=, __VA_ARGS__))

#  define cr_assert_arr_gt_cmp(...) CR_EXPAND(cr_assert_arr_op_cmp_va_(CR_FAIL_ABORT_,      >, __VA_ARGS__))
#  define cr_expect_arr_gt_cmp(...) CR_EXPAND(cr_assert_arr_op_cmp_va_(CR_FAIL_CONTINUES_,  >, __VA_ARGS__))

#  define cr_assert_arr_geq_cmp(...) CR_EXPAND(cr_assert_arr_op_cmp_va_(CR_FAIL_ABORT_,     >=, __VA_ARGS__))
#  define cr_expect_arr_geq_cmp(...) CR_EXPAND(cr_assert_arr_op_cmp_va_(CR_FAIL_CONTINUES_, >=, __VA_ARGS__))

# else

#  define CRITERION_GNUC_WARN__(Msg) \
    _Pragma(#Msg)

#  define CRITERION_GNUC_WARN_(Name) CRITERION_GNUC_WARN__(             \
        message                                                         \
        "The `" #Name "` macro is only available on GNU C compilers."   \
    )

#  define cr_assert_arr_eq_cmp(...) CRITERION_GNUC_WARN_(cr_assert_arr_eq_cmp) CR_NOOP
#  define cr_expect_arr_eq_cmp(...) CRITERION_GNUC_WARN_(cr_expect_arr_eq_cmp) CR_NOOP

#  define cr_assert_arr_neq_cmp(...) CRITERION_GNUC_WARN_(cr_assert_arr_neq_cmp) CR_NOOP
#  define cr_expect_arr_neq_cmp(...) CRITERION_GNUC_WARN_(cr_expect_arr_neq_cmp) CR_NOOP

#  define cr_assert_arr_lt_cmp(...) CRITERION_GNUC_WARN_(cr_assert_arr_lt_cmp) CR_NOOP
#  define cr_expect_arr_lt_cmp(...) CRITERION_GNUC_WARN_(cr_expect_arr_lt_cmp) CR_NOOP

#  define cr_assert_arr_leq_cmp(...) CRITERION_GNUC_WARN_(cr_assert_arr_leq_cmp) CR_NOOP
#  define cr_expect_arr_leq_cmp(...) CRITERION_GNUC_WARN_(cr_expect_arr_leq_cmp) CR_NOOP

#  define cr_assert_arr_gt_cmp(...) CRITERION_GNUC_WARN_(cr_assert_arr_gt_cmp) CR_NOOP
#  define cr_expect_arr_gt_cmp(...) CRITERION_GNUC_WARN_(cr_expect_arr_gt_cmp) CR_NOOP

#  define cr_assert_arr_geq_cmp(...) CRITERION_GNUC_WARN_(cr_assert_arr_geq_cmp) CR_NOOP
#  define cr_expect_arr_geq_cmp(...) CRITERION_GNUC_WARN_(cr_expect_arr_geq_cmp) CR_NOOP

# endif

# ifdef __cplusplus

#  define cr_assert_throw_(Fail, Statement, Exception, ...)                 \
    try {                                                                   \
        Statement;                                                          \
    } catch (Exception const &) {                                           \
    } catch (...) { CR_EXPAND(cr_fail(Fail, CR_VA_TAIL(__VA_ARGS__))); }

#  define cr_assert_throw_va_(...)                  \
    CR_EXPAND(cr_assert_throw_(                     \
            CR_VA_HEAD(__VA_ARGS__),                \
            CR_VA_HEAD(CR_VA_TAIL(__VA_ARGS__)),    \
            CR_VA_HEAD(CR_VA_TAIL(CR_VA_TAIL(__VA_ARGS__))),    \
            dummy,                                  \
            CR_VA_TAIL(CR_VA_TAIL(CR_VA_TAIL(__VA_ARGS__)))     \
    ))

#  define cr_assert_throw(...) CR_EXPAND(cr_assert_throw_va_(CR_FAIL_ABORT_,     __VA_ARGS__))
#  define cr_expect_throw(...) CR_EXPAND(cr_assert_throw_va_(CR_FAIL_CONTINUES_, __VA_ARGS__))

#  define cr_assert_no_throw_(Fail, Statement, Exception, ...)                      \
    try {                                                                           \
        Statement;                                                                  \
    } catch (Exception const &) { CR_EXPAND(cr_fail(Fail, CR_VA_TAIL(__VA_ARGS__))); }

#  define cr_assert_no_throw_va_(...)                           \
    CR_EXPAND(cr_assert_no_throw_(                              \
            CR_VA_HEAD(__VA_ARGS__),                            \
            CR_VA_HEAD(CR_VA_TAIL(__VA_ARGS__)),                \
            CR_VA_HEAD(CR_VA_TAIL(CR_VA_TAIL(__VA_ARGS__))),    \
            dummy,                                              \
            CR_VA_TAIL(CR_VA_TAIL(CR_VA_TAIL(__VA_ARGS__)))     \
    ))

#  define cr_assert_no_throw(...) CR_EXPAND(cr_assert_no_throw_va_(CR_FAIL_ABORT_,     __VA_ARGS__))
#  define cr_expect_no_throw(...) CR_EXPAND(cr_assert_no_throw_va_(CR_FAIL_CONTINUES_, __VA_ARGS__))

# endif

// The section below is here for backward compatibility purposes.
// It shall be removed in the next major version of Criterion
# ifndef CRITERION_NO_COMPAT

#  define CRITERION_ASSERT_DEPRECATED_(Name) CRITERION_ASSERT_DEPRECATED__( \
        message                                                             \
        ("The `" #Name "` macro is deprecated, "                            \
        "please use `cr_" #Name "` instead.")                               \
    )

#  define CRITERION_ASSERT_DEPRECATED_B(Name, Newname) \
    CRITERION_ASSERT_DEPRECATED__(                     \
        message                                        \
        ("The `" #Name "` macro is deprecated, "       \
        "please use `" #Newname "` instead.")          \
    )

# ifdef _MSC_VER
#  define CRITERION_ASSERT_DEPRECATED__(Msg) \
    __pragma(Msg)
# else
#  define CRITERION_ASSERT_DEPRECATED__(Msg) \
    _Pragma(#Msg)
# endif

#  ifndef assert
#   define assert(...) CRITERION_ASSERT_DEPRECATED_(assert) cr_assert(__VA_ARGS__)

// this is needed to make the POSIX assert.h redefine assert if
// subsequently included
#   ifndef _ASSERT_H
#    define _ASSERT_H 1
#   endif /* !_ASSERT_H */
#  endif /* !assert */

// scheduled for removal after 2.0
#  define cr_abort_test(Message) CRITERION_ASSERT_DEPRECATED_B(cr_abort_test, cr_assert_fail) cr_assert_fail(Message)
#  define cr_assert_strings_eq(...) CRITERION_ASSERT_DEPRECATED_B(cr_assert_strings_eq, cr_assert_str_eq) cr_assert_str_eq(__VA_ARGS__)
#  define cr_assert_strings_neq(...) CRITERION_ASSERT_DEPRECATED_B(cr_assert_strings_neq, cr_assert_str_neq) cr_assert_str_neq(__VA_ARGS__)
#  define cr_assert_strings_lt(...) CRITERION_ASSERT_DEPRECATED_B(cr_assert_strings_lt, cr_assert_str_lt) cr_assert_str_lt(__VA_ARGS__)
#  define cr_assert_strings_leq(...) CRITERION_ASSERT_DEPRECATED_B(cr_assert_strings_leq, cr_assert_str_leq) cr_assert_str_leq(__VA_ARGS__)
#  define cr_assert_strings_gt(...) CRITERION_ASSERT_DEPRECATED_B(cr_assert_strings_gt, cr_assert_str_gt) cr_assert_str_gt(__VA_ARGS__)
#  define cr_assert_strings_geq(...) CRITERION_ASSERT_DEPRECATED_B(cr_assert_strings_geq, cr_assert_str_geq) cr_assert_str_geq(__VA_ARGS__)

#  define cr_assert_arrays_eq(...) CRITERION_ASSERT_DEPRECATED_B(cr_assert_arrays_eq, cr_assert_arr_eq) cr_assert_arr_eq(__VA_ARGS__)
#  define cr_assert_arrays_neq(...) CRITERION_ASSERT_DEPRECATED_B(cr_assert_arrays_neq, cr_assert_arr_neq) cr_assert_arr_neq(__VA_ARGS__)

#  define cr_assert_arrays_eq_cmp(...) CRITERION_ASSERT_DEPRECATED_B(cr_assert_arrays_eq_cmp, cr_assert_arr_eq_cmp) cr_assert_arr_eq_cmp(__VA_ARGS__)
#  define cr_assert_arrays_neq_cmp(...) CRITERION_ASSERT_DEPRECATED_B(cr_assert_arrays_neq_cmp, cr_assert_arr_neq_cmp) cr_assert_arr_neq_cmp(__VA_ARGS__)

// scheduled for removal at 2.0
#  define abort_test(Message) CRITERION_ASSERT_DEPRECATED_(abort_test) cr_abort_test(Message)
#  define expect(...) CRITERION_ASSERT_DEPRECATED_(expect) cr_expect(__VA_ARGS__)
#  define assert_not(...) CRITERION_ASSERT_DEPRECATED_(assert_not) cr_assert_not(__VA_ARGS__)
#  define expect_not(...) CRITERION_ASSERT_DEPRECATED_(expect_not) cr_expect_not(__VA_ARGS__)
#  define assert_eq(...) CRITERION_ASSERT_DEPRECATED_(assert_eq) cr_assert_eq(__VA_ARGS__)
#  define expect_eq(...) CRITERION_ASSERT_DEPRECATED_(expect_eq) cr_expect_eq(__VA_ARGS__)
#  define assert_neq(...) CRITERION_ASSERT_DEPRECATED_(assert_neq) cr_assert_neq(__VA_ARGS__)
#  define expect_neq(...) CRITERION_ASSERT_DEPRECATED_(expect_neq) cr_expect_neq(__VA_ARGS__)
#  define assert_lt(...) CRITERION_ASSERT_DEPRECATED_(assert_lt) cr_assert_lt(__VA_ARGS__)
#  define expect_lt(...) CRITERION_ASSERT_DEPRECATED_(expect_lt) cr_expect_lt(__VA_ARGS__)
#  define assert_gt(...) CRITERION_ASSERT_DEPRECATED_(assert_gt) cr_assert_gt(__VA_ARGS__)
#  define expect_gt(...) CRITERION_ASSERT_DEPRECATED_(expect_gt) cr_expect_gt(__VA_ARGS__)
#  define assert_leq(...) CRITERION_ASSERT_DEPRECATED_(assert_leq) cr_assert_leq(__VA_ARGS__)
#  define expect_leq(...) CRITERION_ASSERT_DEPRECATED_(expect_leq) cr_expect_leq(__VA_ARGS__)
#  define assert_geq(...) CRITERION_ASSERT_DEPRECATED_(assert_geq) cr_assert_geq(__VA_ARGS__)
#  define expect_geq(...) CRITERION_ASSERT_DEPRECATED_(expect_geq) cr_expect_geq(__VA_ARGS__)
#  define assert_null(...) CRITERION_ASSERT_DEPRECATED_(assert_null) cr_assert_null(__VA_ARGS__)
#  define expect_null(...) CRITERION_ASSERT_DEPRECATED_(expect_null) cr_expect_null(__VA_ARGS__)
#  define assert_not_null(...) CRITERION_ASSERT_DEPRECATED_(assert_not_null) cr_assert_not_null(__VA_ARGS__)
#  define expect_not_null(...) CRITERION_ASSERT_DEPRECATED_(expect_not_null) cr_expect_not_null(__VA_ARGS__)
#  define assert_float_eq(...) CRITERION_ASSERT_DEPRECATED_(assert_float_eq) cr_assert_float_eq(__VA_ARGS__)
#  define expect_float_eq(...) CRITERION_ASSERT_DEPRECATED_(expect_float_eq) cr_expect_float_eq(__VA_ARGS__)
#  define assert_float_neq(...) CRITERION_ASSERT_DEPRECATED_(assert_float_neq) cr_assert_float_neq(__VA_ARGS__)
#  define expect_float_neq(...) CRITERION_ASSERT_DEPRECATED_(expect_float_neq) cr_expect_float_neq(__VA_ARGS__)
#  define assert_strings_eq(...) CRITERION_ASSERT_DEPRECATED_(assert_strings_eq) cr_assert_strings_eq(__VA_ARGS__)
#  define expect_strings_eq(...) CRITERION_ASSERT_DEPRECATED_(expect_strings_eq) cr_expect_strings_eq(__VA_ARGS__)
#  define assert_strings_neq(...) CRITERION_ASSERT_DEPRECATED_(assert_strings_neq) cr_assert_strings_neq(__VA_ARGS__)
#  define expect_strings_neq(...) CRITERION_ASSERT_DEPRECATED_(expect_strings_neq) cr_expect_strings_neq(__VA_ARGS__)
#  define assert_strings_gt(...) CRITERION_ASSERT_DEPRECATED_(assert_strings_gt) cr_assert_strings_gt(__VA_ARGS__)
#  define expect_strings_gt(...) CRITERION_ASSERT_DEPRECATED_(expect_strings_gt) cr_expect_strings_gt(__VA_ARGS__)
#  define assert_strings_lt(...) CRITERION_ASSERT_DEPRECATED_(assert_strings_lt) cr_assert_strings_lt(__VA_ARGS__)
#  define expect_strings_lt(...) CRITERION_ASSERT_DEPRECATED_(expect_strings_lt) cr_expect_strings_lt(__VA_ARGS__)
#  define assert_strings_leq(...) CRITERION_ASSERT_DEPRECATED_(assert_strings_leq) cr_assert_strings_leq(__VA_ARGS__)
#  define expect_strings_leq(...) CRITERION_ASSERT_DEPRECATED_(expect_strings_leq) cr_expect_strings_leq(__VA_ARGS__)
#  define assert_strings_geq(...) CRITERION_ASSERT_DEPRECATED_(assert_strings_geq) cr_assert_strings_geq(__VA_ARGS__)
#  define expect_strings_geq(...) CRITERION_ASSERT_DEPRECATED_(expect_strings_geq) cr_expect_strings_geq(__VA_ARGS__)
#  define assert_arrays_eq(...) CRITERION_ASSERT_DEPRECATED_(assert_arrays_eq) cr_assert_arrays_eq(__VA_ARGS__)
#  define expect_arrays_eq(...) CRITERION_ASSERT_DEPRECATED_(expect_arrays_eq) cr_expect_arrays_eq(__VA_ARGS__)
#  define assert_arrays_neq(...) CRITERION_ASSERT_DEPRECATED_(assert_arrays_neq) cr_assert_arrays_neq(__VA_ARGS__)
#  define expect_arrays_neq(...) CRITERION_ASSERT_DEPRECATED_(expect_arrays_neq) cr_expect_arrays_neq(__VA_ARGS__)
#  define assert_arrays_eq_cmp(...) CRITERION_ASSERT_DEPRECATED_(assert_arrays_eq_cmp) cr_assert_arrays_eq_cmp(__VA_ARGS__)
#  define expect_arrays_eq_cmp(...) CRITERION_ASSERT_DEPRECATED_(expect_arrays_eq_cmp) cr_expect_arrays_eq_cmp(__VA_ARGS__)
#  define assert_arrays_neq_cmp(...) CRITERION_ASSERT_DEPRECATED_(assert_arrays_neq_cmp) cr_assert_arrays_neq_cmp(__VA_ARGS__)
#  define expect_arrays_neq_cmp(...) CRITERION_ASSERT_DEPRECATED_(expect_arrays_neq_cmp) cr_expect_arrays_neq_cmp(__VA_ARGS__)

# endif

#endif /* !CRITERION_ASSERT_H_ */
