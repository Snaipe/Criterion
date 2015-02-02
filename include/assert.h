#ifndef CRITERION_ASSERT_H_
# define CRITERION_ASSERT_H_

# include <stdlib.h>
# include <stdbool.h>
# include "criterion.h"

enum criterion_assert_kind {
    NORMAL,
    FATAL
};

struct criterion_assert_stat {
    enum criterion_assert_kind kind;
    const char *condition;
    const char *message;
    bool passed;
    unsigned line;

    struct criterion_assert_stat *next;
};

struct criterion_test_stats {
    struct criterion_test *test;
    struct criterion_assert_stat *asserts;
    unsigned failed;
    unsigned passed;
};

extern struct criterion_test_stats g_current_test_stats;

# define assertImpl(Kind, Condition, ...)                       \
    do {                                                        \
        int passed = !!(Condition);                             \
        struct criterion_assert_stat *stat =                    \
                malloc(sizeof (struct criterion_assert_stat));  \
        *stat = (struct criterion_assert_stat) {                \
            .kind = Kind,                                       \
            .condition = #Condition,                            \
            .message = "" __VA_ARGS__,                          \
            .passed = passed,                                   \
            .line = __LINE__,                                   \
            .next = g_current_test_stats.asserts,               \
        };                                                      \
        g_current_test_stats.asserts = stat;                    \
        if (passed)                                             \
            ++g_current_test_stats.passed;                      \
        else                                                    \
            ++g_current_test_stats.failed;                      \
        if (!passed && Kind == FATAL)                           \
            return;                                             \
    } while (0)

# define expect(Condition, ...) assertImpl(NORMAL, Condition, __VA_ARGS__)
# define assert(Condition, ...) assertImpl(FATAL, Condition, __VA_ARGS__)

# define assertArrayEquals(A, B, Size, ...) \
    assertImpl(NORMAL, !memcmp((A), (B), Size), __VA_ARGS__)
# define expectArrayEquals(A, B, Size, ...) \
    assertImpl(FATAL, !memcmp((A), (B), Size), __VA_ARGS__)

#endif /* !CRITERION_ASSERT_H_ */
