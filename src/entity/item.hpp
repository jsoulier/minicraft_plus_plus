#pragma once

#include <savepoint/savepoint.hpp>

#include "../entity.hpp"
#include "../item.hpp"

class MppItemEntity final : public MppEntity
{
    SAVEPOINT_POLY(MppItemEntity)

public:
    MppItemEntity();
    MppItemEntity(const MppItem& item);
    void Visit(SavepointVisitor& visitor) override;
    void Render() const override;
    bool OnCollision(MppEntity& instigator) override;
    int GetPhysicsOffsetX() const override;
    int GetPhysicsOffsetY() const override;
    int GetPhysicsWidth() const override;
    int GetPhysicsHeight() const override;
    int GetSize() const override;

private:
    MppItem Item;
};
