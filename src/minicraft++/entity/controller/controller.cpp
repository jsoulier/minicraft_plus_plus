#include <cstdint>
#include <memory>

#include <minicraft++/entity/controller/controller.hpp>
#include <minicraft++/entity/entity.hpp>
#include <minicraft++/entity/mob/mob.hpp>
#include <minicraft++/inventory.hpp>


void MppController::Visit(SavepointVisitor& visitor)
{
}

void MppController::Possess(const std::shared_ptr<MppMobEntity>& entity)
{
    Entity = entity;
    entity->OnPossess(shared_from_this());
}

void MppController::Unpossess()
{
    std::shared_ptr<MppMobEntity> entity = Entity.lock();
    if (entity)
    {
        entity->OnUnpossess();
    }
    Entity.reset();
}

std::shared_ptr<MppInventory> MppController::GetInventory() const
{
    return Entity.lock()->GetInventory();
}

bool MppController::ActionFilter(const std::shared_ptr<MppEntity>& entity) const
{
    return !entity->IsA<MppMobEntity>();
}

bool MppController::InteractionFilter(const std::shared_ptr<MppEntity>& entity) const
{
    return false;
}
