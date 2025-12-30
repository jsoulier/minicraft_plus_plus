#pragma once

#include <savepoint/fwd.hpp>

#include <memory>

#include "entity.hpp"

class MppController;
class MppMobInventory;

class MppMobEntity : public MppEntity
{
public:
    MppMobEntity();
    void Update(MppLevel& level, MppRenderer& renderer, int ticks) override;
    virtual void Visit(SavepointVisitor& visitor);
    virtual void Move(MppLevel& level, int dx, int dy, int ticks);
    int GetSize() const override;
    MppPhysicsType GetPhysicsType() const override;
    const std::shared_ptr<MppMobInventory>& GetInventory();

protected:
    void SetController(const std::shared_ptr<MppController>& controller);
    void SetInventory(const std::shared_ptr<MppMobInventory>& inventory);
    
    int DeltaX;
    int DeltaY;

private:
    std::shared_ptr<MppController> Controller;
    std::shared_ptr<MppMobInventory> Inventory;
};