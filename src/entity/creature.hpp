#pragma once

#include <savepoint/savepoint.hpp>

#include <cstdint>

#include "../sprite.hpp"
#include "mob.hpp"

class MppCreatureEntity : public MppMobEntity
{
public:
    MppCreatureEntity();
    virtual void OnAddEntity() override;
    virtual void Visit(SavepointVisitor& visitor) override;
    virtual void PostUpdate(uint64_t ticks) override;
    virtual void Render() const override;
    virtual void OnAction(MppEntity& instigator) override;
    int GetSpeed() const override;
    int GetDefaultSpeed() const;
    int GetFleeSpeed() const;
    int GetIdleCooldown() const;
    int GetFleeCooldown() const;
    int GetFleeTicks() const;

protected:
    virtual int GetSpritePoseX() const = 0;
    virtual int GetSpritePoseY() const = 0;
    virtual int GetSpriteColor1() const = 0;
    virtual int GetSpriteColor2() const = 0;
    virtual int GetSpriteColor3() const = 0;
    virtual int GetSpriteColor4() const = 0;
    virtual int GetSpriteColor5() const = 0;
    std::shared_ptr<MppController> GetController() override;

private:
    MppSpriteAnimation Animation;
    int FleeTicks;
};
