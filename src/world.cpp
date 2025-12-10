#include <SDL3/SDL.h>
#include <savepoint.hpp>

#include <memory>

#include "level.hpp"
#include "surface_level.hpp"
#include "underground_level.hpp"
#include "version.hpp"
#include "world.hpp"

MppWorld::MppWorld()
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
            Levels[level]->Load(savepoint, level);
        }
        break;
    case SavepointStatus::New:
        for (int level = 0; level < LevelCount; level++)
        {
            Levels[level]->Generate();
        }
        break;
    }
    return true;
}

void MppWorld::Quit()
{
}

void MppWorld::Update(MppRenderer& renderer, float dt, float ticks)
{
}