#pragma once

enum MppPhysicsType
{
    MppPhysicsTypeGroundTile = 0x01,
    MppPhysicsTypeWallTile   = 0x02,
    MppPhysicsTypeLiquidTile = 0x04,
    MppPhysicsTypeMobEntity  = 0x08,
};

bool MppPhysicsTest(float x1, float y1, int s1, float x2, float y2, int s2);
bool MppPhysicsTestTile(float x1, float y1, int s1, int x2, int y2);