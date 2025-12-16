#include <SDL3/SDL.h>
#include <savepoint.hpp>

#include "color.hpp"
#include "console.hpp"
#include "entity.hpp"
#include "level.hpp"
#include "physics.hpp"
#include "renderer.hpp"
#include "tile.hpp"

MppEntity::MppEntity()
    : X{0.0f}
    , Y{0.0f}
{
}

void MppEntity::Update(MppLevel& level, MppRenderer& renderer, float dt, float ticks)
{
    if (!MppConsoleDebugPhysics)
    {
        return;
    }
    float x = GetPhysicsX();
    float y = GetPhysicsY();
    float width = GetPhysicsWidth();
    float height = GetPhysicsHeight();
    renderer.DrawRect(kMppColorEntityPhysics, x, y, width, height, MppRenderer::LayerPhysics);
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

float MppEntity::GetPhysicsX() const
{
    return X + GetPhysicsOffsetX();
}

float MppEntity::GetPhysicsY() const
{
    return Y + GetPhysicsOffsetY();
}

void MppEntity::Move(MppLevel& level, float dx, float dy)
{
    float x = X;
    float y = Y;
    X += dx;
    Y += dy;
    if (!MoveTest(level))
    {
        X = x;
        Y = y;
    }
}

bool MppEntity::MoveTest(MppLevel& level)
{
    float size = GetSize();
    float x = GetPhysicsX();
    float y = GetPhysicsY();
    float width = GetPhysicsWidth();
    float height = GetPhysicsHeight();
    int tileX1 = x / MppTile::kWidth;
    int tileY1 = y / MppTile::kWidth;
    int tileX2 = (x + width) / MppTile::kWidth;
    int tileY2 = (y + height) / MppTile::kWidth;
    for (int tileX = tileX1; tileX <= tileX2; tileX++)
    for (int tileY = tileY1; tileY <= tileY2; tileY++)
    {
        const MppTile& tile = level.GetTile(tileX, tileY);
        if (!(tile.GetPhysicsType() & MppPhysicsTypeWallTile))
        {
            continue;
        }
        int tx = tile.GetPhysicsX(tileX);
        int ty = tile.GetPhysicsY(tileY);
        int tileWidth = tile.GetPhysicsWidth();
        int tileHeight = tile.GetPhysicsHeight();
        if (MppPhysicsTest(x, y, width, height, tx, ty, tileWidth, tileHeight))
        {
            return false;
        }
    }
    return true;
}
