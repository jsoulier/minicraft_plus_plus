#include <FastNoiseLite.h>

#include <minicraft++/tile.hpp>
#include <minicraft++/gen.hpp>

struct Generator
{
    Generator()
    {
        Noise.SetFrequency(0.1f);
    }

    FastNoiseLite Noise;
};

static Generator generator;

MppTile MppGenSurface(float x, float y, int level)
{
    float noise = generator.Noise.GetNoise(x, y);
    if (noise > 0.6)
    {
        return MppTile{MppTileIDGrass, MppTileIDTree};
    }
    else if (noise > 0.2)
    {
        return MppTile{MppTileIDGrass};
    }
    else if (noise > -0.2)
    {
        return MppTile{MppTileIDDirt};
    }
    else if (noise > -0.6)
    {
        return MppTile{MppTileIDDirt, MppTileIDStoneWall};
    }
    else
    {
        return MppTile{MppTileIDSand};
    }
}

MppTile MppGenUnderground(float x, float y, int level)
{
    return MppTile{MppTileIDDirt, MppTileIDStoneWall};
}
