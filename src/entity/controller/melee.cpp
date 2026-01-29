#include <memory>

#include "../mob.hpp"
#include "melee.hpp"

MppMeleeController::MppMeleeController(const std::shared_ptr<MppMobEntity>& entity)
    : MppAttackController(entity)
{
}

void MppMeleeController::GetAttackPosition(const std::weak_ptr<MppMobEntity>& target, int& x, int& y) const
{
    std::shared_ptr<MppMobEntity> entity = target.lock();
    x = entity->GetX();
    y = entity->GetY();
}
