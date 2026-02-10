#pragma once

#include <cstdint>
#include <memory>

#include <minicraft++/sprite.hpp>
#include <minicraft++/entity/mob/mob.hpp>

class MppSpiderEntity final : public MppMobEntity
{
    SAVEPOINT_POLY(MppSpiderEntity)

public:
    MppSpiderEntity();
    void Render() const override;
    
protected:
    std::shared_ptr<MppController> GetController();
    int GetSpritePose1X() const override;
    int GetSpritePose1Y() const override;
    int GetPhysicsOffsetX() const override;
    int GetPhysicsOffsetY() const override;
    int GetPhysicsWidth() const override;
    int GetPhysicsHeight() const override;
    int GetSpriteColor1() const override;
    int GetSpriteColor2() const override;
    int GetSpriteColor3() const override;
    int GetSpriteColor4() const override;
    int GetSpriteColor5() const override;
    int GetMaxHealth() const override;
    int GetMaxHunger() const override;
    int GetMaxEnergy() const override;
    int GetMoveTickRate() const override;
};