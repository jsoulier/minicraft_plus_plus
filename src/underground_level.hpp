#pragma once

#include "level.hpp"

class MppUndergroundLevel : public MppLevel
{
public:
    MppTileID Generate(float x, float y) const override;
};
