/**
 * cpuid.h - cpuid main header
 * System sources under license MIT
 */

#ifndef LIBC_COMPILER_H
#define LIBC_COMPILER_H

/************************************
 *          Note on stdcall         *
 *----------------------------------*
 * Callee must save registers.      *
 *                                  *
 ************************************/

#define STDCALL __attribute__((stdcall))

/************************************
 *           Note on cdecl          *
 *----------------------------------*
 * EAX, ECX, EDX are caller saved   *
 *                                  *
 * Arguments are push to the stack  *
 * in the RTL order.                *
 * Integer and pointer returned     *
 * by EAX.                          *
 * Struct returned via stack        *
 * allocated buffer.                *
 ************************************/

#define CDECL   __attribute__((cdecl))

#endif // LIBC_COMPILER_H
