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
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#ifdef _MSC_VER
# include <malloc.h>
#endif

#include "criterion/criterion.h"
#include "criterion/abort.h"
#include "criterion/internal/new_asserts.h"
#include "protocol/protocol.h"
#include "protocol/messages.h"

CR_API void cri_assert_node_init(struct cri_assert_node *node)
{
    *node = (struct cri_assert_node) { .nchild = 0 };
}

CR_API struct cri_assert_node *cri_assert_node_add(struct cri_assert_node *tree,
        struct cri_assert_node *node)
{
    if (tree->nchild + 1 >= tree->maxchild) {
        tree->maxchild = (tree->maxchild + 1) * 1.5;
        tree->children = realloc(tree->children, tree->maxchild * sizeof (*tree->children));
        if (!tree->children)
            criterion_test_die("Could not realloc assert result tree: %s", strerror(errno));
    }
    tree->children[tree->nchild++] = *node;
    return &tree->children[tree->nchild - 1];
}

CR_API void cri_assert_node_negate(struct cri_assert_node *tree)
{
    for (size_t i = 0; i < tree->nchild; ++i)
        cri_assert_node_negate(&tree->children[i]);
    tree->pass = !tree->pass;
}

CR_API void cri_assert_node_term(struct cri_assert_node *tree)
{
    for (size_t i = 0; i < tree->nchild; ++i)
        cri_assert_node_term(&tree->children[i]);
    free(tree->children);
}

static size_t leaf_count(struct cri_assert_node *tree)
{
    size_t count = 0;

    for (size_t i = 0; i < tree->nchild; ++i) {
        struct cri_assert_node *node = &tree->children[i];
        if (node->nchild > 0)
            count += leaf_count(&tree->children[i]);
        if (node->rtype && !node->pass)
            ++count;
    }
    return count;
}

static inline void make_obj(criterion_protocol_result_object *obj,
        void *data, enum cri_assert_result_type rtype)
{
    int type = rtype & 0xff;

    *obj = (criterion_protocol_result_object) {
        .type = (criterion_protocol_result_object_result_type) type,
    };
    if (rtype == CRI_ASSERT_RT_DATA) {
        obj->which_data = criterion_protocol_result_object_raw_tag;
        obj->data.raw = data;
    } else {
        obj->which_data = criterion_protocol_result_object_str_tag;
        obj->data.str = data;
    }
}

static criterion_protocol_result *collect_leaves(
    criterion_protocol_result *res, struct cri_assert_node *tree)
{
    for (size_t i = 0; i < tree->nchild; ++i) {
        struct cri_assert_node *node = &tree->children[i];
        if (node->nchild > 0) {
            res = collect_leaves(res, node);
            continue;
        }
        if (!node->rtype || node->pass)
            continue;

        res->repr = (char *) node->repr;
        res->which_value = criterion_protocol_result_obj_tag;

        if (node->expected) {
            res->value.obj.has_expected = true;
            make_obj(&res->value.obj.expected, node->expected, node->rtype);
        }
        if (node->actual) {
            res->value.obj.has_actual = true;
            make_obj(&res->value.obj.actual, node->actual, node->rtype);
        }
        ++res;
    }
    return res;
}

CR_API void cri_assert_node_send(const char *file, size_t line, struct cri_assert_node *tree)
{
    size_t nb_results = leaf_count(tree);

#ifdef _MSC_VER
    criterion_protocol_result *results = _alloca(sizeof (*results) * nb_results);
#else
    criterion_protocol_result results[nb_results];
#endif

    collect_leaves(results, tree);

    criterion_protocol_msg msg = criterion_message(assert,
                    .message = (char *) tree->repr,
                    .passed = false,
                    .file = (char *) file,
                    .has_line = true,
                    .line = line,
                    .results_count = nb_results,
                    .results = nb_results ? results : NULL,
                    );

    criterion_message_set_id(msg);
    cr_send_to_runner(&msg);
}

CR_API char *cri_assert_message(const char *fmt, ...)
{
    if (strlen(fmt) == 1)
        return "Assertion failed.";

    char *out = NULL;
    va_list vl;
    va_start(vl, fmt);
    if (cr_vasprintf(&out, fmt + 1, vl) == -1)
        criterion_test_die("Could not allocate assert message: %s", strerror(errno));
    va_end(vl);

    return out;
}
