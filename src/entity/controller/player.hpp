#pragma once

#include "../../input.hpp"
#include "../controller.hpp"

class MppPlayerEntity;

class MppPlayerController
    : public MppController
    , public MppInputHandler
{
    SAVEPOINT_POLY(MppPlayerController)

public:
    MppPlayerController() = default;
    void OnAddEntity() override;
    void OnAction() override;
    void OnInventory() override;
    void OnHeldUp() override;
    void OnHeldDown() override;
    void OnHeldLeft() override;
    void OnHeldRight() override;

private:
    std::shared_ptr<MppPlayerEntity> GetPlayer();
};