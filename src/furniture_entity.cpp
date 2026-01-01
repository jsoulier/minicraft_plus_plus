#include "furniture_entity.hpp"
#include "physics.hpp"
#include "renderer.hpp"

void MppFurnitureEntity::Update(MppLevel& level, MppRenderer& renderer, int ticks)
{
    Draw(renderer);
}

void MppFurnitureEntity::Draw(MppRenderer& renderer)
{
    renderer.Draw(
        MppSprite{
            GetColor1(),
            GetColor2(),
            GetColor3(),
            GetColor4(),
            GetColor5(),
            GetSpriteX(),
            GetSpriteY(),
            GetSize(),
        },
        GetX(),
        GetY(),
        false,
        MppRenderer::LayerFurnitureEntity);
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

MppPhysicsType MppFurnitureEntity::GetPhysicsType() const
{
    return MppPhysicsTypeFurnitureEntity;
}

bool MppFurnitureEntity::IsConvertibleToItem() const
{
    return true;
}

int MppFurnitureEntity::GetPhysicsOffsetX() const
{
    return 1;
}

int MppFurnitureEntity::GetPhysicsOffsetY() const
{
    return 1;
}
