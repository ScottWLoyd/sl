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
#include <math.h>
#include <string.h>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Macros
//

#ifdef SL_DEBUG
#define sl_assert(condition) if (!(condition)) { fprintf(stderr, "%s(%d): Assertion failed.", __FILE__, __LINE__); *(int*)0 = 0; }
#else
#define sl_assert(condition)
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

#if defined(_WIN64) || defined(_WIN32)
#ifndef _CRT_SECURE_NO_WARNINGS
	#define _CRT_SECURE_NO_WARNINGS
#endif
#endif

#define cast(TYPE) (TYPE)


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
#define MetersToFeet(X) ((X) * 3.28084)
#define FeetToMeters(X) ((X) * 0.3048)

real32
Clamp(real32 Value, real32 Min, real32 Max);

real32
Clamp01(real32 Value);

real32
Abs(real32 Value);


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

static read_file_result ReadEntireFile(char* Path, bool AsBinary = false);

char*
CatStrings(char* A, char* B);

void
StringCopy(char* Dest, char* Src, i32 Count);

int
is_alpha(char c);

int
is_num(char c);

int
is_alnum(char c);


int
is_space(char c);

int
sl_atof(char* s, float* v);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// INI File Parser
//

typedef void(*sl_ini_handler)(char* Section, char* Param, char* Value, void* UserData);

i32 ParseIniFile(char* FilePath, sl_ini_handler Handler, void* UserData);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Spacial Types & Math
//

typedef struct vec2f
{
    struct
    {
        real32 X, Y;
    };
    struct
    {
        real32 W, H;
    };
    real32 E[2];
} vec2f;

vec2f Vec2f(real32 X, real32 Y);

vec2f
InvalidVec2f();

vec2f
ParseVec2f(char* s);

char*
Vec2fToString(vec2f V);

void
PrintVec2f(vec2f V);

vec2f AddVec2f(vec2f A, vec2f B);
vec2f SubVec2f(vec2f A, vec2f B);
vec2f ScaleVec2f(real32 A, vec2f B);
vec2f HadamardVec2f(vec2f A, vec2f B);
real32 InnerVec2f(vec2f A, vec2f B);
vec2f PerpVec2f(vec2f A);

#if defined(__cplusplus)
}
#endif
vec2f operator+(const vec2f& A, const vec2f& B);
vec2f operator-(const vec2f& A, const vec2f& B);
real32 operator*(const vec2f& A, const vec2f& B);
vec2f operator*(const real32& A, const vec2f& B);
bool operator==(const vec2f& A, const vec2f& B);
#if defined(__cplusplus)
extern "C" {
#endif

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

vec3f
InvalidVec3f();

vec3f
ParseVec3f(char* s);

char*
Vec3fToString(vec3f V);

void
PrintVec3f(vec3f V);


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

mat4f
Mat4Identity();

vec4f
Mul(mat4f M, vec4f V);

mat4f
TranslateMat4fByVec4f(mat4f M, vec4f V);

mat4f
TranslateMat4fByVec3f(mat4f M, vec3f V);

mat4f
MakeRotationMat4f(vec3f V);

#if 0
// TODO: implement this
mat4f
RotateMat4fByVec3f(mat4f M, vec3f V)
{
	mat4f Result;

	mat4f Rotation = MakeRotationMat4f(V);
	Result = MulMat4f(Rotation, M);

	return Result;
}
#endif

char*
Vec4fToString(vec4f V);

void
PrintVec4f(vec4f V);

#if defined(__cplusplus)
}
#endif

#endif // _SL_H_

//
// Implementation
//

#if defined(_SL_H_IMPLEMENTATION) && !defined(_SL_H_IMPLEMENTATION_DONE)
#define _SL_H_IMPLEMENTATION_DONE


#if defined(__cplusplus)
extern "C" {
#endif

real32
Clamp(real32 Value, real32 Min, real32 Max)
{
	if (Value < Min)
	{
		return Min;
	}
	if (Value > Max)
	{
		return Max;
	}
	return Value;
}

real32
Clamp01(real32 Value)
{
	return Clamp(Value, 0.0f, 1.0f);
}

real32
Abs(real32 Value)
{
	if (Value < 0.0f)
		return -1.0f * Value;
	return Value;
}


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

        Result.contents = cast(char*) malloc(fsize + 2);
        Result.size = fread(Result.contents, 1, fsize, f);
        fclose(f);

		Result.contents[Result.size] = 0;
        Result.contents[Result.size + 1] = EOF;
        Result.success = true;
    }

    return Result;
}

