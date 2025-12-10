#include "surface_level.hpp"

MppSurfaceLevel::MppSurfaceLevel()
    : MppLevel()
{
    Noise.SetFrequency(0.01f);
}

MppTileType MppSurfaceLevel::Generate(int x, int y) const
{
    return MppTileTypeGrass;
}