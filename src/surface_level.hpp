#pragma once

#include "level.hpp"

class MppSurfaceLevel : public MppLevel
{
public:
    MppTileType Generate(int x, int y) const override;
};