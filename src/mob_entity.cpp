#include <glm/glm.hpp>

#include <cmath>

#include "mob_entity.hpp"
#include "ticks.hpp"

static constexpr float kEpsilon = std::numeric_limits<float>::epsilon();
static constexpr float kSpeed = 0.0000001f;

void MppMobEntity::Update(const MppTicks& ticks)
{
    glm::vec2 delta = GetPositionDelta();
    if (glm::length(delta) < kEpsilon)
    {
        return;
    }
    Transform.Position.x -= delta.x * kSpeed * ticks.GetDeltaTime();
    Transform.Position.z += delta.y * kSpeed * ticks.GetDeltaTime();
    Transform.Rotation = std::atan2(delta.y, delta.x);
}