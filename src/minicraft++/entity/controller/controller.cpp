#include <cstdint>
#include <memory>

#include <minicraft++/entity/controller/controller.hpp>
#include <minicraft++/entity/entity.hpp>
#include <minicraft++/entity/mob/mob.hpp>
#include <minicraft++/inventory.hpp>

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

bool MppController::ActionFilter(const std::shared_ptr<MppEntity>& entity) const
{
    return !entity->IsA<MppMobEntity>();
}
