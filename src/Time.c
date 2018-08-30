#include "Time.h"

Timer NewTimerStatic()
{
    Timer Timer;

    clock_gettime(CLOCK_MONOTONIC, &Timer.InternalTime);

    return Timer;
}

void ResetTimer(Timer *Timer)
{
    clock_gettime(CLOCK_MONOTONIC, &Timer->InternalTime);
}

uint64_t ElapsedTimer(Timer *Timer)
{
    uint64_t MicroTime = TimerToMicro(Timer);

    // Get the current time again
    clock_gettime(CLOCK_MONOTONIC, &Timer->InternalTime);

    // Microseconds
    return (TimerToMicro(Timer) - MicroTime);
}

