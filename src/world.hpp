#pragma once

#include <savepoint.hpp>

#include <array>
#include <memory>

#include "level.hpp"

class MppRenderer;

class MppWorld
{
    enum Level
    {
        LevelSurface,
        LevelUnderground1,
        LevelUnderground2,
        LevelUnderground3,
        LevelCount,
    };

public:
    MppWorld();
    bool Init(Savepoint& savepoint, SavepointStatus status);
    void Quit();
    void Update(MppRenderer& renderer, int ticks);
    void SetLevel(int level);

private:
    std::array<std::shared_ptr<MppLevel>, LevelCount> Levels;
    int LevelIndex;
};