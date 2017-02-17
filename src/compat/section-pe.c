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
#include <windows.h>
#include <tlhelp32.h>

#include "err.h"
#include "section.h"

int section_getaddr(HMODULE mod, const char *sectname,
        struct cri_section *sect)
{
    PIMAGE_DOS_HEADER dos_hdr = (PIMAGE_DOS_HEADER) mod;
    PIMAGE_NT_HEADERS nt_hdr = (PIMAGE_NT_HEADERS) ((uintptr_t) dos_hdr
            + dos_hdr->e_lfanew);

    PIMAGE_SECTION_HEADER sec_hdr = IMAGE_FIRST_SECTION(nt_hdr);

    for (int i = 0; i < nt_hdr->FileHeader.NumberOfSections; i++, sec_hdr++) {
        if (*(char *) sec_hdr == '/')
            continue;

        if (!strncmp((char *) sec_hdr->Name, sectname, IMAGE_SIZEOF_SHORT_NAME)) {
            sect->length = sec_hdr->SizeOfRawData;
            sect->addr = (char *) dos_hdr + sec_hdr->VirtualAddress;
            return 1;
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

    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE,
                    GetCurrentProcessId());
    if (snap == INVALID_HANDLE_VALUE)
        cr_panic("CreateToolhelp32Snapshot returned INVALID_HANDLE_VALUE");

    MODULEENTRY32 mod = { .dwSize = sizeof (MODULEENTRY32) };
    for (BOOL more = Module32First(snap, &mod); more;
            more = Module32Next(snap, &mod)) {
        struct cri_section s;

        if (!section_getaddr(mod.hModule, sectname, &s))
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
    unsigned long err = (unsigned long) GetLastError();
    if (err != ERROR_NO_MORE_FILES)
        cr_panic("Error %lu while iterating modules", err);

    *out = sect;
    return 0;
}
