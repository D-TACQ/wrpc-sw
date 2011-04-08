#ifndef __TYPES_H
#define __TYPES_H

#include <inttypes.h> 

struct hw_timestamp {
  int ahead;
  uint32_t utc;
  uint32_t nsec;
  uint32_t phase;
};

#endif