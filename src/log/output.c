#include <stdio.h>
#include <string.h>
#include <khash.h>
#include "criterion/output.h"

KHASH_MAP_INIT_STR(ht_str, criterion_reporter*)
KHASH_MAP_INIT_STR(ht_path, const char *)

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
    int absent;
    khint_t k = kh_put(ht_path, outputs, provider, &absent);
    kh_value(outputs, k) = path;
    return absent;
}

void criterion_free_output(void) {
    kh_destroy(ht_str, reporters);
    kh_destroy(ht_path, outputs);
}
