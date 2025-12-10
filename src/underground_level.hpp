#pragma once

#include "level.hpp"

class MppUndergroundLevel : public MppLevel
{
public:
    MppTileType Generate(int x, int y) const override;
};
