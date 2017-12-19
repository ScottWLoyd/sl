#include <stdio.h>
#include <float.h>

#define SL_DEBUG
#define _SL_H_IMPLEMENTATION
#include "sl.h"

int main(int argc, char** argv)
{

   vec2f V = InvalidVec2f();
   char* s = Vec2fToString(V);
   printf("Invalid: %s\n", s);

   char* t = "{ 123, 456 }";
   V = ParseVec2f(t);
   sl_assert(V.X == 123.0f);
   sl_assert(V.Y == 456.0f);

   t = "987.654, 654.321";
   V = ParseVec2f(t);
   sl_assert(V.X == 987.654f);
   sl_assert(V.Y == 654.321f);

   PrintVec2f(V);
   printf("\n");

   V = Vec2f(1, 1);
   vec2f U = Vec2f(2, 3);

   vec2f W = V + U;
   sl_assert(W.X == 3.0f);
   sl_assert(W.Y == 4.0f);

   W = W - U;
   sl_assert(W == V);


   W = HadamardVec2f(V, U);
   sl_assert(W.X == V.X * U.X);
   sl_assert(W.Y == V.Y * U.Y);

   real32 dot = V*U;
   sl_assert(dot == 6.0f);

   W = dot * V;
   sl_assert(W.X == 6.0f);
   sl_assert(W.Y == 6.0f);

   W = PerpVec2f(V);
   sl_assert(W.X == -1.0f);
   sl_assert(W.Y == 1.0f);

   printf("Success\n");
   return 0;
}