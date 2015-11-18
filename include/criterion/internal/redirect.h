#ifndef CRITERION_INTERNAL_REDIRECT_H_
# define CRITERION_INTERNAL_REDIRECT_H_

# include "common.h"
# include "assert.h"

# define cr_assert_redir_op_(Fail, Fun, Op, File, Str, ...)     \
    CR_EXPAND(cr_assert_impl(                                   \
            Fail,                                               \
            !(Fun((File), (Str)) Op 0),                         \
            dummy,                                              \
            CRITERION_ASSERT_MSG_FILE_STR_MATCH,                \
            (CR_STR(File), Str),                                \
            __VA_ARGS__                                         \
    ))

# define cr_assert_redir_op_va_(Fail, Fun, Op, ...)             \
    CR_EXPAND(cr_assert_redir_op_(                              \
            Fail,                                               \
            Fun,                                                \
            Op,                                                 \
            CR_VA_HEAD(__VA_ARGS__),                            \
            CR_VA_HEAD(CR_VA_TAIL(__VA_ARGS__)),                \
            CR_VA_TAIL(CR_VA_TAIL(__VA_ARGS__))                 \
    ))

# define cr_assert_redir_f_op_(Fail, Fun, Op, File, Ref, ...)   \
    CR_EXPAND(cr_assert_impl(                                   \
            Fail,                                               \
            !(Fun((File), (Ref)) Op 0),                         \
            dummy,                                              \
            CRITERION_ASSERT_MSG_FILE_MATCH,                    \
            (CR_STR(File), CR_STR(Ref)),                        \
            __VA_ARGS__                                         \
    ))

# define cr_assert_redir_f_op_va_(Fail, Fun, Op, ...)           \
    CR_EXPAND(cr_assert_redir_op_(                              \
            Fail,                                               \
            Fun,                                                \
            Op,                                                 \
            CR_VA_HEAD(__VA_ARGS__),                            \
            CR_VA_HEAD(CR_VA_TAIL(__VA_ARGS__)),                \
            CR_VA_TAIL(CR_VA_TAIL(__VA_ARGS__))                 \
    ))


#endif /* !CRITERION_INTERNAL_REDIRECT_H_ */
