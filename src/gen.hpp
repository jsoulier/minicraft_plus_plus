#pragma once

#include "tile.hpp"

using MppGenFunction = MppTileID(*)(float x, float y, int level);

MppTileID MppGenSurface(float x, float y, int level);
MppTileID MppGenUnderground(float x, float y, int level);
