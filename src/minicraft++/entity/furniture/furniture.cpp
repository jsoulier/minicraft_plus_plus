#include <savepoint/savepoint.hpp>

#include <cstdint>

#include <minicraft++/assert.hpp>
#include <minicraft++/color.hpp>
#include <minicraft++/console.hpp>
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
        Move(dx, dy);
        // If still colliding with instigator, instigator is inside the furniture
        if (IsColliding(instigator))
        {
            X = x;
            Y = y;
        }
    }
    return MppEntityCollisionAccept;
}

bool MppFurnitureEntity::Drop(const std::shared_ptr<MppEntity>& instigator)
{
    std::shared_ptr<MppHumanoidEntity> humanoid = instigator->Cast<MppHumanoidEntity>();
    MppAssert(humanoid);
    int x = X;
    int y = Y;
    X = humanoid->GetX() + humanoid->GetFacingX() * humanoid->GetSize();
    Y = humanoid->GetY() + humanoid->GetFacingY() * humanoid->GetSize();
    if (MppConsole::CVarAction.GetBool())
    {
        int px = GetPhysicsX();
        int py = GetPhysicsY();
        int pw = GetPhysicsWidth();
        int ph = GetPhysicsHeight();
        MppRendererDrawRect(kMppColorDebugAction, px, py, pw, ph, MppRendererLayerDebugAction);
    }
    if (IsColliding())
    {
        X = x;
        Y = y;
        return false;
    }
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
