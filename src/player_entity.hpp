#pragma once

#include "mob_entity.hpp"

class MppPlayerEntity : public MppMobEntity
{
public:
    glm::vec2 GetPositionDelta() const override;
};