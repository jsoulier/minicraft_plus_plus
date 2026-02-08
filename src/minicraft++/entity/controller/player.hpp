#pragma once

#include <cstdint>

#include <minicraft++/input.hpp>
#include <minicraft++/entity/controller/controller.hpp>

class MppPlayerEntity;

class MppPlayerController final
    : public MppController
    , public MppInputHandler
{
    SAVEPOINT_POLY(MppPlayerController)

public:
    MppPlayerController() = default;
    void OnAddEntity() override;
    void OnUpdate(uint64_t ticks) override;
    void OnAction() override;
    void OnInventory() override;
    void OnHeldUp() override;
    void OnHeldDown() override;
    void OnHeldLeft() override;
    void OnHeldRight() override;
    void OnHeldCrouch() override;
    bool ActionFilter(const std::shared_ptr<MppEntity>& entity) const override;

private:
    void OnActionCallback(int index);
    void OnDropCallback(int index);

private:
    std::shared_ptr<MppPlayerEntity> GetPlayer();
};