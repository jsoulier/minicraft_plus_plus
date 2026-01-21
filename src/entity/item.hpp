#pragma once

#include <savepoint/base.hpp>

#include "../entity.hpp"
#include "../item.hpp"

class MppItemEntity final : public MppEntity
{
    SAVEPOINT_DERIVED(MppItemEntity)

public:
    MppItemEntity();
    MppItemEntity(const MppItem& item);
    void Visit(SavepointVisitor& visitor) override;
    void Render() const override;
    int GetPhysicsOffsetX() const override;
    int GetPhysicsOffsetY() const override;
    int GetPhysicsWidth() const override;
    int GetPhysicsHeight() const override;
    int GetSize() const override;

private:
    MppItem Item;
};
