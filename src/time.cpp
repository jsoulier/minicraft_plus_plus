#include <SDL3/SDL.h>

#include <cstdint>

#include "time.hpp"

MppTime::MppTime(uint64_t savedTime)
    : savedTime{savedTime}
{
    startTime = SDL_GetTicksNS();
    Tick();
}

void MppTime::Tick()
{
    time2 = SDL_GetTicksNS();
    deltaTime = time2 - time1;
    time1 = time2;
    currentTime = savedTime + time2 - startTime;
}

uint64_t MppTime::GetCurrentTime() const
{
    return currentTime;
}

uint64_t MppTime::GetDeltaTime() const
{
    return deltaTime;
}