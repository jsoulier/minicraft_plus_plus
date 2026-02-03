#include <savepoint/savepoint.hpp>

#include <cstdint>

#include "../color.hpp"
#include "../entity.hpp"
#include "../input.hpp"
#include "../inventory.hpp"
#include "../item.hpp"
#include "../menu.hpp"
#include "../renderer.hpp"
#include "../tile.hpp"
#include "../world.hpp"
#include "controller/player.hpp"
#include "item.hpp"
#include "player.hpp"

MppPlayerEntity::MppPlayerEntity()
    : MppHumanoidEntity()
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

void MppPlayerEntity::PostUpdate(uint64_t ticks)
{
    MppHumanoidEntity::PostUpdate(ticks);
    MppRendererMove(X, Y, GetSize());
}

void MppPlayerEntity::Render() const
{
    MppHumanoidEntity::Render();
    MppMenu::Render();
    int health = std::ceil(float(Health) / 10);
    int hunger = std::ceil(float(Hunger) / 10);
    int energy = std::ceil(float(Energy) / 10);
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
    if (held.IsValid())
    {
        MppMenu::Render(held.GetName(), kMppColorMenuUnlocked, 256, 128, MppMenuAlignmentRight);
    }
}

std::shared_ptr<MppController> MppPlayerEntity::GetController() 
{
    return std::make_shared<MppPlayerController>();
}

int MppPlayerEntity::GetSpriteBorderColor() const
{
    return 0;
}

int MppPlayerEntity::GetSpriteSkinColor() const
{
    return 532;
}

int MppPlayerEntity::GetSpriteShirtColor() const
{
    return 500;
}

int MppPlayerEntity::GetSpritePantColor() const
{
    return 5;
}

int MppPlayerEntity::GetSpriteShoeColor() const
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
