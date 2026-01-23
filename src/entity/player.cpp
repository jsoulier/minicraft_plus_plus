#include <memory>

#include "../input.hpp"
#include "../inventory.hpp"
#include "../renderer.hpp"
#include "player.hpp"

void MppPlayerEntity::OnAddEntity()
{
    MppHumanoidEntity::OnAddEntity();
    MppInputSetPlayer(std::dynamic_pointer_cast<MppInputHandler>(shared_from_this()));
    GetInventory()->SetX2(120);
    GetInventory()->SetY2(120);
}

void MppPlayerEntity::Update(uint64_t ticks)
{
    MppHumanoidEntity::Update(ticks);
    MppRendererMove(X, Y, GetSize());
}

void MppPlayerEntity::Render() const
{
    MppHumanoidEntity::Render();
}

void MppPlayerEntity::OnAction()
{
}

void MppPlayerEntity::OnInventory()
{
    MppInputSetInteraction(Inventory);
}

void MppPlayerEntity::OnHeldUp()
{
    VelocityY--;
}

void MppPlayerEntity::OnHeldDown()
{
    VelocityY++;
}

void MppPlayerEntity::OnHeldLeft()
{
    VelocityX--;
}

void MppPlayerEntity::OnHeldRight()
{
    VelocityX++;
}
