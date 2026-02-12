#pragma once

#include <savepoint/savepoint.hpp>

#include <minicraft++/input.hpp>
#include <minicraft++/entity/controller/controller.hpp>

class MppPlayerController;

class MppMountController final
    : public MppController
    , public MppInputHandler
{
    SAVEPOINT_POLY(MppMountController)

public:
    MppMountController() = default;
    MppMountController(const std::shared_ptr<MppMobEntity>& rider);
    void Possess(const std::shared_ptr<MppMobEntity>& entity) override;
    void Unpossess() override;
    void Visit(SavepointVisitor& visitor) override;
    void Update(uint64_t ticks) override;
    void RenderFromEntity() const override;
    void OnRender() const override;
    void OnAction() override;
    void OnInteract() override;
    void OnDismount() override;
    void OnExit() override;
    void OnHeldUp() override;
    void OnHeldDown() override;
    void OnHeldLeft() override;
    void OnHeldRight() override;
    bool ActionFilter(const std::shared_ptr<MppEntity>& entity) const override;
    std::shared_ptr<MppMobEntity> GetRider();
    
    // TOOD: refactor
    void SetEntityContext(const std::shared_ptr<MppMobEntity>& entity) { Entity = entity; }

private:
    // TODO: Could be architected better. We have to forward to the player controller (if it exists)
    std::shared_ptr<MppPlayerController> GetPlayerController() const;

private:
    // The rider is removed from the world and has its controller stolen (given to the MppController::Entity).
    // It'll be given back the stolen controller when unpossessed
    std::shared_ptr<MppMobEntity> Rider;
};
