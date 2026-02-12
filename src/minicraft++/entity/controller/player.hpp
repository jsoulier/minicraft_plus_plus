#pragma once

#include <cstdint>

#include <minicraft++/input.hpp>
#include <minicraft++/menu.hpp>
#include <minicraft++/entity/controller/controller.hpp>

class MppPlayerController final
    : public MppController
    , public MppInputHandler
    , public MppMenu
{
    SAVEPOINT_POLY(MppPlayerController)

public:
    MppPlayerController() = default;
    void Possess(const std::shared_ptr<MppMobEntity>& entity) override;
    void Unpossess() override;
    void Update(uint64_t ticks) override;
    void OnRender() const override;
    void RenderFromEntity() const override;
    void OnAction() override;
    void OnInteract() override;
    void OnExit() override;
    void OnHeldUp() override;
    void OnHeldDown() override;
    void OnHeldLeft() override;
    void OnHeldRight() override;
    bool ActionFilter(const std::shared_ptr<MppEntity>& entity) const override;
    bool InteractionFilter(const std::shared_ptr<MppEntity>& entity) const override;

private:
    void OnActionCallback(int index);
    void OnDropCallback(int index);
};