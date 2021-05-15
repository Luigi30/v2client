#pragma once

#include "glide.h"

typedef unsigned int uint32_t;

extern FxU32 texMinAddress;
extern FxU32 texMaxAddress;

#ifdef _MSC_VER
#define M_PI 3.14159265358979323846264338327950288
#include <windows.h>
#endif

#ifndef _MSC_VER
typedef struct tagPOINT {
  uint32_t x;
  uint32_t y;
} POINT, *PPOINT, *NPPOINT, *LPPOINT;
#endif