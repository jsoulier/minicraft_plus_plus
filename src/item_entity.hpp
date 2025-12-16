#pragma once

#include "entity.hpp"

class MppItemEntity : public MppEntity
{
    SAVEPOINT_DERIVED(MppItemEntity)

public:
    void Update(MppLevel& level, MppRenderer& renderer, int ticks) override;
    int GetSize() const override;
    int GetPhysicsWidth() const override;
    int GetPhysicsHeight() const override;
    
protected:
    int GetPhysicsOffsetX() const override;
    int GetPhysicsOffsetY() const override;
};