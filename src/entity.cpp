#include <SDL3/SDL.h>
#include <savepoint/visitor.hpp>

#include "color.hpp"
#include "console.hpp"
#include "entity.hpp"
#include "level.hpp"
#include "physics.hpp"
#include "renderer.hpp"
#include "tile.hpp"

MppEntity::MppEntity()
    : X{0}
    , Y{0}
{
}

void MppEntity::Update(MppLevel& level, MppRenderer& renderer, int ticks)
{
    if (!MppConsoleDebugPhysics)
    {
        return;
    }
    int x = GetPhysicsX();
    int y = GetPhysicsY();
    int width = GetPhysicsWidth();
    int height = GetPhysicsHeight();
    renderer.DrawRect(kMppColorEntityPhysics, x, y, width, height, MppRenderer::LayerPhysics);
}

void MppEntity::Visit(SavepointVisitor& visitor)
{
    visitor(X);
    visitor(Y);
}

void MppEntity::SetX(int x)
{
    X = x;
}

void MppEntity::SetY(int y)
{
    Y = y;
}

int MppEntity::GetX() const
{
    return X;
}

int MppEntity::GetY() const
{
    return Y;
}

int MppEntity::GetPhysicsX() const
{
    return X + GetPhysicsOffsetX();
}

int MppEntity::GetPhysicsY() const
{
    return Y + GetPhysicsOffsetY();
}

void MppEntity::Move(MppLevel& level, int dx, int dy)
{
    MoveAxis(level, dx, 0);
    MoveAxis(level, 0, dy);
}

void MppEntity::MoveAxis(MppLevel& level, int dx, int dy)
{
    int x = X;
    int y = Y;
    X += dx;
    Y += dy;
    if (!MoveAxis(level))
    {
        X = x;
        Y = y;
    }
}

bool MppEntity::MoveAxis(MppLevel& level)
{
    int size = GetSize();
    int x = GetPhysicsX();
    int y = GetPhysicsY();
    int width = GetPhysicsWidth();
    int height = GetPhysicsHeight();
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
