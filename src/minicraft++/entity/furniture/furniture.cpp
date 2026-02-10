#include <savepoint/savepoint.hpp>

#include <cstdint>

#include <minicraft++/assert.hpp>
#include <minicraft++/entity/furniture/furniture.hpp>
#include <minicraft++/entity/mob/humanoid.hpp>
#include <minicraft++/entity/mob/mob.hpp>
#include <minicraft++/renderer.hpp>

MppFurnitureEntity::MppFurnitureEntity()
    : MppEntity()
{
}

void MppFurnitureEntity::Update(uint64_t ticks)
{
    MppEntity::Update(ticks);
}

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
        MppRendererModNone,
        MppRendererLayerEntity);
}

bool MppFurnitureEntity::OnAction(MppEntity& instigator)
{
    MppHumanoidEntity* humanoid = dynamic_cast<MppHumanoidEntity*>(&instigator);
    MppAssert(humanoid);
    humanoid->HoldEntity(shared_from_this());
    return true;
}

bool MppFurnitureEntity::OnCollision(MppEntity& instigator, int dx, int dy)
{
    MppMobEntity* mob = dynamic_cast<MppMobEntity*>(&instigator);
    MppFurnitureEntity* furniture = dynamic_cast<MppFurnitureEntity*>(&instigator);
    if (mob || furniture)
    {
        Move(dx, dy);
    }
    return true;
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
