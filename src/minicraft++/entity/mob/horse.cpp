#include <savepoint/savepoint.hpp>

#include <cstdint>
#include <memory>

#include <minicraft++/assert.hpp>
#include <minicraft++/entity/controller/controller.hpp>
#include <minicraft++/entity/mob/creature.hpp>
#include <minicraft++/entity/mob/horse.hpp>
#include <minicraft++/entity/mob/mob.hpp>
#include <minicraft++/entity/mob/mount.hpp>

MppHorseEntity::MppHorseEntity()
    : MppCreatureEntity()
    , Mount{std::make_shared<MppMobEntityMount>()}
{
}

void MppHorseEntity::OnAdd()
{
    MppCreatureEntity::OnAdd();
    Mount->OnAdd();
}

void MppHorseEntity::Visit(SavepointVisitor& visitor)
{
    MppCreatureEntity::Visit(visitor);
    visitor(Mount);
}

void MppHorseEntity::Update(uint64_t ticks)
{
    MppCreatureEntity::Update(ticks);
    Mount->Update(ticks);
}

void MppHorseEntity::Render() const
{
    MppCreatureEntity::Render();
    Mount->Render();
}

bool MppHorseEntity::OnInteraction(std::shared_ptr<MppEntity>& instigator)
{
    Mount->Possess(Cast<MppMobEntity>(), instigator);
    return true;
}

void MppHorseEntity::DoAction()
{
    Mount->DoAction();
}

int MppHorseEntity::GetAnimationPose1X() const
{
    return 12;
}

int MppHorseEntity::GetAnimationPose1Y() const
{
    return 8;
}

int MppHorseEntity::GetColor1() const
{
    return 0;
}

int MppHorseEntity::GetColor2() const
{
    return 210;
}

int MppHorseEntity::GetColor3() const
{
    return 111;
}

int MppHorseEntity::GetColor4() const
{
    return 433;
}

int MppHorseEntity::GetColor5() const
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
    return 2;
}

int MppHorseEntity::GetMoveTickRate() const
{
    return 1;
}
