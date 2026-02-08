#include <savepoint/savepoint.hpp>

#include "../renderer.hpp"
#include "furniture.hpp"
#include "humanoid.hpp"

void MppFurnitureEntity::Render() const
{
    MppEntity::Render();
    MppRendererDraw(MppSprite{
            GetColor1(),
            GetColor2(),
            GetColor3(),
            GetColor4(),
            GetColor5(),
            GetSpriteX(),
            GetSpriteY(),
            GetSize(),
        },
        X,
        Y,
        false,
        MppRendererLayerEntity);
}

void MppFurnitureEntity::OnAction(MppEntity& instigator)
{
    MppHumanoidEntity* humanoid = dynamic_cast<MppHumanoidEntity*>(&instigator);
    if (humanoid)
    {
        humanoid->HoldEntity(shared_from_this());
    }
}

bool MppFurnitureEntity::OnInteraction(MppEntity& instigator)
{
    return false;
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

bool MppFurnitureEntity::IsEmpty() const
{
    return true;
}
