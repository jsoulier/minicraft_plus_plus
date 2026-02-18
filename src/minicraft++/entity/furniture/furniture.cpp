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

void MppFurnitureEntity::OnCreate()
{
    MppEntity::OnCreate();
    Animation.SetTickRate(GetAnimationTickRate());
    int poses[4][2];
    poses[0][0] = GetAnimationPose1X();
    poses[0][1] = GetAnimationPose1Y();
    poses[1][0] = GetAnimationPose2X();
    poses[1][1] = GetAnimationPose2Y();
    poses[2][0] = GetAnimationPose3X();
    poses[2][1] = GetAnimationPose3Y();
    poses[3][0] = GetAnimationPose4X();
    poses[3][1] = GetAnimationPose4Y();
    for (int i = 0; i < 4; i++)
    {
        if (poses[i][0] == -1)
        {
            MppAssert(poses[i][1] == -1);
            break;
        }
        Animation.SetPose(i, poses[i][0], poses[i][1]);
    }
}

void MppFurnitureEntity::Update(uint64_t ticks)
{
    MppEntity::Update(ticks);
    Animation.Update(ticks);
}

void MppFurnitureEntity::Render() const
{
    MppEntity::Render();
    MppRendererDraw(
        MppSprite{
            GetColor1(),
            GetColor2(),
            GetColor3(),
            GetColor4(),
            GetColor5(),
            Animation.GetX(),
            Animation.GetY(),
            GetSize(),
        },
        X,
        Y,
        Animation.GetMod(),
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
    X = humanoid->GetX() + humanoid->GetFacingX() * (humanoid->GetPhysicsWidth() + 1);
    Y = humanoid->GetY() + humanoid->GetFacingY() * (humanoid->GetPhysicsHeight() + 1);
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

int MppFurnitureEntity::GetAnimationTickRate() const
{
    return 10;
}

int MppFurnitureEntity::GetAnimationPose2X() const
{
    return -1;
}

int MppFurnitureEntity::GetAnimationPose2Y() const
{
    return -1;
}

int MppFurnitureEntity::GetAnimationPose3X() const
{
    return -1;
}

int MppFurnitureEntity::GetAnimationPose3Y() const
{
    return -1;
}

int MppFurnitureEntity::GetAnimationPose4X() const
{
    return -1;
}

int MppFurnitureEntity::GetAnimationPose4Y() const
{
    return -1;
}

