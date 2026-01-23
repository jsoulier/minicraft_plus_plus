#pragma once

#include <memory>

#include "../entity.hpp"

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

protected:
    std::shared_ptr<MppInventory> Inventory;
    int VelocityX;
    int VelocityY;
    int FacingX;
    int FacingY;
};