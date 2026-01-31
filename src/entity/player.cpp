#include <savepoint/savepoint.hpp>

#include <cstdint>

#include "../color.hpp"
#include "../entity.hpp"
#include "../input.hpp"
#include "../inventory.hpp"
#include "../item.hpp"
#include "../renderer.hpp"
#include "../tile.hpp"
#include "../world.hpp"
#include "controller/player.hpp"
#include "item.hpp"
#include "player.hpp"

MppPlayerEntity::MppPlayerEntity()
    : MppHumanoidEntity()
{
    Inventory->SetIsFocused(true);
    Inventory->SetX2(124);
    Inventory->SetY2(136);
}

void MppPlayerEntity::OnAddEntity()
{
    MppHumanoidEntity::OnAddEntity();
}

void MppPlayerEntity::PostUpdate(uint64_t ticks)
{
    MppHumanoidEntity::PostUpdate(ticks);
    MppRendererMove(X, Y, GetSize());
}

void MppPlayerEntity::Render() const
{
    MppHumanoidEntity::Render();
}

std::shared_ptr<MppController> MppPlayerEntity::GetController() 
{
    return std::make_shared<MppPlayerController>();
}

int MppPlayerEntity::GetSpriteBorderColor() const
{
    return kMppColorPlayerBorder;
}

int MppPlayerEntity::GetSpriteSkinColor() const
{
    return kMppColorPlayerSkin;
}

int MppPlayerEntity::GetSpriteShirtColor() const
{
    return kMppColorPlayerShirt;
}

int MppPlayerEntity::GetSpritePantColor() const
{
    return kMppColorPlayerPants;
}

int MppPlayerEntity::GetSpriteShoeColor() const
{
    return kMppColorPlayerShoes;
}
