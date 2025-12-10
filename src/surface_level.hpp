#pragma once

#include <FastNoiseLite.h>

#include "level.hpp"

class MppSurfaceLevel : public MppLevel
{
public:
    MppSurfaceLevel();
    MppTileType Generate(int x, int y) const override;

private:
    FastNoiseLite Noise;
};