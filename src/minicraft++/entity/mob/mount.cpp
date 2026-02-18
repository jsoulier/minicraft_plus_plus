#include <savepoint/savepoint.hpp>

#include <cstdint>
#include <memory>

#include <minicraft++/assert.hpp>
#include <minicraft++/entity/controller/player.hpp>
#include <minicraft++/entity/furniture/furniture.hpp>
#include <minicraft++/entity/mob/mob.hpp>
#include <minicraft++/entity/mob/mount.hpp>
#include <minicraft++/renderer.hpp>
#include <minicraft++/world.hpp>

void MppMobEntityMount::OnAdd()
{
    if (Rider)
    {
        MppInputAddHandler(std::dynamic_pointer_cast<MppInputHandler>(shared_from_this()));
    }
}

void MppMobEntityMount::Possess(const std::shared_ptr<MppMobEntity>& vehicle, const std::shared_ptr<MppEntity>& instigator)
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
    Rider->OnMount(vehicle);
}

void MppMobEntityMount::Unpossess()
{
    MppAssert(Rider);
    MppAssert(!Rider->GetController());
    if (GetInputHandler())
    {
        MppInputRemoveHandler(this);
    }
    Rider->OnUnmount();
    std::shared_ptr<MppMobEntity> vehicle = GetVehicle();
    std::shared_ptr<MppController> controller = vehicle->GetController();
    MppAssert(controller);
    controller->Possess(Rider);
    if (VehicleController)
    {
        VehicleController->Possess(vehicle);
    }
    std::shared_ptr<MppEntity> rider = Rider->Cast<MppEntity>();
    MppWorldAddEntity(rider);
    Vehicle = MppEntityReference{};
    VehicleController = nullptr;
    Rider = nullptr;
}

void MppMobEntityMount::Visit(SavepointVisitor& visitor)
{
    visitor(Vehicle);
    visitor(VehicleController);
    visitor(Rider);
    if (visitor.IsReading() && Rider)
    {
        Rider->OnCreate();
    }
}

void MppMobEntityMount::Update(uint64_t ticks)
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
        offsetX = -vehicle->GetFacingX() * 2;
        offsetY = -4;
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

void MppMobEntityMount::Render() const
{
    if (Rider)
    {
        MppRendererLayerOverride override1{MppRendererLayerEntity, MppRendererLayerEntity2};
        MppRendererLayerOverride override2{MppRendererLayerTopEntity, MppRendererLayerTopEntity2};
        Rider->Render();
    }
}

void MppMobEntityMount::DoAction()
{
    if (Rider)
    {
        Rider->DoAction();
    }
}

void MppMobEntityMount::OnInputRender() const
{
    // No-op. All registered input handlers are rendered
}

void MppMobEntityMount::OnInputAction()
{
    MppAssert(Rider);
    if (std::shared_ptr<MppInputHandler> controller = GetInputHandler())
    {
        controller->OnInputAction();
    }
}

void MppMobEntityMount::OnInputInteract()
{
    MppAssert(Rider);
    if (std::shared_ptr<MppInputHandler> handler = GetInputHandler())
    {
        handler->OnInputInteract();
    }
}

void MppMobEntityMount::OnInputDismount()
{
    Unpossess();
}

void MppMobEntityMount::OnInputExit()
{
    // No-op. Must be manually dismounted 
}

void MppMobEntityMount::OnInputHeldUp()
{
    MppAssert(Rider);
    if (std::shared_ptr<MppInputHandler> handler = GetInputHandler())
    {
        handler->OnInputHeldUp();
    }
}

void MppMobEntityMount::OnInputHeldDown()
{
    MppAssert(Rider);
    if (std::shared_ptr<MppInputHandler> handler = GetInputHandler())
    {
        handler->OnInputHeldDown();
    }
}

void MppMobEntityMount::OnInputHeldLeft()
{
    MppAssert(Rider);
    if (std::shared_ptr<MppInputHandler> handler = GetInputHandler())
    {
        handler->OnInputHeldLeft();
    }
}

void MppMobEntityMount::OnInputHeldRight()
{
    MppAssert(Rider);
    if (std::shared_ptr<MppInputHandler> handler = GetInputHandler())
    {
        handler->OnInputHeldRight();
    }
}

std::shared_ptr<MppMobEntity> MppMobEntityMount::GetRider()
{
    return Rider;
}

std::shared_ptr<MppMobEntity> MppMobEntityMount::GetVehicle()
{
    return Vehicle.GetEntity()->Cast<MppMobEntity>();
}

std::shared_ptr<MppInputHandler> MppMobEntityMount::GetInputHandler() 
{
    return std::dynamic_pointer_cast<MppInputHandler>(GetVehicle()->GetController());
}
