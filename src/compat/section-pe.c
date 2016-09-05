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
#include <windows.h>

int open_module_self(mod_handle *mod)
{
    *mod = GetModuleHandle(NULL);
    return 1;
}

void close_module(mod_handle *mod)
{
    (void) mod;
}

void *map_section_data(mod_handle *mod, const char *name,
        struct section_mapping *map)
{
    PIMAGE_DOS_HEADER dos_hdr = (PIMAGE_DOS_HEADER) *mod;
    PIMAGE_NT_HEADERS nt_hdr = (PIMAGE_NT_HEADERS) ((uintptr_t) dos_hdr
            + dos_hdr->e_lfanew);

    PIMAGE_SECTION_HEADER sec_hdr = IMAGE_FIRST_SECTION(nt_hdr);

    for (int i = 0; i < nt_hdr->FileHeader.NumberOfSections; i++, sec_hdr++) {
        if (!strncmp((char *) sec_hdr->Name, name, IMAGE_SIZEOF_SHORT_NAME)) {
            map->sec_len = sec_hdr->SizeOfRawData;
            return (char *) dos_hdr + sec_hdr->VirtualAddress;
        }
    }
    return NULL;
}

void unmap_section_data(struct section_mapping *map)
{
    (void) map;
}
