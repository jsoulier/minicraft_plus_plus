#pragma once

#include <FastNoiseLite.h>

#include "level.hpp"

class MppSurfaceLevel : public MppLevel
{
public:
    MppSurfaceLevel();
    void Generate(MppWorld& world, int level) override;
    MppTileID Generate(float x, float y) const override;

private:
    FastNoiseLite Noise;
};