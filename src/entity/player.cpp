#include <memory>

#include "../input.hpp"
#include "../renderer.hpp"
#include "player.hpp"

void MppPlayerEntity::OnAddEntity()
{
    MppHumanoidEntity::OnAddEntity();
    MppInputSetPlayer(SharedFromThis<MppPlayerEntity>());
}

void MppPlayerEntity::Update(uint64_t ticks)
{
    MppHumanoidEntity::Update(ticks);
    MppRendererMove(X, Y, GetSize());
}

void MppPlayerEntity::Render()
{
    MppHumanoidEntity::Render();
}

void MppPlayerEntity::OnAction()
{
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
