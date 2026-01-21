#pragma once

#include <memory>

#include "../input.hpp"
#include "humanoid.hpp"

class MppPlayerEntity final
    : public MppHumanoidEntity
    , public MppInputHandler
{
    SAVEPOINT_DERIVED(MppPlayerEntity)

public:
    void OnAddEntity() override;
    void Update(uint64_t ticks) override;
    void Render() override;
    void OnAction() override;
    void OnHeldUp() override;
    void OnHeldDown() override;
    void OnHeldLeft() override;
    void OnHeldRight() override;
};