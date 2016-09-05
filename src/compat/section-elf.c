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
#include "section.h"

#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

static int open_self(void)
{
#if defined __linux__
    return open("/proc/self/exe", O_RDONLY);
#elif defined __NetBSD__
    return open("/proc/curproc/exe", O_RDONLY)
#elif defined __FreeBSD__
    int fd = open("/proc/curproc/file", O_RDONLY);
    /* Fallback */
    if (fd == -1 && errno == ENOENT) {
        int mib[4] = { CTL_KERN, KERN_PROC, KERN_PROC_PATHNAME, -1 };
        char path[PATH_MAX];
        size_t cb = sizeof (path);
        sysctl(mib, sizeof (mib) / sizeof (int), path, &cb, NULL, 0);
        fd = open(path, O_RDONLY);
    }
    return fd;
#elif defined __OpenBSD__ || defined __DragonFly__
    return open("/proc/curproc/file", O_RDONLY)
#else
    return -1;
#endif
}

static int open_module_map(mod_handle *mod)
{
    /* Load the ELF header and the section header table */
    const ElfW(Ehdr) * elf = mmap(NULL, sizeof (ElfW(Ehdr)),
                    PROT_READ, MAP_PRIVATE, mod->fd, 0);
    const void *oldm = elf;

    if (elf == MAP_FAILED)
        goto fail;

    if (memcmp(elf->e_ident, (char[4]) { 0x7f, 'E', 'L', 'F' }, 4))
        goto fail;

    size_t new_map_len = elf->e_shoff + elf->e_shnum * elf->e_shentsize;

#ifdef HAVE_MREMAP
    elf = mremap((void *) elf, sizeof (ElfW(Ehdr)), new_map_len, MREMAP_MAYMOVE);
#else
    elf = mmap(NULL, new_map_len, PROT_READ, MAP_PRIVATE, mod->fd, 0);
#endif

    if (elf == MAP_FAILED)
        goto fail;

#ifndef HAVE_MREMAP
    munmap((void *) oldm, sizeof (ElfW(Ehdr)));
#else
    (void) oldm;
#endif

    mod->len = new_map_len;
    mod->map = elf;
    return 1;

fail:
    munmap((void *) elf, sizeof (ElfW(Ehdr)));
    return 0;
}

static void close_module_map(mod_handle *mod)
{
    munmap((void *) mod->map, mod->len);
}

int open_module_self(mod_handle *mod)
{
    int fd = open_self();

    if (fd == -1)
        return 0;

    mod->fd = fd;
    return open_module_map(mod);
}

void close_module(mod_handle *mod)
{
    close_module_map(mod);
    close(mod->fd);
}

static const void *map_shdr(int fd, const ElfW (Shdr) *shdr, struct section_mapping *out)
{
    size_t shdr_map_off = shdr->sh_offset & ~0xfffllu;
    size_t shdr_map_len = shdr->sh_size + (shdr->sh_offset - shdr_map_off);

    const uint8_t *shdr_map = mmap(NULL, shdr_map_len,
                    PROT_READ, MAP_PRIVATE, fd, shdr_map_off);

    if (shdr_map == MAP_FAILED)
        return NULL;

    *out = (struct section_mapping) {
        .map = shdr_map,
        .len = shdr_map_len
    };

    return shdr_map + (shdr->sh_offset - shdr_map_off);
}

static void unmap_shdr(struct section_mapping *map)
{
    munmap((void *) map->map, map->len);
}

void *map_section_data(mod_handle *mod, const char *name,
        struct section_mapping *map)
{
    const ElfW(Shdr) * shdr = (void *) ((char *) mod->map + mod->map->e_shoff);
    const ElfW(Shdr) * shstr_shdr = shdr + mod->map->e_shstrndx;

    struct section_mapping shstr_map;
    const char *shstr = map_shdr(mod->fd, shstr_shdr, &shstr_map);

    const void *ptr = NULL;
    for (size_t i = 0; i < mod->map->e_shnum; i++) {
        const char *section_name = shstr + shdr[i].sh_name;
        if (!strcmp(section_name, name)) {
            const ElfW(Shdr) * hdr = shdr + i;
            ptr = map_shdr(mod->fd, hdr, map);
            map->sec_len = hdr->sh_size;
            break;
        }
    }

    unmap_shdr(&shstr_map);
    return (void *) ptr;
}

void unmap_section_data(struct section_mapping *map)
{
    unmap_shdr(map);
}
