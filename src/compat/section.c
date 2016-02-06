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
#include <assert.h>
#include <stdint.h>
#include "section.h"
#include "internal.h"

#ifdef _WIN32
void *get_win_section_start(const char *section) {
    PIMAGE_DOS_HEADER dosHeader = (PIMAGE_DOS_HEADER) GetModuleHandle(NULL);
    PIMAGE_NT_HEADERS ntHeader = ntHeader = (PIMAGE_NT_HEADERS) ((uintptr_t)(dosHeader) + (dosHeader->e_lfanew));

    assert(dosHeader->e_magic == IMAGE_DOS_SIGNATURE);
    assert(ntHeader->Signature == IMAGE_NT_SIGNATURE);

    PIMAGE_SECTION_HEADER pSecHeader = IMAGE_FIRST_SECTION(ntHeader);
    for(int i = 0; i < ntHeader->FileHeader.NumberOfSections; i++, pSecHeader++) {
        if (!strncmp((char*) pSecHeader->Name, section, 8)) {
            return (char*) dosHeader + pSecHeader->VirtualAddress;
        }
    }
    return NULL;
}

void *get_win_section_end(const char *section) {
    PIMAGE_DOS_HEADER dosHeader = (PIMAGE_DOS_HEADER) GetModuleHandle(NULL);
    PIMAGE_NT_HEADERS ntHeader = ntHeader = (PIMAGE_NT_HEADERS) ((uintptr_t)(dosHeader) + (dosHeader->e_lfanew));

    assert(dosHeader->e_magic == IMAGE_DOS_SIGNATURE);
    assert(ntHeader->Signature == IMAGE_NT_SIGNATURE);

    PIMAGE_SECTION_HEADER pSecHeader = IMAGE_FIRST_SECTION(ntHeader);
    for(int i = 0; i < ntHeader->FileHeader.NumberOfSections; i++, pSecHeader++) {
        if (!strncmp((char*) pSecHeader->Name, section, 8)) {
            return (char*) dosHeader + (size_t) pSecHeader->VirtualAddress + pSecHeader->SizeOfRawData;
        }
    }
    return NULL;
}
#endif

#ifdef __APPLE__
# include <mach-o/getsect.h>
# include <mach-o/dyld.h>

# define BASE_IMAGE_INDEX 0

static inline void *get_real_address(void *addr) {
    if (!addr)
        return NULL;

    // We need to slide the section address to get a valid pointer
    // because ASLR will shift the image by a random offset
    return addr + _dyld_get_image_vmaddr_slide(BASE_IMAGE_INDEX);
}

void *get_osx_section_start(const char *section) {
    unsigned long secsize;
    return get_real_address(getsectdata("__DATA", section, &secsize));
}

void *get_osx_section_end(const char *section) {
    unsigned long secsize;
    char *section_start = getsectdata("__DATA", section, &secsize);
    return get_real_address(section_start) + secsize;
}
#endif

