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
    bool Possess(const std::shared_ptr<MppMobEntity>& entity) override;
    bool Unpossess() override;
    void Update(uint64_t ticks) override;
    void OnInputRender() const override;
    void Render() const override;
    void OnInputAction() override;
    void OnInputInteract() override;
    void OnInputExit() override;
    void OnInputHeldUp() override;
    void OnInputHeldDown() override;
    void OnInputHeldLeft() override;
    void OnInputHeldRight() override;
    void OnSetLevel(int level) override;
    bool ActionFilter(const std::shared_ptr<MppEntity>& entity) const override;
    bool InteractionFilter(const std::shared_ptr<MppEntity>& entity) const override;

private:
    void OnActionCallback(int index);
    void OnDropCallback(int index);
};