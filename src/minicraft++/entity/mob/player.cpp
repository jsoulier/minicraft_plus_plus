#include <savepoint/savepoint.hpp>

#include <cmath>
#include <cstdint>

#include <minicraft++/color.hpp>
#include <minicraft++/entity/controller/player.hpp>
#include <minicraft++/entity/entity.hpp>
#include <minicraft++/entity/mob/player.hpp>
#include <minicraft++/input.hpp>
#include <minicraft++/inventory.hpp>
#include <minicraft++/item.hpp>
#include <minicraft++/log.hpp>
#include <minicraft++/menu.hpp>
#include <minicraft++/renderer.hpp>
#include <minicraft++/tile.hpp>
#include <minicraft++/world.hpp>

MppPlayerEntity::MppPlayerEntity()
    : MppHumanoidEntity()
{
}

void MppPlayerEntity::OnAdd()
{
    MppHumanoidEntity::OnAdd();
}

void MppPlayerEntity::Visit(SavepointVisitor& visitor)
{
    MppHumanoidEntity::Visit(visitor);
}

void MppPlayerEntity::Update(uint64_t ticks)
{
    MppMobEntity::Update(ticks);
}

void MppPlayerEntity::Render() const
{
    MppHumanoidEntity::Render();
}

int MppPlayerEntity::GetMoveTickRate() const
{
    return 1;
}

std::shared_ptr<MppController> MppPlayerEntity::GetDefaultController() 
{
    return std::make_shared<MppPlayerController>();
}

int MppPlayerEntity::GetSpriteColor1() const
{
    return 0;
}

int MppPlayerEntity::GetSpriteColor2() const
{
    return 500;
}

int MppPlayerEntity::GetSpriteColor3() const
{
    return 5;
}

int MppPlayerEntity::GetSpriteColor4() const
{
    return 532;
}

int MppPlayerEntity::GetSpriteColor5() const
{
    return 211;
}

int MppPlayerEntity::GetMaxHealth() const
{
    return 100;
}

int MppPlayerEntity::GetMaxHunger() const
{
    return 100;
}

int MppPlayerEntity::GetMaxEnergy() const
{
    return 100;
}
