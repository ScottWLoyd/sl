/*  sl.h 

A collection of common types, macros and functions I find useful.

Copyright 2017 Scott W. Loyd

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated 
documentation files (the "Software"), to deal in the Software without restriction, including without limitation the 
rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit 
persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the 
Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE 
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR 
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


SL_DEBUG:
Define SL_DEBUG _once_ before #including this file to enable debug code

*/

#ifndef _SL_H_
#define _SL_H_

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Macros
//

#ifdef SL_DEBUG
#define sl_assert(condition) if (!condition) { *(int*)0 = 0; }
#else
#define sl_assert(condigion)
#endif

#ifndef SL_STATIC_ASSERT
    #define SL_STATIC_ASSERT3(cond, msg) typedef char static_assertion_##msg[(!!(cond))*2-1]
    #define SL_STATIC_ASSERT2(cond, line) SL_STATIC_ASSERT3(cond, static_assertion_at_line_##line)
    #define SL_STATIC_ASSERT1(cond, line) SL_STATIC_ASSERT2(cond, line)
    #define SL_STATIC_ASSERT(cond)        SL_STATIC_ASSERT1(cond, __LINE__)
#endif

#if defined(_WIN64) || defined(__x86_64__) || defined(_M_X64) || defined(__64BIT__) || defined(__powerpc64__) || defined(__ppc64__)
    #ifndef SL_ARCH_64_BIT
    #define SL_ARCH_64_BIT 1
    #endif
#else
    #ifndef SL_ARCH_32_BIT
    #define SL_ARCH_32_BIT 1
    #endif
#endif


#define internal        static
#define global          static
#define local_persist   static


typedef unsigned char       u8;
typedef unsigned short      u16;
typedef unsigned int        u32;
typedef unsigned long long  u64;

SL_STATIC_ASSERT(sizeof(u8) == 1);
SL_STATIC_ASSERT(sizeof(u16) == 2);
SL_STATIC_ASSERT(sizeof(u32) == 4);
SL_STATIC_ASSERT(sizeof(u64) == 8);

typedef char        i8;
typedef short       i16;
typedef int         i32;
typedef long long   i64;

SL_STATIC_ASSERT(sizeof(i8) == 1);
SL_STATIC_ASSERT(sizeof(i16) == 2);
SL_STATIC_ASSERT(sizeof(i32) == 4);
SL_STATIC_ASSERT(sizeof(i64) == 8);

typedef float   real32;
typedef double  real64;

SL_STATIC_ASSERT(sizeof(real32) == 4);
SL_STATIC_ASSERT(sizeof(real64) == 8);


typedef i8  bool8;
typedef i16 bool16;
typedef i32 bool32;

#ifndef __cplusplus
    #ifndef bool
    #define bool bool32
    #define true    (1==1)
    #define false   (1==0)
    #endif
#endif

typedef struct read_file_result
{
    bool success;
    char* contents;
    size_t size;
} read_file_result;

read_file_result ReadEntireFile(char* Path, bool AsBinary)
{
    read_file_result Result = {0};

    FILE *f = NULL;
    if (AsBinary)
        f = fopen(Path, "rb");
    else 
        f = fopen(Path, "r");

    if (f)
    {
        fseek(f, 0, SEEK_END);
        long fsize = ftell(f);
        fseek(f, 0, SEEK_SET);  //same as rewind(f);

        Result.contents = (char*) malloc(fsize + 1);
        Result.size = fread(Result.contents, 1, fsize, f);
        fclose(f);

        Result.contents[fsize] = 0;
        Result.success = true;
    }

    return Result;
}

#if defined(__cplusplus)
}
#endif

#endif // _SL_H_