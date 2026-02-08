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

MppTileID MppGenSurface(float x, float y, int level)
{
    float noise = generator.Noise.GetNoise(x, y);
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
        return MppTileIDStoneWall;
    }
    else
    {
        return MppTileIDSand;
    }
}

MppTileID MppGenUnderground(float x, float y, int level)
{
    return MppTileIDStoneWall;
}
