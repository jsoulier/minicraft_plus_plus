#include <memory>

#include "color.hpp"
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

int MppPlayerEntity::GetSpriteBorderColor() const
{
    return 0;
}

int MppPlayerEntity::GetSpriteSkinColor() const
{
    return 964;
}

int MppPlayerEntity::GetSpriteShirtColor() const
{
    return 900;
}

int MppPlayerEntity::GetSpritePantColor() const
{
    return 9;
}