char* 
CatStrings(char* A, char* B)
{
    int LenA = strlen(A);
    int LenB = strlen(B);
    char* Result = cast(char*)malloc(LenA + LenB + 1);
    snprintf(Result, LenA + LenB + 1, "%s%s", A, B);
    return Result;
}

void
StringCopy(char* Dest, char* Src, i32 Count)
{
    for (; Count > 0; Count--)
    {
        *Dest++ = *Src++;
    }
    *Dest = 0;
}

int
is_alpha(char c)
{
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

int
is_num(char c)
{
    return (c >= '0' && c <= '9');
}

int
is_num_with_base(char c, int base)
{
	if (base == 10)
	{
		return is_num(c);
	}
	if (base == 16)
	{
		return is_num(c) || ((c >= 'a') && (c <= 'f')) || ((c >= 'a') && (c <= 'f'));
	}
	if (base == 2)
	{
		return c == '0' || c == '1';
	}
	return 0;
}

int
is_alnum(char c)
{
    return is_alpha(c) || is_num(c);
}


int
is_space(char c)
{
    return (c == ' ' || c == '\t' || c == '\r' || c == '\n');
}

int
sl_atof(char* s, float* v)
{
    if (!s || strlen(s) == 0)
        return 0;

    while (is_space(*s))
        s++;

    int sign = 1;
    if (*s == '-')
    {
        sign = -1;
        s++;
    }
    else if (*s == '+')
    {
        s++;
    }

    while (is_space(*s))
    {
        s++;
    }

    if (!is_num(*s))
        return 0;

    *v = (float)(*s - '0');

    s++;
    while (is_num(*s))
    {
        *v = (*v * 10) + (*s - '0');
        s++;
    }

    if (*s == '.')
    {
        s++;
        int i=1;
        while(is_num(*s))
        {
            *v += (float)(*s - '0') * (float)pow(10, -i);
            s++;
            i++;
        }
    }

    *v = sign * (*v);

    return 1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// INI File Parser
//

typedef void (*sl_ini_handler)(char* Section, char* Param, char* Value, void* UserData);

internal char*
sl_get_line(char* s)
{
    static char* Start;
    char* End;

    if (s)
    {
        Start = s;
    }
    else if (Start)
    {
        s = Start;
    } 
    else
    {
        return 0;
    }

    while(*s != EOF && *s != '\n')
    {
        s++;
    }

    End = s;

    i32 len = End - Start;
    char* Result = cast(char*)malloc(len + 1);
    StringCopy(Result, Start, len);

    if (*s == EOF)
    {
        Start = 0;
        Result[len-1] = EOF;
    }
    else
    {
        Start = End + 1; // skip the newline
    }

    return Result;
}

internal char*
sl_find_next_char(char* s, char c)
{
    while(s && *s && *s != c)
        s++;
    if (!*s || *s == c)
        return s;
    return 0;
}

internal void
sl_trim_whitespace(char* s)
{
    i32 len = strlen(s);
    while (len && is_space(s[len-1]))
        s[--len] = 0;
}

i32 ParseIniFile(char* FilePath, sl_ini_handler Handler, void* UserData)
{
    i32 LineNumber;

    read_file_result ReadFile = ReadEntireFile(FilePath);

    if (!ReadFile.success)
    {
        return -1;
    }

    char Section[256];
    char Key[256];
    char Value[256];

    Section[0] = 0;

    char* line = sl_get_line(ReadFile.contents);
    LineNumber = 0;
    do {
        char* str = line;

        if (!str)
        {
            break;
        }
        else if (strlen(str) == 0)
        {
            // empty line
            continue;
        }

        if (str[0] == '[')
        {
            char* Start = ++str;
            char* End = sl_find_next_char(str, ']');
            i32 len = End - Start;
            StringCopy(Section, Start, len);
        }
        else
        {
            // skip any leading whitespace
            while (is_space(*str)) 
                str++;

            // skip comment lines
            if (*str == ';' || *str == '#')
            {
                free(line);
                LineNumber++;
                continue;
            }

            char* Start = str;
            char* End = sl_find_next_char(str, '=');

			if (Start == End)
			{
				// Empty line
				free(line);
				LineNumber++;
				continue;
			}

            StringCopy(Key, Start, End - Start);
            sl_trim_whitespace(Key);

            str = ++End;
            while (is_space(*str))
                str++;

            Start = str;
            // find the end of the line and ignore any trailing comments
            while (*str && *str != ';' && *str != '#')
                str++;

            End = str;
            StringCopy(Value, Start, End - Start);
            sl_trim_whitespace(Value);

            Handler(Section, Key, Value, UserData);
        }

        free(line);
        LineNumber++;
    } while ((line = sl_get_line(0)));

    return 0;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Spacial Types & Math
//


//
// Vec2f
//

vec2f
Vec2f(real32 X, real32 Y)
{
    vec2f Result;
    Result.X = X;
    Result.Y = Y;
    return Result;
}

vec2f
InvalidVec2f()
{
    vec2f Result;
    Result.X = cast(real32)0xffffffff;
    Result.Y = cast(real32)0xffffffff;
    return Result;
}

vec2f
ParseVec2f(char* s)
{
    // Expected formats:
    // { X, Y }
    // X, Y
    vec2f Result = {0};

    while (is_space(*s))
        s++;

    if (*s == '{') 
        s++;

    while (is_space(*s))
        s++;    

    char* Start = s;
    char* End = sl_find_next_char(s, ',');

    char Value[64];
    StringCopy(Value, Start, End - Start);
    sl_trim_whitespace(Value);
    if (!sl_atof(Value, &Result.X))
        return InvalidVec2f();

    Start = s = End + 1; // skip the comma
    End = sl_find_next_char(Start, '}');
    StringCopy(Value, Start, End - Start);
    sl_trim_whitespace(Value);
    if (!sl_atof(Value, &Result.Y))
        return InvalidVec2f();

    return Result;
}

char*
Vec2fToString(vec2f V)
{
    int Length = snprintf(0, 0, "%f, %f", V.X, V.Y);
    char* Result = cast(char*)malloc(Length);
    
    snprintf(Result, Length, "%f, %f", V.X, V.Y);

    return Result;
}

void
PrintVec2f(vec2f V)
{
    char* FormattedString = Vec2fToString(V);
    printf("{ %s }", FormattedString);
    free(FormattedString);
}

vec2f AddVec2f(vec2f A, vec2f B)
{
    vec2f Result;
    Result.X = A.X + B.X;
    Result.Y = A.Y + B.Y;
    return Result;
}

vec2f SubVec2f(vec2f A, vec2f B)
{
    vec2f Result;
    Result.X = A.X - B.X;
    Result.Y = A.Y - B.Y;
    return Result;
}

vec2f HadamardVec2f(vec2f A, vec2f B)
{
    vec2f Result;
    Result.X = A.X * B.X;
    Result.Y = A.Y * B.Y;
    return Result;
}

real32 InnerVec2f(vec2f A, vec2f B)
{
    real32 Result;
    Result = A.X * B.X + A.Y + B.Y;
    return Result;
}

vec2f ScaleVec2f(real32 A, vec2f B)
{
    vec2f Result;
    Result.X = A * B.X;
    Result.Y = A * B.Y;
    return Result;
}

vec2f PerpVec2f(vec2f A)
{
    vec2f Result;
    Result.X = -A.Y;
    Result.Y = A.X;
    return Result;
}

int EqualsVec2f(vec2f A, vec2f B)
{
    return (A.X == B.X) && (A.Y == B.Y);
}

#if defined(__cplusplus)
}
#endif
vec2f operator+(const vec2f& A, const vec2f& B) { return AddVec2f(A, B); }
vec2f operator-(const vec2f& A, const vec2f& B) { return SubVec2f(A, B); }
real32 operator*(const vec2f& A, const vec2f& B) { return InnerVec2f(A, B); }
vec2f operator*(const real32& A, const vec2f& B) { return ScaleVec2f(A, B); }
bool operator==(const vec2f& A, const vec2f& B) { return cast(bool)EqualsVec2f(A, B); }
#if defined(__cplusplus)
extern "C" {
#endif

//
// Vec3f
//

vec3f
InvalidVec3f()
{
    vec3f Result;
    Result.X = cast(real32)0xffffffff;
    Result.Y = cast(real32)0xffffffff;
    Result.Z = cast(real32)0xffffffff;
    return Result;
}

vec3f
ParseVec3f(char* s)
{
    // Expected formats:
    // { X, Y, Z }
    // X, Y, Z
    vec3f Result = {0};

    while (is_space(*s))
        s++;

    if (*s == '{') 
        s++;

    while (is_space(*s))
        s++;    

    char* Start = s;
    char* End = sl_find_next_char(s, ',');

    char Value[64];
    StringCopy(Value, Start, End - Start);
    sl_trim_whitespace(Value);
    if (!sl_atof(Value, &Result.X))
        return InvalidVec3f();

    Start = s = End + 1; // skip the comma
    End = sl_find_next_char(Start, ',');
    StringCopy(Value, Start, End - Start);
    sl_trim_whitespace(Value);
    if (!sl_atof(Value, &Result.Y))
        return InvalidVec3f();

    Start = s = End + 1; // skip the comma
    End = sl_find_next_char(Start, '}');
    StringCopy(Value, Start, End - Start);
    sl_trim_whitespace(Value);
    if (!sl_atof(Value, &Result.Z))
        return InvalidVec3f();

    return Result;
}

char*
Vec3fToString(vec3f V)
{
    int Length = snprintf(0, 0, "%f, %f, %f", V.X, V.Y, V.Z);
    char* Result = cast(char*)malloc(Length);
    
    snprintf(Result, Length, "%f, %f, %f", V.X, V.Y, V.Z);

    return Result;
}

void
PrintVec3f(vec3f V)
{
    char* FormattedString = Vec3fToString(V);
    printf("{ %s }", FormattedString);
    free(FormattedString);
}



mat4f 
Mat4Identity()
{
    mat4 Result;

    Result.col[0] = {1.f, 0.f, 0.f, 0.f };
    Result.col[1] = {0.f, 1.f, 0.f, 0.f };
    Result.col[2] = {0.f, 0.f, 1.f, 0.f };
    Result.col[3] = {0.f, 0.f, 0.f, 1.f };

    return Result;
}

vec4f
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
    
mat4f
TranslateMat4fByVec4f(mat4f M, vec4f V)
{
	mat4f Result = M;

	Result.col[3].X += V.X;
	Result.col[3].Y += V.Y;
	Result.col[3].Z += V.Z;
	//Result.col[3].W += V.W;

	return Result;
}

mat4f
TranslateMat4fByVec3f(mat4f M, vec3f V)
{
    mat4f Result = M;

	vec4f V4 = { V.X, V.Y, V.Z, 1.f };

	Result = TranslateMat4fByVec4f(M, V4);

    return Result;
}

mat4f
MakeRotationMat4f(vec3f V)
{
    mat4f Result;

    real32 A = (real32)cos(V.X);
    real32 B = (real32)sin(V.X);
    real32 C = (real32)cos(V.Y);
    real32 D = (real32)sin(V.Y);
    real32 E = (real32)cos(V.Z);
    real32 F = (real32)sin(V.Z);

    real32 AD = A * D;
    real32 BD = B * D;

    Result.E[0] = C * E;
    Result.E[1] = -C * F;
    Result.E[2] = D;
    Result.E[4] = BD * E + A * F;
    Result.E[5] = -BD * F + A * E;
    Result.E[6] = -B * C;
    Result.E[8] = -AD * E + B * F;
    Result.E[9] = AD * F + B * E;
    Result.E[10] = A * C;

    Result.E[3] = Result.E[7] = Result.E[11] = Result.E[12] = Result.E[13] = Result.E[14] = 0.f;
    Result.E[15] = 1.f;

    return Result;
}

#if 0
// TODO: implement this
mat4f
RotateMat4fByVec3f(mat4f M, vec3f V)
{
    mat4f Result;

    mat4f Rotation = MakeRotationMat4f(V);
    Result = MulMat4f(Rotation, M);

    return Result;
}
#endif

char*
Vec4fToString(vec4f V)
{
    int Length = snprintf(0, 0, "%f, %f, %f, %f", V.X, V.Y, V.Z, V.W);
    char* Result = (char*)malloc(Length);
    
    snprintf(Result, Length, "%f, %f, %f, %f", V.X, V.Y, V.Z, V.W);

    return Result;
}

void
PrintVec4f(vec4f V)
{
    char* FormattedString = Vec4fToString(V);
    printf("{ %s }", FormattedString);
    free(FormattedString);
}

#if defined(__cplusplus)
}
#endif

#endif // _SL_H_IMPLEMENTATION

