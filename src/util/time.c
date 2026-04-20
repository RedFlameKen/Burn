#ifndef TIME_C
#define TIME_C

#define NANOS_PER_SECOND  1000000000.0f
#define NANOS_PER_MILLIS  1000000.0f
#define MILLIS_PER_SECOND 1000.0f

unsigned long current_time_nanos(void);
unsigned long current_time_millis(void);

#ifdef TIME_IMPLEMENTATION
#define _XOPEN_SOURCE 500
#include <time.h>
#include <stdint.h>

unsigned long current_time_nanos(void){
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return NANOS_PER_SECOND * ts.tv_sec + ts.tv_nsec;
}

unsigned long current_time_millis(void){
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return MILLIS_PER_SECOND * ts.tv_sec + (ts.tv_nsec / NANOS_PER_MILLIS);
}

#endif

#endif
