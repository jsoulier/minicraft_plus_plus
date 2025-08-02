#pragma once

#include <cstdint>

class MppTicks
{
public:
    MppTicks() = default;
    MppTicks(uint64_t savedTime);
    void Tick();
    uint64_t GetCurrentTime() const;
    uint64_t GetDeltaTime() const;

private:
    uint64_t Time1;
    uint64_t Time2;
    uint64_t SavedTime;
    uint64_t StartTime;
    uint64_t CurrentTime;
    uint64_t DeltaTime;
};