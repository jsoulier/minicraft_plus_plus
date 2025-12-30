#pragma once

#include <savepoint/base.hpp>

#include "entity.hpp"
#include "item.hpp"

class MppItemEntity : public MppEntity
{
    SAVEPOINT_DERIVED(MppItemEntity)

public:
    MppItemEntity();
    MppItemEntity(const MppItem& item);
    void Update(MppLevel& level, MppRenderer& renderer, int ticks) override;
    void Visit(SavepointVisitor& visitor);
    int GetSize() const override;
    MppPhysicsType GetPhysicsType() const;
    int GetPhysicsWidth() const override;
    int GetPhysicsHeight() const override;
    
protected:
    int GetPhysicsOffsetX() const override;
    int GetPhysicsOffsetY() const override;

private:
    MppItem Item;
};