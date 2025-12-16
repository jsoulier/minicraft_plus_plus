#pragma once

enum MppPhysicsType
{
    MppPhysicsTypeGroundTile = 0x01,
    MppPhysicsTypeWallTile   = 0x02,
    MppPhysicsTypeLiquidTile = 0x04,
    MppPhysicsTypeMobEntity  = 0x08,
};

bool MppPhysicsTest(float x1, float y1, int w1, int h1, float x2, float y2, int w2, int h2);