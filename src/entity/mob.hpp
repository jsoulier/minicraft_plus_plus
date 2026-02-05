#pragma once

#include <savepoint/savepoint.hpp>

#include <memory>

#include "../entity.hpp"

class MppController;
class MppInventory;

class MppMobEntity : public MppEntity
{
public:
    MppMobEntity();
    virtual void OnAddEntity() override;
    virtual void Visit(SavepointVisitor& visitor) override;
    virtual void Update(uint64_t ticks) override final;
    virtual void Render() const override;
    bool IsInFov(const std::shared_ptr<MppEntity>& entity);
    int GetSize() const override;
    std::shared_ptr<MppInventory> GetInventory();
    virtual int GetActionRange() const;
    void Push(int dx, int dy);
    void PushNow(int dx, int dy);
    int GetFacingX() const;
    int GetFacingY() const;
    virtual void DoAction(std::shared_ptr<MppEntity>& entity) {}

protected:
    virtual void PostUpdate(uint64_t ticks);
    virtual int GetMaxItems() const;
    virtual int GetSpeed() const;
    virtual std::shared_ptr<MppController> GetController();
    virtual float GetFov() const;
    virtual int GetMaxHealth() const = 0;
    virtual int GetMaxHunger() const = 0;
    virtual int GetMaxEnergy() const = 0;

protected:
    std::shared_ptr<MppInventory> Inventory;
    std::shared_ptr<MppController> Controller;
    int VelocityX;
    int VelocityY;
    int FacingX;
    int FacingY;
    int Health;
    int Hunger;
    int Energy;
};
