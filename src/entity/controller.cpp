#include <cstdint>
#include <memory>

#include "../assert.hpp"
#include "../tile.hpp"
#include "../util.hpp"
#include "../world.hpp"
#include "controller.hpp"
#include "mob.hpp"

MppController::MppController(const std::shared_ptr<MppMobEntity>& entity)
    : Entity{entity}
{
}

void MppController::Visit(SavepointVisitor& visitor)
{
}

void MppController::SetEntity(const std::shared_ptr<MppMobEntity>& entity)
{
    Entity = entity;
}

void MppController::PickRandomTarget(int maxDistance, int& x, int& y)
{
    std::shared_ptr<MppMobEntity> entity = Entity.lock();
    MppAssert(entity);
    int dx = 0;
    int dy = 0;
    switch (MppGetRandomInt(0, 3))
    {
    case 0: dx = -1; break;
    case 1: dx = 1; break;
    case 2: dy = -1; break;
    case 3: dy = 1; break;
    }
    int distance = MppGetRandomInt(1, maxDistance);
    int tx = entity->GetX() / MppTile::kSize;
    int ty = entity->GetY() / MppTile::kSize;
    for (int i = 0; i < distance; i++)
    {
        tx += dx;
        ty += dy;
        const MppTile& tile = MppWorldGetTile(tx, ty);
        if (tile.GetPhysicsType() != MppTilePhysicsTypeGround)
        {
            tx -= dx;
            ty -= dy;
            break;
        }
    }
    x = tx * MppTile::kSize;
    y = ty * MppTile::kSize;
}
