#include <cstdint>
#include <memory>

#include <minicraft++/assert.hpp>
#include <minicraft++/entity/controller/controller.hpp>
#include <minicraft++/entity/entity.hpp>
#include <minicraft++/entity/mob/mob.hpp>
#include <minicraft++/inventory.hpp>

bool MppController::Possess(const std::shared_ptr<MppMobEntity>& entity)
{
    if (Entity.lock() == entity)
    {
        return false;
    }
    std::shared_ptr<MppController> controller = entity->GetController();
    if (controller)
    {
        controller->Unpossess();
    }
    if (Entity.lock())
    {
        Unpossess();
    }
    Entity = entity;
    entity->OnPossess(shared_from_this());
    return true;
}

bool MppController::Unpossess()
{
    std::shared_ptr<MppMobEntity> entity = Entity.lock();
    if (!entity)
    {
        return false;
    }
    entity->OnUnpossess();
    Entity.reset();
    return true;
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
