#include <SDL3/SDL.h>

#include <cstdint>

#include "time.hpp"

MppTime::MppTime(uint64_t savedTime)
    : SavedTime{savedTime}
{
    StartTime = SDL_GetTicksNS();
    Tick();
}

void MppTime::Tick()
{
    Time2 = SDL_GetTicksNS();
    DeltaTime = Time2 - Time1;
    Time1 = Time2;
    CurrentTime = SavedTime + Time2 - StartTime;
}

uint64_t MppTime::GetCurrentTime() const
{
    return CurrentTime;
}

uint64_t MppTime::GetDeltaTime() const
{
    return DeltaTime;
}