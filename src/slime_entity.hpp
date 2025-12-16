#pragma once

#include "mob_entity.hpp"

class MppSlimeEntity : public MppMobEntity
{
    SAVEPOINT_DERIVED(MppSlimeEntity)

public:
    MppSlimeEntity();
    float GetPhysicsWidth() const override;
    float GetPhysicsHeight() const override;
    
protected:
    int GetPhysicsOffsetX() const override;
    int GetPhysicsOffsetY() const override;
};