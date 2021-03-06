// Numbers.h (c) 2019-2022 Jules Bloomenthal

#ifndef NUMBERS_HDR
#define NUMBERTS_HDR

#include "VecMat.h"

void Number(vec3 p, mat4 m, unsigned int n, vec3 color = vec3(0, 0, 0), float ptSize = 10);
    // m is full camera view

void Number(int x, int y, unsigned int n, vec3 color = vec3(0, 0, 0), float ptSize = 10);
    // x and y in pixels

char *Nice(float f);

#endif
