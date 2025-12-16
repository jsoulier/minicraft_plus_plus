#include <memory>

#include "player_controller.hpp"
#include "player_entity.hpp"
#include "renderer.hpp"

MppPlayerEntity::MppPlayerEntity()
    : MppHumanoidEntity()
{
    SetController(std::make_shared<MppPlayerController>(*this));
}

void MppPlayerEntity::Update(MppLevel& level, MppRenderer& renderer, int ticks)
{
    MppHumanoidEntity::Update(level, renderer, ticks);
    renderer.SetCamera(X, Y, GetSize());
}