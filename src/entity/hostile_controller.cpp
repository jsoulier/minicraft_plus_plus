#include <savepoint/savepoint.hpp>

#include <cstdint>
#include <memory>

#include "../world.hpp"
#include "hostile_controller.hpp"
#include "mob.hpp"

MppHostileMobController::MppHostileMobController(const std::shared_ptr<MppMobEntity>& entity)
    : MppMobController(entity)
{
}

void MppHostileMobController::Visit(SavepointVisitor& visitor)
{
    MppMobController::Visit(visitor);
    // TODO: serialize target
}

void MppHostileMobController::Update(uint64_t ticks)
{
    std::shared_ptr<MppEntity> entity = Entity.lock();
    for (std::shared_ptr<MppEntity>& entity : MppWorldGetEntities(entity->GetX(), entity->GetY()))
    {
        // TODO:
    }
}
