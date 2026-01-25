#include <cstdint>
#include <memory>

#include "controller.hpp"
#include "mob.hpp"

MppMobController::MppMobController(const std::shared_ptr<MppMobEntity>& entity)
    : Entity{entity}
{
}

void MppMobController::Update(uint64_t ticks)
{
}
