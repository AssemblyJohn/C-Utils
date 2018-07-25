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

uint64_t TimerToMicro(Timer *Tm)
{
    return (Tm->InternalTime.tv_sec * 1000000) + (Tm->InternalTime.tv_nsec / 1000);
}

double MicroToSecond(uint64_t Micro)
{
    return (Micro / 1000000.0);
}

double MicroToMilli(uint64_t Micro)
{
    return (Micro / 1000.0);
}

