#pragma once

#include "mob_entity.hpp"

class MppHumanoidEntity : public MppMobEntity
{
public:
    MppHumanoidEntity();
    int GetPhysicsWidth() const override;
    int GetPhysicsHeight() const override;
    
protected:
    int GetPhysicsOffsetX() const override;
    int GetPhysicsOffsetY() const override;
};