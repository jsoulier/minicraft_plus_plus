#include <cstdint>
#include <memory>

#include "controller.hpp"
#include "mob.hpp"

MppMobController::MppMobController(const std::shared_ptr<MppMobEntity>& entity)
    : Entity{entity}
{
}

void MppMobController::Visit(SavepointVisitor& visitor)
{
}

void MppMobController::SetEntity(const std::shared_ptr<MppMobEntity>& entity)
{
    Entity = entity;
}
