#pragma once

#include <cstdint>

#include <minicraft++/input.hpp>
#include <minicraft++/entity/controller/controller.hpp>

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
    void OnInteract() override;
    void OnExit() override;
    void OnHeldUp() override;
    void OnHeldDown() override;
    void OnHeldLeft() override;
    void OnHeldRight() override;
    bool ActionFilter(const std::shared_ptr<MppEntity>& entity) const override;

private:
    void OnActionCallback(int index);
    void OnDropCallback(int index);
};