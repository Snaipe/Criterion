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
#ifndef CRITERION_INTERNAL_ASSERT_EXCEPTIONS_H_
#define CRITERION_INTERNAL_ASSERT_EXCEPTIONS_H_

/* Exception specifiers */

#define CRI_ASSERT_TEST_SPECIFIER_throw(...) ,
#define CRI_ASSERT_SPECIFIER_throw(Ex, Stmt)                        \
    cri_cond_def; do {                                              \
        cri_assert_node_init(&cri_tmpn);                            \
        cri_tmpn.repr = "throw(" #Ex ", " #Stmt ")";                \
        try {                                                       \
            Stmt;                                                   \
        } catch (Ex &cri_ex) {                                      \
            cri_cond_un = 1;                                        \
        } catch (std::exception &cri_ex) {                          \
            cri_cond_un = 0;                                        \
            /* TODO: Add std::exception message */                  \
        } catch (...) {                                             \
            cri_cond_un = 0;                                        \
            /* TODO: Add generic exception message */               \
        }                                                           \
        cri_prevnode = cri_assert_node_add(cri_node, &cri_tmpn);    \
    } while (0)

#define CRI_ASSERT_TEST_SPECIFIER_nothrow(...) ,
#define CRI_ASSERT_SPECIFIER_nothrow(Stmt)                          \
    cri_cond_def; do {                                              \
        cri_assert_node_init(&cri_tmpn);                            \
        cri_tmpn.repr = "throw(" #Stmt ")";                         \
        try {                                                       \
            Stmt;                                                   \
            cri_cond_un = 1;                                        \
        } catch (std::exception &cri_ex) {                          \
            cri_cond_un = 0;                                        \
            /* TODO: Add std::exception message */                  \
        } catch (...) {                                             \
            cri_cond_un = 0;                                        \
            /* TODO: Add generic exception message */               \
        }                                                           \
        cri_prenode = cri_assert_node_add(cri_node, &cri_tmpn);     \
    } while (0)

#define CRI_ASSERT_TEST_SPECIFIER_anythrow(...) ,
#define CRI_ASSERT_SPECIFIER_anythrow(Stmt)                         \
    cri_cond_def; do {                                              \
        cri_assert_node_init(&cri_tmpn);                            \
        cri_tmpn.repr = "anythrow(" #Stmt ")";                      \
        try {                                                       \
            Stmt;                                                   \
            cri_cond_un = 0;                                        \
        } catch (...) {                                             \
            cri_cond_un = 1;                                        \
            /* TODO: Add generic exception message */               \
        }                                                           \
        cri_prevnode = cri_assert_node_add(cri_node, &cri_tmpn);    \
    } while (0)

#endif /* !CRITERION_INTERNAL_ASSERT_EXCEPTIONS_H_ */
