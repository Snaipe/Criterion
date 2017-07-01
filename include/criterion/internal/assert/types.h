/*
 * The MIT License (MIT)
 *
 * Copyright © 2016 Franklin "Snaipe" Mathieu <http://snai.pe/>
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
#ifndef CRITERION_INTERNAL_ASSERT_TYPES_H_
#define CRITERION_INTERNAL_ASSERT_TYPES_H_

enum cri_assert_result_type {
    CRI_ASSERT_RT_RAW,
    CRI_ASSERT_RT_STR,
};

#define CRI_ASSERT_MAX_PARAMS    4

struct cri_assert_param {
    const char *name;
    void *data;
    enum cri_assert_result_type type;
};

struct cri_assert_node {
    const char *repr;
    const char *message;
    struct cri_assert_param params[CRI_ASSERT_MAX_PARAMS];

    unsigned pass : 1;
    unsigned dynrepr : 1;
    uint32_t nchild : 30;
    uint32_t maxchild;
    struct cri_assert_node *children;
};

CR_BEGIN_C_API

CR_API void cri_assert_node_init(struct cri_assert_node *node);
CR_API struct cri_assert_node *cri_assert_node_add(struct cri_assert_node *tree,
        struct cri_assert_node *node);
CR_API void cri_assert_node_negate(struct cri_assert_node *tree);
CR_API void cri_assert_node_term(struct cri_assert_node *tree);
CR_API void cri_assert_node_send(const char *file, size_t line, struct cri_assert_node *tree);
CR_API char *cri_assert_message(const char *fmt, ...);
CR_API void *cri_assert_mkbytes(size_t size, const void *bytes);

CR_END_C_API

#endif /* !CRITERION_INTERNAL_ASSERT_TYPES_H_ */
