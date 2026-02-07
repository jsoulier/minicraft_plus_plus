#pragma once

#include <savepoint/savepoint.hpp>

#include <memory>

#include "../input.hpp"
#include "../menu.hpp"
#include "humanoid.hpp"

class MppPlayerEntity final
    : public MppHumanoidEntity
    , public MppInputHandler
    , public MppMenu
{
    SAVEPOINT_POLY(MppPlayerEntity)

public:
    MppPlayerEntity();
    void OnAddEntity() override;
    void Visit(SavepointVisitor& visitor) override;
    void Update(uint64_t ticks) override;
    void Render() const override;
    void SetCrouching(bool crouching);
    int GetSpeed() const override;

protected:
    std::shared_ptr<MppController> GetController();
    int GetSpriteColor1() const override;
    int GetSpriteColor2() const override;
    int GetSpriteColor3() const override;
    int GetSpriteColor4() const override;
    int GetSpriteColor5() const override;
    int GetMaxHealth() const override;
    int GetMaxHunger() const override;
    int GetMaxEnergy() const override;

private:
    bool Crouching;
};