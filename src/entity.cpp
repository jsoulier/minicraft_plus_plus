#include <cstdint>

#include "entity.hpp"
#include "tile.hpp"
#include "world.hpp"

MppEntity::MppEntity()
    : X{0}
    , Y{0}
{
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

void MppEntity::Move(int velocityX, int velocityY)
{
    MoveAxis(velocityX, 0);
    MoveAxis(0, velocityY);
}

void MppEntity::MoveAxis(int velocityX, int velocityY)
{
    int x = X;
    int y = Y;
    X += velocityX;
    Y += velocityY;
    if (!MoveAxisTest())
    {
        X = x;
        Y = y;
    }
}

static bool Test(float x1, float y1, int w1, int h1, float x2, float y2, int w2, int h2)
{
    return (x1 + w1 > x2) && (x1 < x2 + w2) && (y1 + h1 > y2) && (y1 < y2 + h2);
}

bool MppEntity::MoveAxisTest()
{
    int size = GetSize();
    int x = GetPhysicsX();
    int y = GetPhysicsY();
    int w = GetPhysicsWidth();
    int h = GetPhysicsHeight();
    int tileX1 = x / MppTile::kSize;
    int tileY1 = y / MppTile::kSize;
    int tileX2 = (x + w) / MppTile::kSize;
    int tileY2 = (y + h) / MppTile::kSize;
    for (int tileX = tileX1; tileX <= tileX2; tileX++)
    for (int tileY = tileY1; tileY <= tileY2; tileY++)
    {
        MppTile& tile = MppWorldGetTile(tileX, tileY);
        int tx = tile.GetPhysicsX(tileX);
        int ty = tile.GetPhysicsY(tileY);
        int tw = tile.GetPhysicsWidth();
        int th = tile.GetPhysicsHeight();
        if (!Test(x, y, w, h, tx, ty, tw, th))
        {
            continue;
        }
        tile.OnCollision(*this);
        if (tile.GetPhysicsType() & MppTilePhysicsTypeWall)
        {
            return false;
        }
    }
    return true;
}
