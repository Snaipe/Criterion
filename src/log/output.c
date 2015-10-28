#include <stdio.h>
#include <string.h>
#include <khash.h>
#include <kvec.h>
#include "criterion/output.h"

typedef kvec_t(const char *) str_vec;

KHASH_MAP_INIT_STR(ht_str, criterion_reporter*)
KHASH_MAP_INIT_STR(ht_path, str_vec*)

static khash_t(ht_str) *reporters;
static khash_t(ht_path) *outputs;

void criterion_init_output(void) {
    reporters = kh_init(ht_str);
    outputs = kh_init(ht_path);
}

int criterion_register_output_provider(const char *name, criterion_reporter *reporter) {
    int absent;
    khint_t k = kh_put(ht_str, reporters, name, &absent);
    kh_value(reporters, k) = reporter;
    return absent;
}

int criterion_add_output(const char *provider, const char *path) {
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

void criterion_free_output(void) {
    for (khint_t k = kh_begin(outputs); k != kh_end(outputs); ++k) {
        if (!kh_exist(outputs, k))
            continue;
        str_vec *vec = kh_value(outputs, k);
        kv_destroy(*vec);
        free(vec);
    }
    kh_destroy(ht_str, reporters);
    kh_destroy(ht_path, outputs);
}

void process_all_output(struct criterion_global_stats *stats) {
    for (khint_t k = kh_begin(reporters); k != kh_end(reporters); ++k) {
        if (!kh_exist(reporters, k))
            continue;

        criterion_reporter *report = kh_value(reporters, k);
        khint_t ko = kh_get(ht_path, outputs, kh_key(reporters, k));
        if (ko == kh_end(outputs))
            continue;

        str_vec *vec = kh_value(outputs, ko);
        for (size_t i = 0; i < kv_size(*vec); ++i) {
            const char *path = kv_A(*vec, i);

            FILE *f;
            if (path[0] == '-' && !path[1])
                f = stderr;
            else
                f = fopen(path, "w");

            report(f, stats);
        }
    }
}
