#pragma once

#include <savepoint/base.hpp>

#include "mob_entity.hpp"

class MppSlimeEntity : public MppMobEntity
{
    SAVEPOINT_DERIVED(MppSlimeEntity)

public:
    MppSlimeEntity();
    int GetPhysicsWidth() const override;
    int GetPhysicsHeight() const override;
    
protected:
    int GetPhysicsOffsetX() const override;
    int GetPhysicsOffsetY() const override;
};