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
#ifndef CRITERION_ASSERT_BASE_H_
# define CRITERION_ASSERT_BASE_H_

# include "preprocess.h"
# include "asprintf-compat.h"

# ifdef __cplusplus
#  include <cstring>
#  include <cstdlib>
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
        criterion_send_event(ASSERT, stat, bufsize);                        \
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

#endif /* !CRITERION_ASSERT_BASE_H_ */
