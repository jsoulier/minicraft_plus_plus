#pragma once

#include <savepoint/savepoint.hpp>

#include <cstdint>
#include <memory>

#include <minicraft++/entity/entity.hpp>
#include <minicraft++/input.hpp>

class MppController;
class MppEntity;
class MppHumanoidEntity;
class MppMobEntity;

class MppMountEntityProxy final
    : public MppInputHandler
    , public SavepointPoly
    , public std::enable_shared_from_this<MppMountEntityProxy>
{
    SAVEPOINT_POLY(MppMountEntityProxy)

public:
    MppMountEntityProxy() = default;
    void OnAdd();
    void Possess(const std::shared_ptr<MppMobEntity>& vehicle, const std::shared_ptr<MppEntity>& instigator);
    void Unpossess();
    void Visit(SavepointVisitor& visitor) override;
    void Update(uint64_t ticks);
    void Render() const;
    void DoAction();
    void OnInputRender() const override;
    void OnInputAction() override;
    void OnInputInteract() override;
    void OnInputDismount() override;
    void OnInputExit() override;
    void OnInputHeldUp() override;
    void OnInputHeldDown() override;
    void OnInputHeldLeft() override;
    void OnInputHeldRight() override;

private:
    std::shared_ptr<MppMobEntity> GetVehicle();
    std::shared_ptr<MppInputHandler> GetInputHandler();
    std::shared_ptr<MppHumanoidEntity> GetRiderAsHumanoid();

private:
    MppEntityReference Vehicle;
    std::shared_ptr<MppController> VehicleController;
    std::shared_ptr<MppMobEntity> Rider;
};
