#pragma once

#include <FastNoiseLite.h>
#include <savepoint/savepoint.hpp>

#include <minicraft++/level/level.hpp>

#include <minicraft++/tile.hpp>

class MppUndergroundLevel : public MppLevel
{
    SAVEPOINT_POLY(MppUndergroundLevel)

public:
    MppUndergroundLevel();
    int GetLightColor() const override;

protected:
    MppTile GenerateTile(float x, float y) const override;

private:
    FastNoiseLite Noise;
};
