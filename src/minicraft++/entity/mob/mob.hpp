#pragma once

#include <savepoint/savepoint.hpp>

#include <memory>

#include <minicraft++/entity/entity.hpp>
#include <minicraft++/sprite.hpp>

class MppController;
class MppInventory;

class MppMobEntity : public MppEntity
{
public:
    MppMobEntity();
    virtual void OnCreate() override;
    virtual void OnAdd() override;
    virtual void OnPossess(const std::shared_ptr<MppController>& controller);
    virtual void OnUnpossess();
    std::shared_ptr<MppController>& GetController();
    virtual void Visit(SavepointVisitor& visitor) override;
    virtual void Update(uint64_t ticks) override;
    virtual void Render() const override;
    virtual void DoAction();
    virtual void EquipItemFromInventory(int index);
    bool IsInFov(const std::shared_ptr<MppEntity>& entity);
    int GetSize() const override;
    std::shared_ptr<MppInventory> GetInventory();
    virtual int GetActionRange() const;
    void Push(int dx, int dy);
    void SetFacingX(int facingX);
    void SetFacingY(int facingY);
    int GetFacingX() const;
    int GetFacingY() const;
    bool IsFacing(int facingX, int facingY) const;
    bool IsMoving();
    int GetHealth() const;
    int GetHunger() const;
    int GetEnergy() const;
    virtual int GetSpeed() const;

protected:
    virtual int GetMaxItems() const;
    virtual int GetMoveTickRate() const;
    virtual std::shared_ptr<MppController> GetDefaultController();
    virtual float GetFov() const;
    virtual int GetMaxHealth() const = 0;
    virtual int GetMaxHunger() const = 0;
    virtual int GetMaxEnergy() const = 0;
    virtual int GetPose() const;
    virtual int GetSpriteColor1() const = 0;
    virtual int GetSpriteColor2() const = 0;
    virtual int GetSpriteColor3() const = 0;
    virtual int GetSpriteColor4() const = 0;
    virtual int GetSpriteColor5() const = 0;
    virtual int GetSpriteTickRate() const;
    virtual int GetSpritePose1X() const = 0;
    virtual int GetSpritePose1Y() const = 0;
    virtual int GetSpritePose2X() const;
    virtual int GetSpritePose2Y() const;
    void SetTickAnimation();
    virtual int GetActionOffset() const;

protected:
    std::shared_ptr<MppInventory> Inventory;
    std::shared_ptr<MppController> Controller;
    MppSpriteAnimation Animation;

private:
    int FacingX;
    int FacingY;
    int VelocityX;
    int VelocityY;
    int Health;
    int Hunger;
    int Energy;
    bool TickAnimation;
};
