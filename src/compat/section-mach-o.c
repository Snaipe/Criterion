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
#include <stdlib.h>
#include <string.h>
#include <mach-o/dyld.h>
#include <mach-o/getsect.h>
#include <mach-o/loader.h>

#include "section.h"

#ifdef __LP64__
typedef struct mach_header_64 mach_hdr;
typedef struct section_64 section;
#else
typedef struct mach_header mach_hdr;
typedef struct section section;
#endif

static inline void *get_real_address(int lib, void *addr)
{
    if (!addr)
        return NULL;

    /* We need to slide the section address to get a valid pointer
       because ASLR will shift the image by a random offset */
    return addr + _dyld_get_image_vmaddr_slide(lib);
}

static inline void *ptr_add(const void *ptr, size_t off)
{
    return (char *) ptr + off;
}

static int section_getaddr(int lib, const char *sectname,
        struct cri_section *sect)
{
    const struct mach_header *hdr = _dyld_get_image_header(lib);
    const struct load_command *lc = ptr_add(hdr, sizeof (mach_hdr));

    for (size_t i = 0; i < hdr->ncmds; ++i, lc = ptr_add(lc, lc->cmdsize)) {
        if (lc->cmd == LC_SEGMENT) {
            const struct segment_command *sc = (void *) lc;
            if (strncmp("__DATA", sc->segname, 16))
                continue;
            const struct section *s = ptr_add(sc, sizeof (*sc));
            for (size_t j = 0; j < sc->nsects; ++j, ++s) {
                if (strncmp(sectname, s->sectname, 16))
                    continue;

                sect->length = s->size;
                sect->addr = get_real_address(lib, (void *) (uintptr_t) s->addr);
                return 1;
            }
        } else if (lc->cmd == LC_SEGMENT_64) {
            const struct segment_command_64 *sc = (void *) lc;
            if (strncmp("__DATA", sc->segname, 16))
                continue;
            const struct section_64 *s = ptr_add(sc, sizeof (*sc));
            for (size_t j = 0; j < sc->nsects; ++j, ++s) {
                if (strncmp(sectname, s->sectname, 16))
                    continue;

                sect->length = s->size;
                sect->addr = get_real_address(lib, (void *) (uintptr_t) s->addr);
                return 1;
            }
        }
    }
    return 0;
}

int cri_sections_getaddr(const char *sectname, struct cri_section **out)
{
    struct cri_section *sect = malloc(sizeof (struct cri_section) * 3);

    if (!sect)
        cr_panic("Could not allocate cri_section");

    sect[0].addr = NULL;

    size_t i = 0;
    size_t size = 2;

    size_t nb_images = _dyld_image_count();
    for (size_t lib = 0; lib < nb_images; ++lib) {
        struct cri_section s;

        if (!section_getaddr(lib, sectname, &s))
            continue;

        if (i >= size) {
            size *= 1.5f;
            sect = realloc(sect, sizeof (struct cri_section) * (size + 1));
            if (!sect)
                cr_panic("Could not allocate cri_section");
        }

        sect[i] = s;
        sect[i + 1].addr = NULL;
        ++i;
    }

    *out = sect;
    return 0;
}
