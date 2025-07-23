#pragma once

#include <cstdint>

class MppTime
{
public:
    MppTime(uint64_t savedTime);
    void Tick();
    uint64_t GetCurrentTime() const;
    uint64_t GetDeltaTime() const;

private:
    uint64_t time1;
    uint64_t time2;
    uint64_t savedTime;
    uint64_t startTime;
    uint64_t currentTime;
    uint64_t deltaTime;
};