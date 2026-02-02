#pragma once

#include <cstdint>
#include <memory>

#include "../sprite.hpp"
#include "mob.hpp"

class MppSpiderEntity final : public MppMobEntity
{
    SAVEPOINT_DERIVED(MppSpiderEntity)

public:
    MppSpiderEntity();
    void OnAddEntity() override;
    void PostUpdate(uint64_t ticks) override;
    void Render() const override;
    
protected:
    std::shared_ptr<MppController> GetController();
    int GetSpritePose1X() const;
    int GetSpritePose1Y() const;
    int GetPhysicsOffsetX() const override;
    int GetPhysicsOffsetY() const override;
    int GetPhysicsWidth() const override;
    int GetPhysicsHeight() const override;
    int GetSpriteBorderColor() const;
    int GetSpriteEyeColor() const;
    int GetSpriteBodyColor1() const;
    int GetSpriteBodyColor2() const;
    int GetSpriteLegColor() const;
    int GetMaxHealth() const override;
    int GetMaxHunger() const override;
    int GetMaxEnergy() const override;
    int GetSpeed() const override;
    
private:
    MppSpriteAnimation Animation;
};