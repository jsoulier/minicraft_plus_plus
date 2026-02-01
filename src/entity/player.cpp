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
    Inventory->SetIsFocused(true);
    Inventory->SetX2(124);
    Inventory->SetY2(120);
}

void MppPlayerEntity::OnAddEntity()
{
    MppHumanoidEntity::OnAddEntity();
    SetX1(0);
    SetX2(256);
    SetY1(128);
    SetY2(144);
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
        int y = 128;
        MppItem{MppItemIDFood}.Render(x, y, MppRendererLayerMenuContent);
    }
    for (int i = 0; i < energy; i++)
    {
        int x = i * MppItem::kSize;
        int y = 136;
        MppItem{MppItemIDEnergy}.Render(x, y, MppRendererLayerMenuContent);
    }
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
