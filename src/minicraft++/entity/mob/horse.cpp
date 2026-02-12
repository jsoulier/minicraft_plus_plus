// TODO: all this shit needs a massive refactor if I want mountable boats/minecarts. i don't wanna duplicate this

#include <savepoint/savepoint.hpp>

#include <memory>

#include <minicraft++/assert.hpp>
#include <minicraft++/entity/controller/mount.hpp>
#include <minicraft++/entity/mob/creature.hpp>
#include <minicraft++/entity/mob/horse.hpp>
#include <minicraft++/entity/mob/mob.hpp>

MppHorseEntity::MppHorseEntity()
    : MppCreatureEntity()
{
}

void MppHorseEntity::Visit(SavepointVisitor& visitor)
{
    MppCreatureEntity::Visit(visitor);
    visitor(MountController);
}

void MppHorseEntity::OnAdd()
{
    MppMobEntity::OnAdd();
    // TODO: refactor
    if (MountController)
    {
        // TODO: can this be added before the player controller? that would be bad
        MppInputAddHandler(MountController);
        MountController->SetEntityContext(std::dynamic_pointer_cast<MppMobEntity>(shared_from_this()));
    }
}

void MppHorseEntity::Update(uint64_t ticks)
{
    MppCreatureEntity::Update(ticks);
    if (MountController)
    {
        MountController->Update(ticks);
    }
}

void MppHorseEntity::Render() const
{
    MppCreatureEntity::Render();
    if (MountController)
    {
        MountController->RenderFromEntity();
    }
}

void MppHorseEntity::OnUnpossess()
{
    MppCreatureEntity::OnUnpossess();
    MountController = nullptr;
    // TODO: refactor
    std::shared_ptr<MppController> controller = GetDefaultController();
    controller->Possess(std::dynamic_pointer_cast<MppMobEntity>(shared_from_this()));
}

bool MppHorseEntity::OnInteraction(MppEntity& instigator)
{
    MppMobEntity* mob = dynamic_cast<MppMobEntity*>(&instigator);
    MppAssert(mob);
    MountController = std::make_shared<MppMountController>(std::dynamic_pointer_cast<MppMobEntity>(mob->shared_from_this()));
    MountController->Possess(std::dynamic_pointer_cast<MppMobEntity>(shared_from_this()));
    return true;
}

void MppHorseEntity::DoAction()
{
    // When mounted, forward actions to the rider
    if (MountController)
    {
        // TODO: This doesn't work because the Rider doesn't have a controller
        // I'm wondering if the Rider needs a PuppetController or something so that
        // they can perform basic actions. Right now, the controller is only used for the
        // action filter. Maybe we can make the ActionFilter a static function and get the
        // function pointer to that in the PuppetController. Basically just simulate the previous.
        // We'll need this for skeleton riders
        MountController->GetRider()->DoAction();
    }
}

int MppHorseEntity::GetSpritePose1X() const
{
    return 12;
}

int MppHorseEntity::GetSpritePose1Y() const
{
    return 8;
}

int MppHorseEntity::GetSpriteColor1() const
{
    return 0;
}

int MppHorseEntity::GetSpriteColor2() const
{
    return 210;
}

int MppHorseEntity::GetSpriteColor3() const
{
    return 111;
}

int MppHorseEntity::GetSpriteColor4() const
{
    return 433;
}

int MppHorseEntity::GetSpriteColor5() const
{
    return 0;
}

int MppHorseEntity::GetPhysicsOffsetX() const
{
    return 0;
}

int MppHorseEntity::GetPhysicsOffsetY() const
{
    return 0;
}

int MppHorseEntity::GetPhysicsWidth() const
{
    return 16;
}

int MppHorseEntity::GetPhysicsHeight() const
{
    return 16;
}

int MppHorseEntity::GetMaxHealth() const
{
    return 1;
}

int MppHorseEntity::GetMaxHunger() const
{
    return 1;
}

int MppHorseEntity::GetMaxEnergy() const
{
    return 1;
}

int MppHorseEntity::GetSpeed() const
{
    // TODO: should only go fast when we're mounted
    return 2;
}

int MppHorseEntity::GetMoveTickRate() const
{
    return 1;
}
