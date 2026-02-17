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

bool MppFurnitureEntity::OnAction(std::shared_ptr<MppEntity>& instigator)
{
    std::shared_ptr<MppHumanoidEntity> humanoid = instigator->Cast<MppHumanoidEntity>();
    MppAssert(humanoid);
    humanoid->PickupEntity(shared_from_this());
    return true;
}

MppEntityCollision MppFurnitureEntity::OnCollision(std::shared_ptr<MppEntity>& instigator, int dx, int dy)
{
    if (instigator->IsA<MppMobEntity>() || instigator->IsA<MppFurnitureEntity>())
    {
        int x = X;
        int y = Y;
        switch (Move(dx, dy))
        {
        case MppEntityCollisionAccept:
        case MppEntityCollisionTeleport:
            // If still colliding with instigator, instigator is inside the furniture
            if (IsColliding(instigator))
            {
                X = x;
                Y = y;
            }
            return MppEntityCollisionAccept;
        case MppEntityCollisionReject:
            return MppEntityCollisionReject;
        }
    }
    return MppEntityCollisionReject;
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
