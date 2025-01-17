/*
 * Mesa 3-D graphics library
 *
 * Copyright (C) 2010 LunarG Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * Authors:
 *    Chia-I Wu <olv@lunarg.com>
 */

#include <stdlib.h>
#include <stdint.h>

#include "entry.h"
#include "u_current.h"
#include "util/u_endian.h"
#include "util/u_thread.h"

#define _U_STRINGIFY(x) #x
#define U_STRINGIFY(x) _U_STRINGIFY(x)

/* define macros for use by assembly dispatchers */
#define ENTRY_CURRENT_TABLE U_STRINGIFY(u_current_table)

/* in bridge mode, mapi is a user of glapi */
#ifdef MAPI_MODE_BRIDGE
#define ENTRY_CURRENT_TABLE_GET "_glapi_get_dispatch"
#else
#define ENTRY_CURRENT_TABLE_GET U_STRINGIFY(u_current_get_table_internal)
#endif

/* REALLY_INITIAL_EXEC implies USE_ELF_TLS and __GNUC__ */
#if defined(USE_X86_ASM) && defined(REALLY_INITIAL_EXEC)
#include "entry_x86_tls.h"
#elif defined(USE_X86_ASM) && !defined(GLX_X86_READONLY_TEXT) && defined(__GNUC__)
#include "entry_x86_tsd.h"
#elif defined(USE_X86_64_ASM) && defined(REALLY_INITIAL_EXEC)
#include "entry_x86-64_tls.h"
#elif defined(USE_PPC64LE_ASM) && UTIL_ARCH_LITTLE_ENDIAN && defined(REALLY_INITIAL_EXEC)
#include "entry_ppc64le_tls.h"
/* ppc64le non-IE TSD stubs are possible but not currently implemented */
#elif defined(USE_PPC64LE_ASM) && UTIL_ARCH_LITTLE_ENDIAN && !defined(USE_ELF_TLS) && defined(__GNUC__)
#include "entry_ppc64le_tsd.h"
#else

static inline const struct _glapi_table *
entry_current_get(void)
{
#ifdef MAPI_MODE_BRIDGE
   return GET_DISPATCH();
#else
   return u_current_get_table_internal();
#endif
}

/* C version of the public entries */
#define MAPI_TMP_DEFINES
#define MAPI_TMP_PUBLIC_DECLARES
#define MAPI_TMP_PUBLIC_ENTRIES
#include "mapi_tmp.h"

#ifndef MAPI_MODE_BRIDGE

void
entry_patch_public(void)
{
}

mapi_func
entry_get_public(int slot)
{
   /* pubic_entries are defined by MAPI_TMP_PUBLIC_ENTRIES */
   return public_entries[slot];
}

#endif /* MAPI_MODE_BRIDGE */

#if defined(_WIN32) && defined(_WINDOWS_)
#error "Should not include <windows.h> here"
#endif

#endif /* asm */
