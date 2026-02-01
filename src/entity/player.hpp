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
    SAVEPOINT_DERIVED(MppPlayerEntity)

public:
    MppPlayerEntity();
    void OnAddEntity() override;
    void PostUpdate(uint64_t ticks) override;
    void Render() const override;

protected:
    std::shared_ptr<MppController> GetController();
    int GetSpriteBorderColor() const override;
    int GetSpriteSkinColor() const override;
    int GetSpriteShirtColor() const override;
    int GetSpritePantColor() const override;
    int GetSpriteShoeColor() const override;
    int GetMaxHealth() const override;
    int GetMaxHunger() const override;
    int GetMaxEnergy() const override;
};