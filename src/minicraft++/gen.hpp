#pragma once

#include <minicraft++/tile.hpp>

using MppGenFunction = MppTile(*)(float x, float y, int level);

MppTile MppGenSurface(float x, float y, int level);
MppTile MppGenUnderground(float x, float y, int level);
