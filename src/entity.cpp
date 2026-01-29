#include <savepoint/savepoint.hpp>

#include <cmath>
#include <cstdint>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "assert.hpp"
#include "color.hpp"
#include "console.hpp"
#include "entity.hpp"
#include "entity/furniture.hpp"
#include "entity/mob.hpp"
#include "entity/player.hpp"
#include "log.hpp"
#include "renderer.hpp"
#include "tile.hpp"
#include "world.hpp"

MppEntityReference::MppEntityReference()
    : Entity{}
    , EntityID{}
{
}

void MppEntityReference::Visit(SavepointVisitor& visitor)
{
    if (visitor.IsReading())
    {
        MppAssert(Entity.expired());
        MppAssert(!EntityID.IsValid());
    }
    visitor(EntityID);
}

void MppEntityReference::SetEntity(const std::shared_ptr<MppEntity>& entity)
{
    Entity = entity;
    if (entity)
    {
        EntityID = entity->SavepointGetID();
    }
    else
    {
        EntityID = SavepointID{};
    }
}

void MppEntityReference::Update()
{
    if (!EntityID.IsValid())
    {
        MppAssert(Entity.expired());
        return;
    }
    if (!Entity.expired())
    {
        return;
    }
    for (std::shared_ptr<MppEntity>& entity : MppWorldGetEntities())
    {
        if (entity->SavepointGetID() == EntityID)
        {
            Entity = entity;
            return;
        }
    }
    MppLog("Failed to find reference on entity");
    EntityID = SavepointID{};
}

std::shared_ptr<MppEntity> MppEntityReference::GetEntity() const
{
    return Entity.lock();
}

bool MppEntityReference::IsValid() const
{
    return !Entity.expired();
}

MppEntity::MppEntity()
    : X{0}
    , Y{0}
    , Dead{false}
{
}

void MppEntity::OnAddEntity()
{
    // Validation
    int x = GetPhysicsOffsetX();
    int y = GetPhysicsOffsetY();
    int w = GetPhysicsWidth();
    int h = GetPhysicsHeight();
    MppAssert((x * 2 + w) == MppTile::kSize);
    MppAssert((y * 2 + h) == MppTile::kSize);
}

void MppEntity::Visit(SavepointVisitor& visitor)
{
    visitor(X);
    visitor(Y);
}

void MppEntity::Render() const
{
    if (MppConsole::CVarPhysics.GetBool())
    {
        int color = 0;
        int x = GetPhysicsX();
        int y = GetPhysicsY();
        int w = GetPhysicsWidth();
        int h = GetPhysicsHeight();
        if (dynamic_cast<const MppPlayerEntity*>(this))
        {
            color = kMppColorDebugPlayerEntityPhysics;
        }
        else if (dynamic_cast<const MppMobEntity*>(this))
        {
            color = kMppColorDebugMobEntityPhysics;
        }
        else if (dynamic_cast<const MppFurnitureEntity*>(this))
        {
            color = kMppColorDebugFurnitureEntityPhysics;
        }
        if (color)
        {
            MppRendererDrawRect(color, x, y, w, h, MppRendererLayerDebugPhysics);
        }
    }
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

void MppEntity::Move(int dx, int dy)
{
    MoveAxis(dx, 0);
    MoveAxis(0, dy);
}

void MppEntity::MoveAxis(int dx, int dy)
{
    int x = X;
    int y = Y;
    X += dx;
    Y += dy;
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
    for (std::shared_ptr<MppEntity>& entity : MppWorldGetEntities(X, Y))
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

std::vector<std::pair<int, int>> MppEntity::Raycast(const std::shared_ptr<MppEntity>& entity)
{
    // TODO: adding the kSize / 2 here might be a problem
    int x1 = X + MppTile::kSize / 2;
    int y1 = Y + MppTile::kSize / 2;
    int x2 = entity->GetX() + MppTile::kSize / 2;
    int y2 = entity->GetY() + MppTile::kSize / 2;
    int tx = x1 / MppTile::kSize;
    int ty = y1 / MppTile::kSize;
    int tx2 = x2 / MppTile::kSize;
    int ty2 = y2 / MppTile::kSize;
    int dx = std::abs(tx2 - tx);
    int dy = std::abs(ty2 - ty);
    int sx = (tx < tx2) ? 1 : -1;
    int sy = (ty < ty2) ? 1 : -1;
    int error = dx - dy;
    std::vector<std::pair<int, int>> points;
    while (true)
    {
        const MppTile& tile = MppWorldGetTile(tx, ty);
        if (tile.GetPhysicsType() == MppTilePhysicsTypeWall)
        {
            points.clear();
            break;
        }
        // TODO: I removed the MppTile::kSize / 2 because I don't think we want it but I also haven't tested yet
        int x = tx * MppTile::kSize;
        int y = ty * MppTile::kSize;
        // int x = tx * MppTile::kSize + MppTile::kSize / 2;
        // int y = ty * MppTile::kSize + MppTile::kSize / 2;
        points.emplace_back(x, y);
        if (tx == tx2 && ty == ty2)
        {
            break;
        }
        int e2 = error * 2;
        if (e2 > -dy)
        {
            error -= dy;
            tx += sx;
        }
        else if (e2 < dx)
        {
            error += dx;
            ty += sy;
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
    return std::string(SavepointGetString());
}
