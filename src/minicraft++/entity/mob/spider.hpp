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
    
protected:
    std::shared_ptr<MppController> GetDefaultController();
    int GetAnimationPose1X() const override;
    int GetAnimationPose1Y() const override;
    int GetPhysicsOffsetX() const override;
    int GetPhysicsOffsetY() const override;
    int GetPhysicsWidth() const override;
    int GetPhysicsHeight() const override;
    int GetColor1() const override;
    int GetColor2() const override;
    int GetColor3() const override;
    int GetColor4() const override;
    int GetColor5() const override;
    int GetMaxHealth() const override;
    int GetMaxHunger() const override;
    int GetMaxEnergy() const override;
    int GetMoveTickRate() const override;
};