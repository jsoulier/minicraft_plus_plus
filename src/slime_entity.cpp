#include "ai_controller.hpp"
#include "slime_entity.hpp"

MppSlimeEntity::MppSlimeEntity()
    : MppMobEntity()
{
    SetController(std::make_shared<MppAIController>(*this));
}

int MppSlimeEntity::GetPhysicsWidth() const
{
    return 0;
}

int MppSlimeEntity::GetPhysicsHeight() const
{
    return 0;
}

int MppSlimeEntity::GetPhysicsOffsetX() const
{
    return 0;
}

int MppSlimeEntity::GetPhysicsOffsetY() const
{
    return 0;
}
