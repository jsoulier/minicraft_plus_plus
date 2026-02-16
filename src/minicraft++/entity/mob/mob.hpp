#pragma once

#include <savepoint/savepoint.hpp>

#include <memory>

#include <minicraft++/entity/entity.hpp>
#include <minicraft++/renderer.hpp>
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
    virtual void Visit(SavepointVisitor& visitor) override;
    virtual void Update(uint64_t ticks) override;
    void Render(MppRendererLayer layer) const;
    virtual void Render() const override;
    virtual void DoAction();
    virtual void Equip(int index);
    void Push(int dx, int dy, bool useSpeed);
    bool IsInFov(const std::shared_ptr<MppEntity>& entity);
    int GetSize() const override;
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
    virtual int GetActionOffset() const;
    std::shared_ptr<MppController> GetController();
    std::shared_ptr<MppInventory> GetInventory();
    virtual std::shared_ptr<MppController> GetDefaultController();
    void RequestAnimationTick();

protected:
    virtual int GetMaxItems() const;
    virtual int GetMoveTickRate() const;
    virtual int GetAnimationTickRate() const;
    virtual float GetFov() const;
    virtual int GetMaxHealth() const = 0;
    virtual int GetMaxHunger() const = 0;
    virtual int GetMaxEnergy() const = 0;
    virtual int GetColor1() const = 0;
    virtual int GetColor2() const = 0;
    virtual int GetColor3() const = 0;
    virtual int GetColor4() const = 0;
    virtual int GetColor5() const = 0;
    virtual int GetAnimationPose() const;
    virtual int GetAnimationPose1X() const = 0;
    virtual int GetAnimationPose1Y() const = 0;
    virtual int GetAnimationPose2X() const;
    virtual int GetAnimationPose2Y() const;
    virtual int GetAnimationPose3X() const;
    virtual int GetAnimationPose3Y() const;

protected:
    std::shared_ptr<MppInventory> Inventory;
    std::shared_ptr<MppController> Controller;
    MppSpriteAnimation Animation;

private:
    bool TickAnimation;
    int FacingX;
    int FacingY;
    int VelocityX;
    int VelocityY;
    int Health;
    int Hunger;
    int Energy;
};
