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
#ifndef CRITERION_INTERNAL_NEW_ASSERTS_H_
#define CRITERION_INTERNAL_NEW_ASSERTS_H_

#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
# include <ostream>
#endif

#include "asprintf-compat.h"
#include "common.h"
#include "designated-initializer-compat.h"
#include "preprocess.h"
#include "capabilities.h"

#include "assert/op.h"
#include "assert/tag.h"
#include "assert/tostr.h"
#include "assert/types.h"
#include "assert/ieee.h"

#ifdef __cplusplus
# include "assert/exceptions.h"
#endif

#define CRI_SPECIFIER_INDIRECT()      CRI_ASSERT_SPECIFIER

#define CRI_ASSERT_SPECIFIER(Spec)                      \
    CRI_IF_DEFINED(CRI_ASSERT_TEST_SPECIFIER_ ## Spec,  \
        CR_CONCAT2, (CRI_ASSERT_SPECIFIER_, Spec),      \
        CRI_ASSERT_SPECIFIER_VALUE, (Spec)              \
    )

#define CRI_ASSERT_FAIL(File, Line, Fail, ...)                              \
    CR_EVAL(do {                                                            \
        struct cri_assert_node cri_root;                                    \
        cri_assert_node_init(&cri_root);                                    \
        cri_root.message = cri_assert_message("x" __VA_ARGS__);             \
        cri_assert_node_send(File, Line, &cri_root);                        \
        cri_assert_node_term(&cri_root);                                    \
        Fail();                                                             \
    } while (0))

#define CRI_ASSERT_CALL(File, Line, Fail, Condition, ...)                       \
    CR_EVAL(do {                                                                \
        struct cri_assert_node cri_tmpn, cri_root, *cri_node = &cri_root;       \
        struct cri_assert_node *cri_prevnode;                                   \
        (void) cri_tmpn;                                                        \
        (void) cri_node;                                                        \
        (void) cri_prevnode;                                                    \
        cri_assert_node_init(&cri_root);                                        \
        int cri_cond, cri_cond_un, *cri_pass = &cri_cond_un;                    \
        int cri_cond_def = 1;                                                   \
        int cri_cond_expect = 1;                                                \
        (void) cri_cond_def;                                                    \
        (void) cri_pass;                                                        \
        cri_cond_un = CRI_ASSERT_SPECIFIER(Condition);                          \
        cri_cond = cri_cond_un;                                                 \
        if (cri_cond != cri_cond_expect) {                                      \
            cri_root.message = cri_assert_message("x" CR_VA_TAIL(__VA_ARGS__)); \
            cri_assert_node_send(File, Line, &cri_root);                        \
            cri_assert_node_term(&cri_root);                                    \
            Fail();                                                             \
        }                                                                       \
    } while (0))

#define CRI_ASSERT_SPECIFIER_VALUE(Val)                                 \
    1; do {                                                             \
        cri_cond_un = (Val);                                            \
        if (cri_cond_un != cri_cond_expect) {                           \
            cri_assert_node_init(&cri_tmpn);                            \
            cri_tmpn.repr = CR_STR(Val);                                \
            cri_tmpn.pass = cri_cond_un;                                \
            cri_prevnode = cri_assert_node_add(cri_node, &cri_tmpn);    \
        }                                                               \
    } while (0)

/* Logical specifiers */

#define CRI_ASSERT_TEST_SPECIFIER_not(...)    ,
#define CRI_ASSERT_SPECIFIER_not(E)              \
    cri_cond_expect = !cri_cond_expect;          \
    CRI_OBSTRUCT_N(CRI_SPECIFIER_INDIRECT)()(E); \
    cri_cond_expect = !cri_cond_expect;          \
    cri_cond_un = !cri_cond_un;                  \
    if (cri_cond_un != cri_cond_expect)          \
        cri_assert_node_negate(cri_prevnode)

#define CRI_ASSERT_SPECIFIER_ALL_INDIRECT(Cond, E);             \
    cri_cond_un = CRI_OBSTRUCT_N(CRI_SPECIFIER_INDIRECT)()(E);  \
    Cond = Cond && cri_cond_un

