#include <FastNoiseLite.h>
#include <savepoint/savepoint.hpp>

#include <minicraft++/entity/mob/player.hpp>
#include <minicraft++/level/level.hpp>
#include <minicraft++/level/underground.hpp>
#include <minicraft++/tile.hpp>

MppUndergroundLevel::MppUndergroundLevel()
{
    Noise.SetFrequency(0.1f);
}

int MppUndergroundLevel::GetLightColor() const
{
    return 0;
}

MppTile MppUndergroundLevel::GenerateTile(float x, float y) const
{
    float noise = Noise.GetNoise(x, y);
    if (noise > 0.0f)
    {
        return MppTile{MppTileIDStone, MppTileIDStoneWall};
    }
    else
    {
        return MppTile{MppTileIDStone};
    }
}
