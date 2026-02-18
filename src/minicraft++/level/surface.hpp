#pragma once

#include <FastNoiseLite.h>
#include <savepoint/savepoint.hpp>

#include <minicraft++/level/level.hpp>

#include <minicraft++/tile.hpp>

class MppSurfaceLevel : public MppLevel
{
    SAVEPOINT_POLY(MppSurfaceLevel)

public:
    MppSurfaceLevel();
    void Generate() override;
    int GetLightColor() const override;

protected:
    MppTile GenerateTile(float x, float y) const override;

private:
    FastNoiseLite Noise;
};