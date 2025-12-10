#include "mob_entity.hpp"

MppMobEntity::MppMobEntity()
    : MppEntity()
{
}

void MppMobEntity::Update(const std::shared_ptr<MppLevel>& level, MppRenderer& renderer, float dt, float ticks)
{
}

void MppMobEntity::SetController(const std::shared_ptr<MppController>& controller)
{
    Controller = controller;
}