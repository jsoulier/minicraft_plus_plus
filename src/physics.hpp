#pragma once

enum MppPhysicsType
{
    MppPhysicsTypeGroundTile = 1,
    MppPhysicsTypeWallTile = 2,
    MppPhysicsTypeLiquidTile = 4,
    MppPhysicsTypeMobEntity = 8,
    MppPhysicsTypeFurnitureEntity = 16,
    MppPhysicsTypeRecipeEntity = 32,
    MppPhysicsTypeItemEntity = 64,
};

bool MppPhysicsTest(float x1, float y1, int w1, int h1, float x2, float y2, int w2, int h2);