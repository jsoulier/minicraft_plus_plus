#pragma once

#include "mob_entity.hpp"

class MppHumanoidEntity : public MppMobEntity
{
public:
    MppHumanoidEntity();
    float GetPhysicsWidth() const override;
    float GetPhysicsHeight() const override;
    
protected:
    int GetPhysicsOffsetX() const override;
    int GetPhysicsOffsetY() const override;
};