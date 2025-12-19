#pragma once

#include <savepoint_fwd.hpp>

#include "entity.hpp"
#include "inventory.hpp"

class MppController;

class MppMobEntity : public MppEntity
{
public:
    MppMobEntity();
    void Update(MppLevel& level, MppRenderer& renderer, int ticks) override;
    virtual void Visit(SavepointVisitor& visitor);
    virtual void Move(MppLevel& level, int dx, int dy, int ticks);
    int GetSize() const override;
    void SetDrawInventory(bool draw);
    bool GetDrawInventory() const;

protected:
    void SetController(const std::shared_ptr<MppController>& controller);
    
    int DirectionX;
    int DirectionY;
    bool DrawInventory;

private:
    std::shared_ptr<MppController> Controller;
    MppInventory Inventory;
};