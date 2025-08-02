#include <SDL3/SDL.h>

#include <cstdint>

#include "ticks.hpp"

MppTicks::MppTicks(uint64_t savedTime)
    : SavedTime{savedTime}
{
    StartTime = SDL_GetTicksNS();
    Tick();
}

void MppTicks::Tick()
{
    Time2 = SDL_GetTicksNS();
    DeltaTime = Time2 - Time1;
    Time1 = Time2;
    CurrentTime = SavedTime + Time2 - StartTime;
}

uint64_t MppTicks::GetCurrentTime() const
{
    return CurrentTime;
}

uint64_t MppTicks::GetDeltaTime() const
{
    return DeltaTime;
}