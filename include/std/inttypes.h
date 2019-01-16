#ifndef __WRAPPED_INTTYPES_H
#define __WRAPPED_INTTYPES_H

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
#ifndef _UINT32_T_DECLARED
typedef unsigned int uint32_t;
#define _UINT32_T_DECLARED
#endif
typedef unsigned long long uint64_t;

typedef signed char int8_t;
typedef signed short int16_t;
#ifndef _INT32_T_DECLARED
typedef signed int int32_t;
#define _INT32_T_DECLARED
#endif
typedef signed long long int64_t;

#ifndef _INTPTR_T_DECLARED
typedef unsigned long intptr_t;
#define _INTPTR_T_DECLARED
#endif

#define UINT32_MAX 4294967295U

#endif
