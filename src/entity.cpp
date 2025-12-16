#include <SDL3/SDL.h>
#include <savepoint.hpp>

#include "entity.hpp"
#include "level.hpp"
#include "physics.hpp"
#include "tile.hpp"

MppEntity::MppEntity()
    : X{0.0f}
    , Y{0.0f}
{
}

void MppEntity::Visit(SavepointVisitor& visitor)
{
    visitor(X);
    visitor(Y);
}

void MppEntity::SetX(float x)
{
    X = x;
}

void MppEntity::SetY(float y)
{
    Y = y;
}

float MppEntity::GetX() const
{
    return X;
}

float MppEntity::GetY() const
{
    return Y;
}

void MppEntity::Move(MppLevel& level, float dx, float dy)
{
    float x = X + dx;
    float y = Y + dy;
    float size = GetSize();
    int tileX1 = x / MppTile::kWidth;
    int tileY1 = y / MppTile::kWidth;
    int tileX2 = (x + size) / MppTile::kWidth;
    int tileY2 = (y + size) / MppTile::kWidth;
    for (int tileX = tileX1; tileX <= tileX2; tileX++)
    for (int tileY = tileY1; tileY <= tileY2; tileY++)
    {
        const MppTile& tile = level.GetTile(tileX, tileY);
        if (!(tile.GetPhysicsType() & MppPhysicsTypeWallTile))
        {
            continue;
        }
        float tx = tileX * MppTile::kWidth;
        float ty = tileY * MppTile::kWidth;
        if (MppPhysicsTest(x, y, size, tx, ty, MppTile::kWidth))
        {
            return;
        }
    }
    X = x;
    Y = y;
}