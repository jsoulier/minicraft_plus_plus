#pragma once

#include <cstdint>
#include <memory>

#include "mob.hpp"

class MppHumanoidEntity : public MppMobEntity
{
public:
    virtual void Visit(SavepointVisitor& visitor);
    virtual void Update(uint64_t ticks) override;
    virtual void Render() const override;
    int GetPhysicsOffsetX() const override;
    int GetPhysicsOffsetY() const override;
    int GetPhysicsWidth() const override;
    int GetPhysicsHeight() const override;

protected:
    std::shared_ptr<MppEntity> HeldEntity;
};