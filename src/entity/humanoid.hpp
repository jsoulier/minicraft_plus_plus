#pragma once

#include "mob.hpp"

class MppHumanoidEntity : public MppMobEntity
{
public:
    virtual void Update(uint64_t ticks) override;
    virtual void Render() override;
    int GetPhysicsOffsetX() const override;
    int GetPhysicsOffsetY() const override;
    int GetPhysicsWidth() const override;
    int GetPhysicsHeight() const override;
};