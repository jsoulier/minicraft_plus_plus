#include <savepoint/savepoint.hpp>

#include <cstdint>

#include "../color.hpp"
#include "../entity.hpp"
#include "../input.hpp"
#include "../inventory.hpp"
#include "../item.hpp"
#include "../log.hpp"
#include "../menu.hpp"
#include "../renderer.hpp"
#include "../tile.hpp"
#include "../world.hpp"
#include "controller/player.hpp"
#include "item.hpp"
#include "player.hpp"

MppPlayerEntity::MppPlayerEntity()
    : MppHumanoidEntity()
    , Crouching{false}
{
}

void MppPlayerEntity::OnAddEntity()
{
    MppHumanoidEntity::OnAddEntity();
    SetX1(0);
    SetX2(256);
    SetY1(128);
    SetY2(144);
    Inventory->SetIsFocused(true);
    Inventory->SetX1(4);
    Inventory->SetY1(4);
    Inventory->SetX2(126);
    Inventory->SetY2(124);
}

void MppPlayerEntity::Visit(SavepointVisitor& visitor)
{
    MppHumanoidEntity::Visit(visitor);
    visitor(Crouching);
}

void MppPlayerEntity::Update(uint64_t ticks)
{
    MppMobEntity::Update(ticks);
    MppRendererMove(X, Y, GetSize());
}

void MppPlayerEntity::Render() const
{
    MppHumanoidEntity::Render();
    MppMenu::Render();
    int health = std::ceil(float(GetHealth()) / 10);
    int hunger = std::ceil(float(GetHunger()) / 10);
    int energy = std::ceil(float(GetEnergy()) / 10);
    for (int i = 0; i < health; i++)
    {
        int x = i * MppItem::kSize;
        int y = 128;
        MppItem{MppItemIDHeart}.Render(x, y, MppRendererLayerMenuContent);
    }
    for (int i = 0; i < hunger; i++)
    {
        int x = 176 + i * MppItem::kSize;
        int y = 136;
        MppItem{MppItemIDFood}.Render(x, y, MppRendererLayerMenuContent);
    }
    for (int i = 0; i < energy; i++)
    {
        int x = i * MppItem::kSize;
        int y = 136;
        MppItem{MppItemIDEnergy}.Render(x, y, MppRendererLayerMenuContent);
    }
    const MppItem& held = Inventory->GetBySlot(MppInventorySlotHeld);
    if (HeldEntity)
    {
        MppMenu::Render(HeldEntity->GetName(), kMppColorMenuUnlocked, 256, 128, MppMenuAlignmentRight);
    }
    else if (held.IsValid())
    {
        MppMenu::Render(held.GetName(), kMppColorMenuUnlocked, 256, 128, MppMenuAlignmentRight);
    }
}

void MppPlayerEntity::SetCrouching(bool crouching)
{
    Crouching = crouching;
}

int MppPlayerEntity::GetSpeed() const
{
    if (Crouching)
    {
        return 3;
    }
    else
    {
        return 1;
    }
}

std::shared_ptr<MppController> MppPlayerEntity::GetController() 
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
