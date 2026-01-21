#include "furniture.hpp"

void MppFurnitureEntity::Render()
{
    MppEntity::Render();
}

int MppFurnitureEntity::GetPhysicsOffsetX() const
{
    return 1;
}

int MppFurnitureEntity::GetPhysicsOffsetY() const
{
    return 1;
}

int MppFurnitureEntity::GetPhysicsWidth() const
{
    return 14;
}

int MppFurnitureEntity::GetPhysicsHeight() const
{
    return 14;
}

int MppFurnitureEntity::GetSize() const
{
    return 16;
}
