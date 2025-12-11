#include "color.hpp"
#include "mob_entity.hpp"
#include "renderer.hpp"
#include "sprite.hpp"

MppMobEntity::MppMobEntity()
    : MppEntity()
{
}

void MppMobEntity::Update(MppLevel& level, MppRenderer& renderer, float dt, float ticks)
{
    renderer.Draw(MppSprite(kMppColorWhite), X, Y, MppRenderer::LayerEntity);
}

void MppMobEntity::SetController(const std::shared_ptr<MppController>& controller)
{
    Controller = controller;
}