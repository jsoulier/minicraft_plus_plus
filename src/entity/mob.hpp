#pragma once

#include <savepoint/savepoint.hpp>

#include <memory>

#include "../entity.hpp"

class MppMobController;
class MppInventory;

class MppMobEntity : public MppEntity
{
public:
    MppMobEntity();
    virtual void OnAddEntity() override;
    virtual void Visit(SavepointVisitor& visitor) override;
    virtual void Update(uint64_t ticks) override;
    virtual void Render() const override;
    int GetSize() const override;
    std::shared_ptr<MppInventory> GetInventory();

protected:
    virtual int GetMaxItems() const;
    virtual int GetSpeed() const;

    // Optional Mixin
    virtual std::shared_ptr<MppMobController> GetController() const;

protected:
    std::shared_ptr<MppInventory> Inventory;
    std::shared_ptr<MppMobController> Controller;
    int VelocityX;
    int VelocityY;
    int FacingX;
    int FacingY;
};
