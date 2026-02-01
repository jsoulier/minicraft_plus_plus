#include <cstdint>
#include <memory>

#include "../inventory.hpp"
#include "controller.hpp"
#include "mob.hpp"

void MppController::Visit(SavepointVisitor& visitor)
{
}

void MppController::SetEntity(const std::shared_ptr<MppMobEntity>& entity)
{
    Entity = entity;
}

std::shared_ptr<MppInventory> MppController::GetInventory()
{
    return Entity.lock()->GetInventory();
}
