#include <cmath>
#include <limits>

#include "color.hpp"
#include "controller.hpp"
#include "mob_entity.hpp"
#include "renderer.hpp"
#include "sprite.hpp"

MppMobEntity::MppMobEntity()
    : MppEntity()
{
}

void MppMobEntity::Update(MppLevel& level, MppRenderer& renderer, float dt, float ticks)
{
    Move(dt, ticks);
    renderer.Draw(MppSprite(kMppColorWhite), X, Y, MppRenderer::LayerEntity);
}

void MppMobEntity::Move(float dt, float ticks)
{
    float dx = 0.0f;
    float dy = 0.0f;
    Controller->GetMovement(dx, dy);
    float length = std::hypotf(dx, dy);
    if (length < std::numeric_limits<float>::epsilon())
    {
        return;
    }
    float speed = GetSpeed();
    dx *= speed * dt;
    dy *= speed * dt;
    if (length > 1.0f)
    {
        dx /= length;
        dy /= length;
    }
    X += dx;
    Y += dy;
}

void MppMobEntity::SetController(const std::shared_ptr<MppController>& controller)
{
    Controller = controller;
}

float MppMobEntity::GetSpeed() const
{
    return 0.1f;
}