#include <cmath>
#include <limits>

#include "color.hpp"
#include "controller.hpp"
#include "level.hpp"
#include "mob_entity.hpp"
#include "renderer.hpp"
#include "sprite.hpp"

MppMobEntity::MppMobEntity()
    : MppEntity()
{
}

void MppMobEntity::Update(MppLevel& level, MppRenderer& renderer, float dt, float ticks)
{
    Move(level, dt, ticks);
    renderer.Draw(MppSprite(kMppColorWhite), X, Y, MppRenderer::LayerMobEntity);
}

void MppMobEntity::Move(MppLevel& level, float dt, float ticks)
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
    MppEntity::Move(level, dx, 0.0f);
    MppEntity::Move(level, 0.0f, dy);
}

int MppMobEntity::GetSize() const
{
    return 16;
}

void MppMobEntity::SetController(const std::shared_ptr<MppController>& controller)
{
    Controller = controller;
}

float MppMobEntity::GetSpeed() const
{
    return 0.1f;
}