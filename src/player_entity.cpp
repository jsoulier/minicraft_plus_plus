#include <memory>

#include "player_controller.hpp"
#include "player_entity.hpp"

MppPlayerEntity::MppPlayerEntity()
    : MppHumanoidEntity()
{
    SetController(std::make_shared<MppPlayerController>());
}