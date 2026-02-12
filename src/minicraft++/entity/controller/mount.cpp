#include <savepoint/savepoint.hpp>

#include <minicraft++/assert.hpp>
#include <minicraft++/entity/controller/mount.hpp>
#include <minicraft++/entity/controller/player.hpp>
#include <minicraft++/entity/furniture/furniture.hpp>
#include <minicraft++/entity/mob/mob.hpp>
#include <minicraft++/world.hpp>

MppMountController::MppMountController(const std::shared_ptr<MppMobEntity>& rider)
    : Rider{rider}
{
}

void MppMountController::Possess(const std::shared_ptr<MppMobEntity>& entity)
{
    MppController::Possess(entity);
    // Take the rider's controller and possess the mounted creature
    // (e.g. take the player's player controller and possess the horse)
    std::shared_ptr<MppController> controller = Rider->GetController();
    controller->Unpossess();
    controller->Possess(entity);
    Rider->Kill();
    MppInputAddHandler(std::dynamic_pointer_cast<MppInputHandler>(shared_from_this()));
    MppAssert(entity->GetController());
    MppAssert(!Rider->GetController());
}

void MppMountController::Unpossess()
{
    MppAssert(!Rider->GetController());
    // Unpossess the mounted creature (e.g. horse) to give back the default controller
    std::shared_ptr<MppMobEntity> entity = Entity.lock();
    std::shared_ptr<MppController> controller = entity->GetController();
    MppAssert(controller);
    MppController::Unpossess();
    controller->Unpossess();
    // Unpossess the rider (e.g. player) to give back the default controller (player's player controller)
    // TODO: refactor
    Rider->OnPossess(controller);
    std::shared_ptr<MppEntity> rider = std::dynamic_pointer_cast<MppEntity>(Rider);
    MppWorldAddEntity(rider);
    MppInputRemoveHandler(this);
}

void MppMountController::Visit(SavepointVisitor& visitor)
{
    MppController::Visit(visitor);
    visitor(Rider);
    // TODO: refactor
    if (visitor.IsReading())
    {
        Rider->OnCreate();
    }
}

void MppMountController::Update(uint64_t ticks)
{
    MppController::Update(ticks);
    static constexpr int kOffsetX = 0;
    static constexpr int kOffsetY = -8;
    MppController::Update(ticks);
    std::shared_ptr<MppMobEntity> entity = Entity.lock();
    Rider->SetX(entity->GetX() + kOffsetX);
    Rider->SetY(entity->GetY() + kOffsetY);
    if (!Rider->IsFacing(entity->GetFacingX(), entity->GetFacingY()))
    {
        Rider->SetFacingX(entity->GetFacingX());
        Rider->SetFacingY(entity->GetFacingY());
    }
    Rider->Update(ticks);
}

void MppMountController::RenderFromEntity() const
{
    // Called from MppHorseEntity::Render
    // if (const std::shared_ptr<MppPlayerController> controller = GetPlayerController())
    // {
    //     controller->RenderFromEntity();
    // }
    // TODO: renderer layers
    Rider->Render();
}

void MppMountController::OnRender() const
{
    if (const std::shared_ptr<MppPlayerController> controller = GetPlayerController())
    {
        controller->OnRender();
    }
}

void MppMountController::OnAction()
{
    if (std::shared_ptr<MppPlayerController> controller = GetPlayerController())
    {
        controller->OnAction();
    }
}

void MppMountController::OnInteract()
{
    if (std::shared_ptr<MppPlayerController> controller = GetPlayerController())
    {
        controller->OnInteract();
    }
}

void MppMountController::OnDismount()
{
    Unpossess();
}

void MppMountController::OnExit()
{
    // No-op. Mount controller must be manually dismounted 
}

void MppMountController::OnHeldUp()
{
    if (std::shared_ptr<MppPlayerController> controller = GetPlayerController())
    {
        controller->OnHeldUp();
    }
}

void MppMountController::OnHeldDown()
{
    if (std::shared_ptr<MppPlayerController> controller = GetPlayerController())
    {
        controller->OnHeldDown();
    }
}

void MppMountController::OnHeldLeft()
{
    if (std::shared_ptr<MppPlayerController> controller = GetPlayerController())
    {
        controller->OnHeldLeft();
    }
}

void MppMountController::OnHeldRight()
{
    if (std::shared_ptr<MppPlayerController> controller = GetPlayerController())
    {
        controller->OnHeldRight();
    }
}

bool MppMountController::ActionFilter(const std::shared_ptr<MppEntity>& entity) const
{
    return
        !entity->IsA<MppMobEntity>() &&
        !entity->IsA<MppFurnitureEntity>() &&
        !(entity == Entity.lock()) &&
        // Needed to prevent DoAction from Entity on Rider
        !(entity == Rider);
}

std::shared_ptr<MppMobEntity> MppMountController::GetRider()
{
    return Rider;
}

std::shared_ptr<MppPlayerController> MppMountController::GetPlayerController() const
{
    return std::dynamic_pointer_cast<MppPlayerController>(Entity.lock()->GetController());
}
