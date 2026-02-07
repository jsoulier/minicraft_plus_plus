#include <savepoint/savepoint.hpp>

#include <algorithm>
#include <cmath>
#include <memory>

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

MppEntityReference::MppEntityReference(const std::shared_ptr<MppEntity>& entity)
    : Entity{entity}
    , EntityID{entity->GetID()}
{
}

void MppEntityReference::Visit(SavepointVisitor& visitor)
{
    if (visitor.IsReading())
    {
        MppAssert(Entity.expired());
    }
    visitor(EntityID);
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
    std::shared_ptr<MppEntity> entity = MppWorldGetEntity(EntityID);
    Entity = entity;
    if (!entity)
    {
        MppLog("Failed to find reference on entity");
        EntityID = SavepointID{};
    }
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
    , Killed{false}
{
}

void MppEntity::OnAddEntity()
{
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

bool MppEntity::OnCollision(MppEntity& instigator)
{
    return false;
}

bool MppEntity::HasPhysics() const
{
    return true;
}

bool MppEntity::CanSave() const
{
    return true;
}

MppEntityReference MppEntity::GetReference()
{
    return MppEntityReference(shared_from_this());
}

void MppEntity::Kill()
{
    Killed = true;
}

bool MppEntity::IsKilled() const
{
    return Killed;
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

void MppEntity::GetCenter(int& x, int& y) const
{
    x = GetPhysicsX() + GetPhysicsWidth() / 2;
    y = GetPhysicsY() + GetPhysicsHeight() / 2;
}

int MppEntity::GetDistance(const std::shared_ptr<MppEntity>& entity) const
{
    int x1;
    int y1;
    int x2;
    int y2;
    GetCenter(x1, y1);
    entity->GetCenter(x2, y2);
    float dx = x2 - x1;
    float dy = y2 - y1;
    return std::sqrt(dx * dx + dy * dy);
}

void MppEntity::Move(int dx, int dy)
{
    while (dx || dy)
    {
        if (dx)
        {
            int step = dx / std::abs(dx);
            MoveTest(step, 0);
            dx -= step;
        }
        if (dy)
        {
            int step = dy / std::abs(dy);
            MoveTest(0, step);
            dy -= step;
        }
    }
}

void MppEntity::MoveTest(int dx, int dy)
{
    int entityX = X;
    int entityY = Y;
    X += dx;
    Y += dy;
    int size = GetSize();
    int x = GetPhysicsX();
    int y = GetPhysicsY();
    int w = GetPhysicsWidth();
    int h = GetPhysicsHeight();
    auto test = [&](float x2, float y2, int w2, int h2)
    {
        return (x + w > x2) && (x < x2 + w2) && (y + h > y2) && (y < y2 + h2);
    };
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
        if (!test(tx, ty, tw, th))
        {
            continue;
        }
        rejected |= tile.OnCollision(*this);
    }
    for (std::shared_ptr<MppEntity>& entity : MppWorldGetEntities(X, Y))
    {
        int ex = entity->GetPhysicsX();
        int ey = entity->GetPhysicsY();
        int ew = entity->GetPhysicsWidth();
        int eh = entity->GetPhysicsHeight();
        if (!test(ex, ey, ew, eh))
        {
            continue;
        }
        rejected |= entity->OnCollision(*this);
    }
    if (rejected)
    {
        X = entityX;
        Y = entityY;
    }
}