#define CRI_ASSERT_TEST_SPECIFIER_all(...)            ,
#define CRI_ASSERT_SPECIFIER_all(...)                                                   \
    cri_cond_def; int *cri_pass_orig = cri_pass; cri_pass = &cri_cond_un; do {          \
        cri_assert_node_init(&cri_tmpn);                                                \
        struct cri_assert_node *cri_tmp = cri_assert_node_add(cri_node, &cri_tmpn);     \
        struct cri_assert_node *cri_node = cri_tmp;                                     \
        int cri_cond_def = 1, cri_cond_un;                                              \
        int cri_cond = cri_cond_def                                                     \
            CRITERION_APPLY(CRI_ASSERT_SPECIFIER_ALL_INDIRECT, cri_cond, __VA_ARGS__);  \
        cri_node->pass = cri_cond;                                                      \
        *cri_pass = *cri_pass && cri_cond;                                              \
        cri_prevnode = cri_node;                                                        \
    } while (0); cri_pass = cri_pass_orig

#define CRI_ASSERT_SPECIFIER_NONE_INDIRECT(Cond, E);            \
    cri_cond_expect = !cri_cond_expect;                         \
    cri_cond_un = CRI_OBSTRUCT_N(CRI_SPECIFIER_INDIRECT)()(E);  \
    if (cri_cond_un != cri_cond_expect)                         \
      cri_assert_node_negate(cri_prevnode);                     \
    cri_cond_expect = !cri_cond_expect;                         \
    Cond = Cond && !(cri_cond_un)

#define CRI_ASSERT_TEST_SPECIFIER_none(...)            ,
#define CRI_ASSERT_SPECIFIER_none(...)                                                  \
    cri_cond_def; int *cri_pass_orig = cri_pass; cri_pass = &cri_cond_un; do {          \
        cri_assert_node_init(&cri_tmpn);                                                \
        struct cri_assert_node *cri_tmp = cri_assert_node_add(cri_node, &cri_tmpn);     \
        struct cri_assert_node *cri_node = cri_tmp;                                     \
        int cri_cond_def = 1, cri_cond_un;                                              \
        int cri_cond = cri_cond_def                                                     \
            CRITERION_APPLY(CRI_ASSERT_SPECIFIER_NONE_INDIRECT, cri_cond, __VA_ARGS__); \
        cri_node->pass = cri_cond;                                                      \
        *cri_pass = *cri_pass && cri_cond;                                              \
        cri_prevnode = cri_node;                                                        \
    } while (0); cri_pass = cri_pass_orig

#define CRI_ASSERT_SPECIFIER_ANY_INDIRECT(Cond, E)              \
    ; Cond = Cond || CRI_OBSTRUCT_N(CRI_SPECIFIER_INDIRECT)()(E)

#define CRI_ASSERT_TEST_SPECIFIER_any(...)            ,
#define CRI_ASSERT_SPECIFIER_any(...)                                                   \
    cri_cond_def; int *cri_pass_orig = cri_pass; cri_pass = &cri_cond_un; do {          \
        cri_assert_node_init(&cri_tmpn);                                                \
        struct cri_assert_node *cri_tmp = cri_assert_node_add(cri_node, &cri_tmpn);     \
        struct cri_assert_node *cri_node = cri_tmp;                                     \
        int cri_cond_def = 0;                                                           \
        int cri_cond = cri_cond_def                                                     \
            CRITERION_APPLY(CRI_ASSERT_SPECIFIER_ANY_INDIRECT, cri_cond, __VA_ARGS__);  \
        cri_node->pass = cri_cond;                                                      \
        *cri_pass = *cri_pass && cri_cond;                                              \
        cri_prevnode = cri_node;                                                        \
    } while (0); cri_pass = cri_pass_orig

#undef cr_assert_user
#define cr_assert_user(File, Line, Fail, ...) \
    CRI_ASSERT_CALL(File, Line, Fail, CR_VA_HEAD(__VA_ARGS__), , CR_VA_TAIL(__VA_ARGS__))

#undef cr_assert_fail_user
#define cr_assert_fail_user(File, Line, Fail, ...)    CRI_ASSERT_FAIL(File, Line, Fail, __VA_ARGS__)

#undef cr_assert
#define cr_assert(...)    cr_assert_user(__FILE__, __LINE__, criterion_abort_test, __VA_ARGS__)

#undef cr_expect
#define cr_expect(...)    cr_assert_user(__FILE__, __LINE__, criterion_continue_test, __VA_ARGS__)

#undef cr_fatal
#define cr_fatal(...)    cr_assert_fail_user(__FILE__, __LINE__, criterion_abort_test, __VA_ARGS__)

#undef cr_fail
#define cr_fail(...)    cr_assert_fail_user(__FILE__, __LINE__, criterion_continue_test, __VA_ARGS__)

#undef cr_skip
#define cr_skip(...)    criterion_skip_test("" __VA_ARGS__)

#endif /* !CRITERION_INTERNAL_NEW_ASSERTS_H_ */
