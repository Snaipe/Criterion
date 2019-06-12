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
#define _GNU_SOURCE
#include "section.h"
#include "err.h"

#include <link.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#ifdef __FreeBSD__
# include <sys/sysctl.h>
# include <sys/elf_generic.h>
# define ElfW(type)      ElfW_(Elf, type)
# define ElfW_(e, t)     ElfW__(e, _ ## t)
# define ElfW__(e, t)    e ## t
#endif

struct mod_handle {
    int fd;
    const ElfW(Ehdr) * map;
    size_t len;
};

struct section_mapping {
    const void *map;
    size_t len;
    size_t sec_len;
};

static int open_self(void)
{
#if defined (__linux__)
    const char *self = "/proc/self/exe";
#elif defined __NetBSD__
    const char *self = "/proc/curproc/exe";
#elif defined __FreeBSD__
    char self[PATH_MAX];

    int mib[4] = { CTL_KERN, KERN_PROC, KERN_PROC_PATHNAME, -1 };
    size_t cb = sizeof (self);
    sysctl(mib, sizeof (mib) / sizeof (int), self, &cb, NULL, 0);
#elif defined __OpenBSD__ || defined __DragonFly__
    const char *self = "/proc/curproc/file";
#else
    errno = ENOTSUP;
    return -1;
#endif

    /* We can't just use /proc/self/exe or equivalent to re-exec the
       executable, because tools like valgrind use this path to open
       and map the ELF file -- which would point to the valgrind binary. */
    char fullpath[PATH_MAX];
    ssize_t rc = readlink(self, fullpath, PATH_MAX);
    if (rc == -1) {
        if (errno == EINVAL) {
            strncpy(fullpath, self, PATH_MAX);
            goto do_open;
        }
        return -1;
    }
    if ((size_t) rc == PATH_MAX) {
        errno = ENAMETOOLONG;
        return -1;
    }
    memset(fullpath + rc, 0, PATH_MAX - rc);

do_open:
    return open(fullpath, O_RDONLY);
}

static int open_module_map(struct mod_handle *mod)
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

static void close_module_map(struct mod_handle *mod)
{
    munmap((void *) mod->map, mod->len);
}

static int open_module(const char *name, struct mod_handle *mod)
{
    int fd;

    if (!name)
        fd = open_self();
    else
        fd = open(name, O_RDONLY);

    if (fd == -1)
        return 0;

    mod->fd = fd;
    if (!open_module_map(mod)) {
        close(mod->fd);
        return 0;
    }

    return 1;
}

static void close_module(struct mod_handle *mod)
{
    close_module_map(mod);
    close(mod->fd);
}

static const void *map_shdr(int fd, const ElfW (Shdr) *shdr,
        struct section_mapping *out)
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

static int get_section_data(struct mod_handle *mod, const char *name,
        void *base, struct cri_section *sect)
{
    const ElfW(Shdr) * shdr = (void *) ((char *) mod->map + mod->map->e_shoff);
    const ElfW(Shdr) * shstr_shdr = shdr + mod->map->e_shstrndx;

    struct section_mapping shstr_map;
    const char *shstr = map_shdr(mod->fd, shstr_shdr, &shstr_map);

    for (size_t i = 0; i < mod->map->e_shnum; i++) {
        const char *section_name = shstr + shdr[i].sh_name;
        if (!strcmp(section_name, name)) {
            const ElfW(Shdr) * hdr = shdr + i;
            sect->addr = (char *) base + hdr->sh_addr;
            sect->length = hdr->sh_size;
            unmap_shdr(&shstr_map);
            return 1;
        }
    }

    unmap_shdr(&shstr_map);
    return 0;
}

struct callback {
    const char *sectname;
    struct cri_section *sect;
    size_t size;
    size_t i;
    size_t libnum;
};

static int section_getaddr(struct dl_phdr_info *info,
        CR_UNUSED size_t size, void *data)
{
    struct callback *ctx = data;
    struct mod_handle mod;

    size_t libnum = ctx->libnum++;

    /* a libnum of 0 means the current executable */
    if (!open_module(libnum ? info->dlpi_name : NULL, &mod))
        return 0;

    struct cri_section sect;

    if (get_section_data(&mod, ctx->sectname, (void *) info->dlpi_addr, &sect)) {
        if (ctx->i >= ctx->size) {
            ctx->size *= 1.5f;
            ctx->sect = realloc(ctx->sect, sizeof (struct cri_section) * (ctx->size + 1));
            if (!ctx->sect)
                cr_panic("Could not allocate cri_section");
        }

        ctx->sect[ctx->i] = sect;
        ctx->sect[ctx->i + 1].addr = NULL;
        ++ctx->i;
    }

    close_module(&mod);
    return 0;
}

int cri_sections_getaddr(const char *sectname, struct cri_section **out)
{
    struct callback ctx = {
        .sectname = sectname,
        .sect     = malloc(sizeof (struct cri_section) * 3),
        .size     = 2,
    };

    if (!ctx.sect)
        cr_panic("Could not allocate cri_section");

    ctx.sect[0].addr = NULL;

    dl_iterate_phdr(section_getaddr, &ctx);
    *out = ctx.sect;
    return 0;
}
