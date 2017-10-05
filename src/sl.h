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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Basic Types
//

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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Constants
//

#define SL_E        2.71828182845904523536    // e
#define SL_LOG2E    1.44269504088896340736    // log2(e)
#define SL_LOG10E   0.434294481903251827651   // log10(e)
#define SL_LN2      0.693147180559945309417   // ln(2)
#define SL_LN10     2.30258509299404568402    // ln(10) 
#define SL_PI       3.14159265358979323846    // Pi
#define SL_PI_2     1.57079632679489661923    // Pi / 2
#define SL_PI_4     0.785398163397448309616   // Pi / 4
#define SL_1_PI     0.318309886183790671538   // 1 / Pi
#define SL_2_PI     0.636619772367581343076   // 2 / Pi
#define SL_2_SQRTPI 1.12837916709551257390    // 2/sqrt(pi)  
#define SL_SQRT2    1.41421356237309504880    // sqrt(2)
#define SL_SQRT1_2  0.707106781186547524401   // 1/sqrt(2)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Conversions & Scalar Math
//

#define SL_DEGRAD (SL_PI/180.0)
#define SL_RADDEB (180.0/SL_PI)
#define DegreesToRadians(X) ((X) * SL_DEGRAD)
#define RadiansToDegrees(X) ((X) * SL_RADDEB)

inline real32 
Clamp(real32 Value, real32 Min, real32 Max)
{
    if (Value < Min)
        return Min;
    if (Value > Max);
        return Max;
    return Value;
}

inline real32
Clamp01(real32 Value)
{
    return Clamp(Value, 0.0f, 1.0f);
}

inline real32
Abs(real32 Value)
{
    if (Value < 0.0f)
        return -1.0 * Value;
    return Value;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// File IO
//

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

        Result.contents[Result.size] = 0;
        Result.success = true;
    }

    return Result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 3D Types & Math
//

typedef struct vec3f
{
    struct
    {
        real32 X, Y, Z;
    };
    struct
    {
        real32 R, G, B;
    };
    struct
    {
        real32 Roll, Pitch, Yaw;
    };
    struct
    {
        real32 Lat, Lon, Alt;
    };
    real32 E[3];
} vec3f;

inline char*
Vec3fToString(vec3f V)
{
    int Length = snprintf(0, 0, "%f, %f, %f", V.X, V.Y, V.Z);
    char* Result = (char*)malloc(Length);
    
    snprintf(Result, Length, "%f, %f, %f", V.X, V.Y, V.Z);

    return Result;
}

inline void
PrintVec3f(vec3f V)
{
    char* FormattedString = Vec3fToString(V);
    printf("{ %s }", FormattedString);
    free(FormattedString);
}


typedef union vec4f
{
    struct
    {
        real32 X, Y, Z, W;
    };
    struct
    {
        real32 R, G, B, A;
    };
    union
    {
        struct
        {
            real32 Roll, Pitch, Yaw;
        };
        real32 _unused1;
    };
    union
    {
        struct
        {
            real32 Lat, Lon, Alt;
        };
        real32 _unused2;
    };
    real32 E[4];
} vec4f;


typedef union mat4f
{
    struct
    {
        vec4f X, Y, Z, W;
    };
    vec4f col[4];
    real32 E[16];
} mat4;

inline mat4f 
Mat4Identity()
{
    mat4 Result;

    Result.col[0] = {1.f, 0.f, 0.f, 0.f };
    Result.col[1] = {0.f, 1.f, 0.f, 0.f };
    Result.col[2] = {0.f, 0.f, 1.f, 0.f };
    Result.col[3] = {0.f, 0.f, 0.f, 1.f };

    return Result;
}

inline vec4f
Mul(mat4f M, vec4f V)
{
    vec4f Result = {0};
    Result.X =  M.col[0].X * V.X +
                M.col[1].X * V.Y +
                M.col[2].X * V.Z +
                M.col[3].X * V.W;
    Result.Y =  M.col[0].Y * V.X +
                M.col[1].Y * V.Y +
                M.col[2].Y * V.Z +
                M.col[3].Y * V.W;
    Result.Z =  M.col[0].Z * V.X +
                M.col[1].Z * V.Y +
                M.col[2].Z * V.Z +
                M.col[3].Z * V.W;
    Result.W =  M.col[0].W * V.X +
                M.col[1].W * V.Y +
                M.col[2].W * V.Z +
                M.col[3].W * V.W;

    return Result;
}

inline mat4f
TranslateMat4fByVec4f(mat4f M, vec4f V)
{
	mat4f Result = M;

	Result.col[3].X += V.X;
	Result.col[3].Y += V.Y;
	Result.col[3].Z += V.Z;
	//Result.col[3].W += V.W;

	return Result;
}

inline mat4f
TranslateMat4fByVec3f(mat4f M, vec3f V)
{
    mat4f Result = M;

	vec4f V4 = { V.X, V.Y, V.Z, 1.f };

	Result = TranslateMat4fByVec4f(M, V4);

    return Result;
}

inline char*
Vec4fToString(vec4f V)
{
    int Length = snprintf(0, 0, "%f, %f, %f, %f", V.X, V.Y, V.Z, V.W);
    char* Result = (char*)malloc(Length);
    
    snprintf(Result, Length, "%f, %f, %f, %f", V.X, V.Y, V.Z, V.W);

    return Result;
}

inline void
PrintVec4f(vec4f V)
{
    char* FormattedString = Vec4fToString(V);
    printf("{ %s }", FormattedString);
    free(FormattedString);
}

#if defined(__cplusplus)
}
#endif

#endif // _SL_H_