#include "ai_controller.hpp"
#include "slime_entity.hpp"

MppSlimeEntity::MppSlimeEntity()
    : MppMobEntity()
{
    SetController(std::make_shared<MppAIController>(*this));
}