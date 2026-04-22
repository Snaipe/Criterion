#include <stdio.h>

#include "criterion/criterion.h"
#include "event.h"
#include "assert.h"
#include "../core/filc-simple.h"

int g_client_socket = -1;

void criterion_send_assert(struct criterion_assert_stats *stats)
{
    if (stats->passed)
        return;

    cri_filc_mark_failed();
    if (stats->message && *stats->message) {
        fprintf(stderr, "%s:%u: %s\n", stats->file, stats->line, stats->message);
    } else {
        fprintf(stderr, "%s:%u: assertion failed\n", stats->file, stats->line);
    }
}
