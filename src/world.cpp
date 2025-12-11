#include <SDL3/SDL.h>
#include <savepoint.hpp>

#include <memory>

#include "level.hpp"
#include "surface_level.hpp"
#include "underground_level.hpp"
#include "version.hpp"
#include "world.hpp"

MppWorld::MppWorld()
    : LevelIndex{-1}
{
}

bool MppWorld::Init(Savepoint& savepoint, SavepointStatus status)
{
    Levels[LevelSurface] = std::make_shared<MppSurfaceLevel>();
    Levels[LevelUnderground1] = std::make_shared<MppUndergroundLevel>();
    Levels[LevelUnderground2] = std::make_shared<MppUndergroundLevel>();
    Levels[LevelUnderground3] = std::make_shared<MppUndergroundLevel>();
    switch (status)
    {
    case SavepointStatus::Existing:
        for (int level = 0; level < LevelCount; level++)
        {
            Levels[level]->Load(*this, savepoint, level);
        }
        break;
    case SavepointStatus::New:
        for (int level = 0; level < LevelCount; level++)
        {
            Levels[level]->Generate(*this, level);
        }
        break;
    }
    if (LevelIndex == -1)
    {
        SDL_Log("Missing player");
        return false;
    }
    return true;
}

void MppWorld::Quit()
{
}

void MppWorld::Update(MppRenderer& renderer, float dt, float ticks)
{
    Levels[LevelIndex]->Update(*this, renderer, dt, ticks);
}

void MppWorld::SetLevel(int level)
{
    LevelIndex = level;
}