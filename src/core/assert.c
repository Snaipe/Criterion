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
#include "string/diff.h"

CR_API void cri_assert_node_init(struct cri_assert_node *node)
{
    *node = (struct cri_assert_node) { .nchild = 0 };
}

CR_API struct cri_assert_node *cri_assert_node_add(struct cri_assert_node *tree,
        struct cri_assert_node *node)
{
    if (tree->nchild + 1u >= tree->maxchild) {
        tree->maxchild = (tree->maxchild + 1u) * 1.5;
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
    for (struct cri_assert_param *p = &tree->params[0]; p->name; ++p)
        free(p->data);
    for (size_t i = 0; i < tree->nchild; ++i)
        cri_assert_node_term(&tree->children[i]);
    free((char *) tree->message);
    free(tree->children);
    if (tree->dynrepr)
        free((char *) tree->repr);
}

static size_t leaf_count(struct cri_assert_node *tree)
{
    size_t count = 0;

    for (size_t i = 0; i < tree->nchild; ++i) {
        struct cri_assert_node *node = &tree->children[i];
        if (!node->pass)
            ++count;
        if (node->nchild > 0)
            count += leaf_count(&tree->children[i]);
    }
    return count;
}

static inline void make_obj(criterion_protocol_param_entry *obj,
        struct cri_assert_param *p)
{
    *obj = (criterion_protocol_param_entry) {
        .name = (char *) p->name,
    };

    if (p->type == CRI_ASSERT_RT_RAW) {
        obj->which_data = criterion_protocol_param_entry_raw_tag;
        obj->data.raw = p->data;
    } else if (p->type == CRI_ASSERT_RT_STR) {
        obj->which_data = criterion_protocol_param_entry_str_tag;
        obj->data.str = p->data;
    } else {
        criterion_test_die("Unknown assert param type %d", p->type);
    }
}

static criterion_protocol_result *collect_leaves(
    criterion_protocol_result *res, struct cri_assert_node *tree)
{
    size_t nbparams = 0;
    for (struct cri_assert_param *p = tree->params; p->name; ++p)
        ++nbparams;

    /* If there are any parameters, it means that the children have
       already been digested. No need to recurse down */
    if (nbparams > 0) {
        res->repr = (char *) tree->repr;
        res->message = (char *) tree->message;

        const size_t display_threshold = 30;

        if (nbparams == 2) {
            if (strcmp(tree->params[0].name, "actual")
                    || strcmp(tree->params[1].name, "expected")) {
                goto process_params;
            }

            size_t expected_len = strlen(tree->params[0].data);
            size_t actual_len = strlen(tree->params[1].data);

            if (actual_len <= display_threshold
                    && expected_len <= display_threshold
                    && !strchr(tree->params[0].data, '\n')
                    && !strchr(tree->params[1].data, '\n')) {
                goto process_params;
            }

            res->which_value = criterion_protocol_result_formatted_tag;

            struct cri_diff_buffer expected = {
                .ptr  = tree->params[0].data,
                .size = expected_len + 1,
            };
            struct cri_diff_buffer actual = {
                .ptr  = tree->params[1].data,
                .size = actual_len + 1,
            };
            struct cri_diff_buffer diff;

            /* Replace null-terminating byte by newline. This is fine
               since we don't actually want to process this as a string
               but as a regular char buffer.

               This is done to normalize the diff inputs to be
               newline-terminated. */
            expected.ptr[expected_len] = '\n';
            actual.ptr[actual_len] = '\n';

            int rc = cri_diff_buffer_to_buffer(&expected, &actual, &diff);
            if (rc < 0)
                res->value.formatted = NULL;

            res->value.formatted = diff.ptr;
        } else {
        process_params:
            res->which_value = criterion_protocol_result_params_tag;
            res->value.params = malloc(sizeof (*res->value.params));
            res->value.params->list_count = nbparams;
            res->value.params->list = malloc(sizeof (*res->value.params->list) * nbparams);

            size_t j = 0;
            for (struct cri_assert_param *p = tree->params; p->name; ++p, ++j)
                make_obj(&res->value.params->list[j], p);
        }
        return res + 1;
    }

    for (size_t i = 0; i < tree->nchild; ++i) {
        struct cri_assert_node *node = &tree->children[i];
        if (node->pass)
            continue;
        res = collect_leaves(res, node);
    }
    return res;
}

static void destroy_results(criterion_protocol_result *res, size_t len)
{
    for (size_t i = 0; i < len; ++i) {
        if (res[i].which_value == criterion_protocol_result_params_tag) {
            free(res[i].value.params->list);
            free(res[i].value.params);
        } else if (res[i].which_value == criterion_protocol_result_formatted_tag) {
            free(res[i].value.formatted);
        }
    }
}

CR_API void cri_assert_node_send(const char *file, size_t line, struct cri_assert_node *tree)
{
    size_t nb_results = leaf_count(tree);

#ifdef _MSC_VER
    criterion_protocol_result *results = _alloca(sizeof (*results) * nb_results);
#else
    criterion_protocol_result results[nb_results];
#endif
    criterion_protocol_result *last = collect_leaves(results, tree);
    nb_results = last - results;

    criterion_protocol_msg msg = criterion_message(assert,
                    .message = (char *) tree->message,
                    .passed = false,
                    .file = (char *) file,
                    .has_line = true,
                    .line = line,
                    .results_count = nb_results,
                    .results = nb_results ? results : NULL,
                    );

    criterion_message_set_id(msg);
    cr_send_to_runner(&msg);

    destroy_results(results, nb_results);
}

CR_API char *cri_assert_message(const char *fmt, ...)
{
    if (strlen(fmt) == 1)
        return strdup("");

    char *out = NULL;
    va_list vl;
    va_start(vl, fmt);
    if (cr_vasprintf(&out, fmt + 1, vl) == -1)
        criterion_test_die("Could not allocate assert message: %s", strerror(errno));
    va_end(vl);

    return out;
}
