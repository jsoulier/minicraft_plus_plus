#pragma once

#include "level.hpp"

class MppUndergroundLevel : public MppLevel
{
public:
    MppTileType Generate(float x, float y) const override;
};
