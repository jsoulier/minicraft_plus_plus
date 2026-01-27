#include <savepoint/savepoint.hpp>

#include <cmath>
#include <cstdint>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "entity.hpp"
#include "tile.hpp"
#include "world.hpp"

MppEntity::MppEntity()
    : X{0}
    , Y{0}
    , Dead{false}
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

int MppEntity::GetDistance(const std::shared_ptr<MppEntity>& entity) const
{
    int x = GetPhysicsX() + GetPhysicsWidth() / 2;
    int y = GetPhysicsY() + GetPhysicsHeight() / 2;
    int otherX = entity->GetX() + entity->GetPhysicsWidth() / 2;
    int otherY = entity->GetY() + entity->GetPhysicsHeight() / 2;
    float dx = otherX - x;
    float dy = otherY - y;
    return std::sqrt(dx * dx + dy * dy);
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
    bool rejected = false;
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
        if (tile.GetPhysicsType() == MppTilePhysicsTypeWall)
        {
            rejected = true;
        }
    }
    std::vector<std::shared_ptr<MppEntity>> entities = MppWorldGetEntities(X, Y);
    for (std::shared_ptr<MppEntity>& entity : entities)
    {
        int ex = entity->GetPhysicsX();
        int ey = entity->GetPhysicsY();
        int ew = entity->GetPhysicsWidth();
        int eh = entity->GetPhysicsHeight();
        if (!Test(x, y, w, h, ex, ey, ew, eh))
        {
            continue;
        }
        // TODO: handle collisions
        // or... we can handle them in OnCollision.
        // furniture can inherit mob velocity and immediately call MppEntity::Move
        entity->OnCollision(*this);
    }
    return !rejected;
}

std::vector<std::pair<int, int>> MppEntity::Raycast(int x2, int y2)
{
    int x1 = X;
    int y1 = Y;
    int dx = std::abs(x2 - x1);
    int dy = std::abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int error = dx - dy;
    int x = x1;
    int y = y1;
    std::vector<std::pair<int, int>> points;
    while (true)
    {
        int tx = x / 16;
        int ty = y / 16;
        const MppTile& tile = MppWorldGetTile(tx, ty);
        if (tile.GetPhysicsType() == MppTilePhysicsTypeWall)
        {
            points.clear();
            break;
        }
        points.emplace_back(x, y);
        if (x == x2 && y == y2)
        {
            break;
        }
        int e2 = error * 2;
        if (e2 > -dy)
        {
            error -= dy;
            x += sx;
        }
        if (e2 < dx)
        {
            error += dx;
            y += sy;
        }
    }
    return points;
}

void MppEntity::Kill()
{
    Dead = true;
}

bool MppEntity::IsDead() const
{
    return Dead;
}

std::string MppEntity::GetName() const
{
    return std::string(SavepointDerivedGetString());
}
