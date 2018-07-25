#ifndef __CU_TIME_H_
#define __CU_TIME_H_

#include <sys/time.h>
#include <time.h>

#include "Platform.h"

/**
 * Lightweight timer for measuring elapsed time. Specially designed
 * for benchmarking, and using the 'CLOCK_MONOTONIC'.
 *
 * Usage is trivial:
 *
 *      Timer Tm = NewTimerStatic();
 *      usleep(123200);
 *      uint64_t totalTime = ElapsedTimer(&Tm); // Will contain ~123200
 */
typedef struct Timer
{
    struct timespec InternalTime;
} Timer;

/**
 * Creates a new timer and initializes it with the current time
 */
Timer NewTimerStatic();

/**
 * Resets the timer with the current time
 */
void ResetTimer(Timer *);

/**
 * How many microseconds have elapsed since the last 'ResetTimer' or 'NewTimerStatic'
 */
uint64_t ElapsedTimer(Timer *);

/**
 * Converts the internal structure of a 'Timer' to microsecond time
 */
INLINE extern uint64_t TimerToMicro(Timer *Timer) FORCEINLINE;

/**
 * Transforms microseconds to seconds
 */
INLINE extern double MicroToSecond(uint64_t Micro) FORCEINLINE;

/**
 * Transforms a microseconds to milliseconds
 */
INLINE extern double MicroToMilli(uint64_t Micro) FORCEINLINE;

#endif /* __CU_TIME_H_ */
