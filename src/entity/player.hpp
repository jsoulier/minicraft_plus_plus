#pragma once

#include <savepoint/savepoint.hpp>

#include <memory>

#include "../input.hpp"
#include "humanoid.hpp"

class MppPlayerEntity final
    : public MppHumanoidEntity
    , public MppInputHandler
{
    SAVEPOINT_DERIVED(MppPlayerEntity)

public:
    MppPlayerEntity();
    void OnAddEntity() override;
    void PostUpdate(uint64_t ticks) override;
    void Render() const override;
    void OnAction() override;
    void OnInventory() override;
    void OnHeldUp() override;
    void OnHeldDown() override;
    void OnHeldLeft() override;
    void OnHeldRight() override;

protected:
    int GetSpriteBorderColor() const override;
    int GetSpriteSkinColor() const override;
    int GetSpriteShirtColor() const override;
    int GetSpritePantColor() const override;
    int GetSpriteShoeColor() const override;
};