#pragma once

#include <savepoint/savepoint.hpp>

#include <cstdint>

#include "../sprite.hpp"
#include "mob.hpp"

enum MppCreatureEntityState
{
    MppCreatureEntityStateIdle,
    MppCreatureEntityStateMove,
};

class MppCreatureEntity : public MppMobEntity
{
public:
    MppCreatureEntity();
    virtual void OnAddEntity() override;
    virtual void Visit(SavepointVisitor& visitor) override;
    virtual void Update(uint64_t ticks) override;
    virtual void Render() const override;
    virtual void OnAction(MppEntity& instigator) override;

protected:
    virtual int GetSpritePoseX() const = 0;
    virtual int GetSpritePoseY() const = 0;
    virtual int GetSpriteColor1() const = 0;
    virtual int GetSpriteColor2() const = 0;
    virtual int GetSpriteColor3() const = 0;
    virtual int GetSpriteColor4() const = 0;
    virtual int GetSpriteColor5() const = 0;
    int GetSpeed() const override final;
    int GetDefaultSpeed() const;
    int GetFleeSpeed() const;
    int GetIdleCooldown() const;
    int GetFleeCooldown() const;

private:
    void Idle();
    void Move();

private:
    MppSpriteAnimation Animation;
    MppCreatureEntityState State;
    int TargetX;
    int TargetY;
    int IdleTicks;
    int FleeTicks;
};
