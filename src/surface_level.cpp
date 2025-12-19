#include <memory>

#include "level.hpp"
#include "player_entity.hpp"
#include "surface_level.hpp"
#include "tile.hpp"
#include "world.hpp"

MppSurfaceLevel::MppSurfaceLevel()
    : MppLevel()
{
    Noise.SetFrequency(0.1f);
}

void MppSurfaceLevel::Generate(MppWorld& world, int level)
{
    MppLevel::Generate(world, level);
    std::shared_ptr<MppPlayerEntity> player = std::make_shared<MppPlayerEntity>();
    player->SetX(MppLevel::kWidth * MppTile::kWidth / 2);
    player->SetY(MppLevel::kWidth * MppTile::kWidth / 2);
    AddEntity(player);
    world.SetLevel(level);
}

MppTileID MppSurfaceLevel::Generate(float x, float y) const
{
    float noise = Noise.GetNoise(x, y);
    if (noise > 0.6)
    {
        return MppTileIDTree;
    }
    else if (noise > 0.2)
    {
        return MppTileIDGrass;
    }
    else if (noise > -0.2)
    {
        return MppTileIDDirt;
    }
    else if (noise > -0.6)
    {
        return MppTileIDStone;
    }
    else
    {
        return MppTileIDSand;
    }
}