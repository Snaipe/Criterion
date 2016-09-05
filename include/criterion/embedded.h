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
#ifndef CRITERION_EMBEDDED_H_
#define CRITERION_EMBEDDED_H_

typedef signed long long int cr_ssize;
typedef unsigned long long int cr_size;
typedef unsigned char cr_byte;
typedef int cr_err;
typedef struct cr_io_ctx cr_io_ctx;

struct cr_embed_vtable {
    cr_err (*init)(struct cr_io_ctx *ctx);
    cr_ssize (*send)(const cr_byte *buf, cr_size size);
    cr_ssize (*recv)(cr_byte *buf, cr_size size);
    cr_err (*term)(struct cr_io_ctx *ctx);
};

struct cr_embed_properties {
    struct cr_embed_vtable vtable;
    struct cr_io_ctx *ctx;
};

int cr_embed_init(struct cr_embed_properties *props);
int cr_embed_term(struct cr_embed_properties *props);

#endif /* !CRITERION_EMBEDDED_H_ */
