#pragma once

#include "entity.hpp"

class MppItemEntity : public MppEntity
{
    SAVEPOINT_DERIVED(MppItemEntity)

public:
    void Update(MppLevel& level, MppRenderer& renderer, float dt, float ticks) override;
    int GetSize() const override;
    float GetPhysicsWidth() const override;
    float GetPhysicsHeight() const override;
    
protected:
    int GetPhysicsOffsetX() const override;
    int GetPhysicsOffsetY() const override;
};