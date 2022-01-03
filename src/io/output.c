/*
 * The MIT License (MIT)
 *
 * Copyright © 2015-2016 Franklin "Snaipe" Mathieu <http://snai.pe/>
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
#include <stdio.h>
#include <string.h>
#include <khash.h>
#include <kvec.h>
#include <errno.h>
#include "criterion/options.h"
#include "criterion/output.h"
#include "log/logging.h"
#include "string/i18n.h"
#include "compat/path.h"

typedef const char *const msg_t;

#ifdef ENABLE_NLS
static msg_t msg_err = N_("Could not open the file @ `%1$s` for %2$s reporting: %3$s.\n");
static msg_t msg_ok  = N_("Writing %1$s report in `%2$s`.\n");
#else
static msg_t msg_err = "Could not open the file @ `%s` for %s reporting: %s.\n";
static msg_t msg_ok  = "Writing %s report in `%s`.\n";
#endif

typedef kvec_t (const char *) str_vec;

KHASH_MAP_INIT_STR(ht_str, criterion_reporter *)
KHASH_MAP_INIT_STR(ht_path, str_vec *)

static khash_t(ht_str) * reporters;
static khash_t(ht_path) * outputs;

int criterion_register_output_provider(const char *name, criterion_reporter *reporter)
{
    if (!reporters)
        reporters = kh_init(ht_str);

    int absent;
    khint_t k = kh_put(ht_str, reporters, name, &absent);
    kh_value(reporters, k) = reporter;
    return absent;
}

int criterion_add_output(const char *provider, const char *path)
{
    if (!outputs)
        outputs = kh_init(ht_path);

    khint_t k = kh_get(ht_path, outputs, provider);

    if (k == kh_end(outputs)) {
        int absent;
        k = kh_put(ht_path, outputs, provider, &absent);
        if (absent == -1)
            return -1;

        str_vec *vec = malloc(sizeof (str_vec));
        kv_init(*vec);
        kh_value(outputs, k) = vec;
    }
    str_vec *vec = kh_value(outputs, k);

    kv_push(const char *, *vec, path);
    return 1;
}

void criterion_free_output(void)
{
    if (reporters)
        kh_destroy(ht_str, reporters);

    if (outputs) {
        for (khint_t k = kh_begin(outputs); k != kh_end(outputs); ++k) {
            if (!kh_exist(outputs, k))
                continue;
            str_vec *vec = kh_value(outputs, k);
            kv_destroy(*vec);
            free(vec);
        }
        kh_destroy(ht_path, outputs);
    }
}

void process_all_output(struct criterion_global_stats *stats)
{
    if (!outputs || !reporters)
        return;

    for (khint_t k = kh_begin(reporters); k != kh_end(reporters); ++k) {
        if (!kh_exist(reporters, k))
            continue;

        criterion_reporter *report = kh_value(reporters, k);
        const char *name = kh_key(reporters, k);
        khint_t ko = kh_get(ht_path, outputs, name);
        if (ko == kh_end(outputs))
            continue;

        str_vec *vec = kh_value(outputs, ko);
        for (size_t i = 0; i < kv_size(*vec); ++i) {
            const char *path = kv_A(*vec, i);

            FILE *f;
            if (!strcmp(path, "-")) {
                f = stdout;
            } else if (!strcmp(path, "/dev/stderr")) {
                f = stderr;
            } else if (cri_path_isdirectory(path)) {
                const char *short_executable_name = basename_compat(criterion_options.executable_name);
                char *output_path = cri_path_gen_unique_filename(path, short_executable_name, name);
                f = fopen(output_path, "w");

                if (!f) {
                    int errno2 = errno;
                    criterion_perror(_(msg_err), output_path, name, strerror(errno2));
                    free(output_path);
                    continue;
                }

                free(output_path);
            } else {
                f = fopen(path, "w");
            }

            if (!f) {
                int errno2 = errno;
                criterion_perror(_(msg_err), path, name, strerror(errno2));
                continue;
            }

            criterion_pinfo(CRITERION_PREFIX_DASHES, _(msg_ok), name, path);
            report(f, stats);

            if (f != stdout && f != stderr)
                fclose(f);
        }
    }
}
