#include "furniture_entity.hpp"
#include "physics.hpp"

int MppFurnitureEntity::GetPhysicsWidth() const
{
    return 16;
}

int MppFurnitureEntity::GetPhysicsHeight() const
{
    return 16;
}

int MppFurnitureEntity::GetSize() const
{
    return 16;
}

MppPhysicsType MppFurnitureEntity::GetPhysicsType() const
{
    return MppPhysicsTypeFurnitureEntity;
}

int MppFurnitureEntity::GetPhysicsOffsetX() const
{
    return 0;
}

int MppFurnitureEntity::GetPhysicsOffsetY() const
{
    return 0;
}
