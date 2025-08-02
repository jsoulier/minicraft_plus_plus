#pragma once

#include <glm/glm.hpp>

#include "entity.hpp"

class MppMobEntity : public MppEntity
{
public:
    void Update(const MppTicks& ticks) override;
    virtual glm::vec2 GetPositionDelta() const = 0;
};