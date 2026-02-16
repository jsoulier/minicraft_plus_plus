#include <savepoint/savepoint.hpp>

#include <cstdint>
#include <memory>

#include <minicraft++/assert.hpp>
#include <minicraft++/entity/controller/player.hpp>
#include <minicraft++/entity/furniture/furniture.hpp>
#include <minicraft++/entity/mob/humanoid.hpp>
#include <minicraft++/entity/mob/mob.hpp>
#include <minicraft++/entity/mount.hpp>
#include <minicraft++/renderer.hpp>
#include <minicraft++/world.hpp>

void MppMountEntityProxy::OnAdd()
{
    if (Rider)
    {
        MppInputAddHandler(std::dynamic_pointer_cast<MppInputHandler>(shared_from_this()));
    }
}

void MppMountEntityProxy::Possess(const std::shared_ptr<MppMobEntity>& vehicle, const std::shared_ptr<MppEntity>& instigator)
{
    Vehicle = MppEntityReference{vehicle};
    VehicleController = vehicle->GetController();
    Rider = instigator->Cast<MppMobEntity>();
    MppAssert(Rider);
    std::shared_ptr<MppController> controller = Rider->GetController();
    controller->Possess(vehicle);
    Rider->Unspawn();
    MppAssert(vehicle->GetController());
    MppAssert(!Rider->GetController());
    if (GetInputHandler())
    {
        MppInputAddHandler(std::dynamic_pointer_cast<MppInputHandler>(shared_from_this()));
    }
    std::shared_ptr<MppHumanoidEntity> humanoid = GetRiderAsHumanoid();
    if (humanoid)
    {
        humanoid->SetRiding(true);
    }
    Rider->RequestAnimationTick();
}

void MppMountEntityProxy::Unpossess()
{
    MppAssert(Rider);
    MppAssert(!Rider->GetController());
    if (GetInputHandler())
    {
        MppInputRemoveHandler(this);
    }
    std::shared_ptr<MppHumanoidEntity> humanoid = GetRiderAsHumanoid();
    if (humanoid)
    {
        humanoid->SetRiding(false);
    }
    Rider->RequestAnimationTick();
    std::shared_ptr<MppMobEntity> vehicle = GetVehicle();
    std::shared_ptr<MppController> controller = vehicle->GetController();
    MppAssert(controller);
    controller->Possess(Rider);
    VehicleController->Possess(vehicle);
    std::shared_ptr<MppEntity> rider = Rider->Cast<MppEntity>();
    MppWorldAddEntity(rider);
    Vehicle = MppEntityReference{};
    VehicleController = nullptr;
    Rider = nullptr;
}

void MppMountEntityProxy::Visit(SavepointVisitor& visitor)
{
    visitor(Vehicle);
    visitor(VehicleController);
    visitor(Rider);
    if (visitor.IsReading() && Rider)
    {
        Rider->OnCreate();
    }
}

void MppMountEntityProxy::Update(uint64_t ticks)
{
    if (!Rider)
    {
        return;
    }
    std::shared_ptr<MppMobEntity> vehicle = GetVehicle();
    int offsetX = 0;
    int offsetY = 0;
    if (vehicle->GetFacingX())
    {
        offsetX = -vehicle->GetFacingX() * 4;
        offsetY = -2;
    }
    else if (vehicle->GetFacingY() == -1)
    {
        offsetY = 0;
    }
    else if (vehicle->GetFacingY() == 1)
    {
        offsetY = -4;
    }
    Rider->SetX(vehicle->GetX() + offsetX);
    Rider->SetY(vehicle->GetY() + offsetY);
    if (!Rider->IsFacing(vehicle->GetFacingX(), vehicle->GetFacingY()))
    {
        Rider->SetFacingX(vehicle->GetFacingX());
        Rider->SetFacingY(vehicle->GetFacingY());
    }
    Rider->Update(ticks);
}

void MppMountEntityProxy::Render() const
{
    if (Rider)
    {
        MppRendererLayerOverride override1{MppRendererLayerEntity, MppRendererLayerEntity2};
        MppRendererLayerOverride override2{MppRendererLayerTopEntity, MppRendererLayerTopEntity2};
        Rider->Render();
    }
}

void MppMountEntityProxy::DoAction()
{
    if (Rider)
    {
        Rider->DoAction();
    }
}

void MppMountEntityProxy::OnInputRender() const
{
    // No-op. All registered input handlers are rendered
}

void MppMountEntityProxy::OnInputAction()
{
    MppAssert(Rider);
    if (std::shared_ptr<MppInputHandler> controller = GetInputHandler())
    {
        controller->OnInputAction();
    }
}

void MppMountEntityProxy::OnInputInteract()
{
    MppAssert(Rider);
    if (std::shared_ptr<MppInputHandler> handler = GetInputHandler())
    {
        handler->OnInputInteract();
    }
}

void MppMountEntityProxy::OnInputDismount()
{
    Unpossess();
}

void MppMountEntityProxy::OnInputExit()
{
    // No-op. Must be manually dismounted 
}

void MppMountEntityProxy::OnInputHeldUp()
{
    MppAssert(Rider);
    if (std::shared_ptr<MppInputHandler> handler = GetInputHandler())
    {
        handler->OnInputHeldUp();
    }
}

void MppMountEntityProxy::OnInputHeldDown()
{
    MppAssert(Rider);
    if (std::shared_ptr<MppInputHandler> handler = GetInputHandler())
    {
        handler->OnInputHeldDown();
    }
}

void MppMountEntityProxy::OnInputHeldLeft()
{
    MppAssert(Rider);
    if (std::shared_ptr<MppInputHandler> handler = GetInputHandler())
    {
        handler->OnInputHeldLeft();
    }
}

void MppMountEntityProxy::OnInputHeldRight()
{
    MppAssert(Rider);
    if (std::shared_ptr<MppInputHandler> handler = GetInputHandler())
    {
        handler->OnInputHeldRight();
    }
}

std::shared_ptr<MppMobEntity> MppMountEntityProxy::GetVehicle()
{
    return Vehicle.GetEntity()->Cast<MppMobEntity>();
}

std::shared_ptr<MppInputHandler> MppMountEntityProxy::GetInputHandler() 
{
    return std::dynamic_pointer_cast<MppInputHandler>(GetVehicle()->GetController());
}

std::shared_ptr<MppHumanoidEntity> MppMountEntityProxy::GetRiderAsHumanoid()
{
    return Rider->Cast<MppHumanoidEntity>();
}
