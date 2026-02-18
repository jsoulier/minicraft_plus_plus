#include <FastNoiseLite.h>
#include <savepoint/savepoint.hpp>

#include <memory>

#include <minicraft++/entity/mob/player.hpp>
#include <minicraft++/level/level.hpp>
#include <minicraft++/level/surface.hpp>
#include <minicraft++/tile.hpp>

MppSurfaceLevel::MppSurfaceLevel()
{
    Noise.SetFrequency(0.1f);
}

void MppSurfaceLevel::Generate() 
{
    MppLevel::Generate();
    std::shared_ptr<MppEntity> player = MppEntity::Create<MppPlayerEntity>();
    AddEntity(player);
}

int MppSurfaceLevel::GetLightColor() const
{
    // TODO: time of day
    return 444;
}

MppTile MppSurfaceLevel::GenerateTile(float x, float y) const
{
    float noise = Noise.GetNoise(x, y);
    if (noise > 0.6f)
    {
        return MppTile{MppTileIDGrass, MppTileIDTree};
    }
    else if (noise > 0.2f)
    {
        return MppTile{MppTileIDGrass};
    }
    else if (noise > -0.2f)
    {
        return MppTile{MppTileIDDirt};
    }
    else if (noise > -0.6f)
    {
        return MppTile{MppTileIDDirt, MppTileIDStoneWall};
    }
    else
    {
        return MppTile{MppTileIDSand};
    }
}
