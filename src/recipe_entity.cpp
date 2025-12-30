#include "recipe_entity.hpp"
#include "physics.hpp"

MppPhysicsType MppRecipeEntity::GetPhysicsType() const
{
    return MppPhysicsType(MppPhysicsTypeRecipeEntity & MppFurnitureEntity::GetPhysicsType());
}
