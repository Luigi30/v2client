#ifndef STDINT_H
#define STDINT_H

#ifdef __ALPHA
#include <ints.h>

typedef uint8   uint8_t;
typedef uint16  uint16_t;
typedef uint32  uint32_t;
typedef uint64  uint64_t;
typedef int8    int8_t;
typedef int16   int16_t;
typedef int32   int32_t;
typedef int64   int64_t;
#endif

#endif