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
#ifndef CLIENT_H_
#define CLIENT_H_

#include <khash.h>
#include <boxfort.h>

/* order matters here */
enum client_state {
    CS_SETUP,
    CS_MAIN,
    CS_TEARDOWN,
    CS_END,

    /* The states belows are non-states that should not be
       added in the state count */
    CS_ABORT,
    CS_TIMEOUT,
    CS_SKIP,
};

/* always make it a power of 2 */
#define CS_MAX_CLIENT_STATES    4

enum client_kind {
    WORKER,
    EXTERN,
};

struct client_ctx {
    enum client_kind kind;
    bxf_instance *instance;
    struct criterion_test_extra_data extern_test_data;
    struct criterion_test extern_test;

    uint32_t state;
    bool alive;
    struct criterion_global_stats *gstats;
    struct criterion_suite_stats *sstats;
    struct criterion_test_stats *tstats;
    struct criterion_test *test;
    struct criterion_suite *suite;
    uint64_t start_time;
};

struct kh_ht_client_s;
struct kh_ht_extern_s;

struct server_ctx {
    int socket;
    struct criterion_suite extern_suite;
    struct criterion_test_extra_data extern_suite_data;
    struct criterion_global_stats *gstats;
    struct criterion_suite_stats *extern_sstats;

    struct kh_ht_client_s *subprocesses;
    struct kh_ht_extern_s *clients;
};

struct client_ctx *process_client_message(struct server_ctx *ctx, const criterion_protocol_msg *msg);

void init_server_context(struct server_ctx *sctx, struct criterion_global_stats *gstats);
void destroy_server_context(struct server_ctx *sctx);
struct client_ctx *add_client_from_worker(struct server_ctx *sctx, struct client_ctx *ctx, bxf_instance *instance);
void remove_client_by_pid(struct server_ctx *sctx, int pid);

#endif /* !CLIENT_H_ */
