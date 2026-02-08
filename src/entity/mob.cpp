#include <savepoint/savepoint.hpp>

#include <cmath>
#include <limits>
#include <memory>
#include <numbers>

#include "../assert.hpp"
#include "../color.hpp"
#include "../console.hpp"
#include "../inventory.hpp"
#include "../renderer.hpp"
#include "../tile.hpp"
#include "../util.hpp"
#include "controller.hpp"
#include "mob.hpp"

MppMobEntity::MppMobEntity()
    : MppEntity()
    , Inventory{std::make_shared<MppInventory>()}
    , Health{-1}
    , Hunger{-1}
    , Energy{-1}
    , FacingX{0}
    , FacingY{1}
    , VelocityX{0}
    , VelocityY{0}
    , TickAnimation{false}
{
}

void MppMobEntity::OnAddEntity()
{
    MppEntity::OnAddEntity();
    Inventory->SetMaxItems(GetMaxItems());
    // Don't override if attained from visiting
    if (!Controller)
    {
        Controller = GetController();
    }
    if (Controller)
    {
        Controller->SetEntity(std::dynamic_pointer_cast<MppMobEntity>(shared_from_this()));
        Controller->OnAddEntity();
    }
    if (Health == -1)
    {
        MppAssert(Hunger == -1);
        MppAssert(Energy == -1);
        Health = GetMaxHealth();
        Hunger = GetMaxHunger();
        Energy = GetMaxEnergy();
        MppAssert(Health > 0);
        MppAssert(Hunger > 0);
        MppAssert(Energy > 0);
    }
    Animation.SetTickRate(GetSpriteTickRate());
    Animation.SetPose(0, GetSpritePose1X(), GetSpritePose1Y());
    Animation.SetPose(1, GetSpritePose2X(), GetSpritePose2Y());
}

void MppMobEntity::Visit(SavepointVisitor& visitor)
{
    MppEntity::Visit(visitor);
    visitor(Inventory);
    visitor(Controller);
    visitor(FacingX);
    visitor(FacingY);
    visitor(Health);
    visitor(Hunger);
    visitor(Energy);
}

void MppMobEntity::Update(uint64_t ticks)
{
    MppEntity::Update(ticks);
    if (Controller)
    {
        Controller->Update(ticks);
    }
    if (IsMoving() || TickAnimation)
    {
        Animation.Update(GetPose(), FacingX, FacingY, ticks);
        TickAnimation = false;
    }
    if (ticks % GetSpeed() == 0)
    {
        MppEntity::Move(VelocityX, VelocityY);
        if (VelocityX || VelocityY)
        {
            FacingX = VelocityX;
            FacingY = VelocityY;
        }
    }
    VelocityX = 0;
    VelocityY = 0;
}

void MppMobEntity::Render() const
{
    MppAssert(FacingX || FacingY);
    MppEntity::Render();
    MppRendererDraw(
        MppSprite{
            GetSpriteColor1(),
            GetSpriteColor2(),
            GetSpriteColor3(),
            GetSpriteColor4(),
            GetSpriteColor5(),
            Animation.GetX(),
            Animation.GetY(),
            GetSize(),
        },
        X,
        Y,
        Animation.GetFlip(),
        MppRendererLayerEntity);
    if (MppConsole::CVarFov.GetBool())
    {
        static constexpr float kLength = 32.0f;
        float angle = std::atan2(FacingY, FacingX);
        float minAngle = angle - GetFov() / 2.0f;
        float maxAngle = angle + GetFov() / 2.0f;
        int x = X + MppTile::kSize / 2;
        int y = Y + MppTile::kSize / 2;
        float minX = x + std::cos(minAngle) * kLength;
        float minY = y + std::sin(minAngle) * kLength;
        float midX = x + std::cos(angle) * kLength;
        float midY = y + std::sin(angle) * kLength;
        float maxX = x + std::cos(maxAngle) * kLength;
        float maxY = y + std::sin(maxAngle) * kLength;
        MppRendererDrawLine(kMppColorDebugFov, x, y, minX, minY, MppRendererLayerDebugFov);
        MppRendererDrawLine(kMppColorDebugFov, x, y, midX, midY, MppRendererLayerDebugFov);
        MppRendererDrawLine(kMppColorDebugFov, x, y, maxX, maxY, MppRendererLayerDebugFov);
    }
}

bool MppMobEntity::IsInFov(const std::shared_ptr<MppEntity>& entity)
{
    int x1 = this->X + MppTile::kSize / 2;
    int y1 = this->Y + MppTile::kSize / 2;
    int x2 = entity->GetX() + MppTile::kSize / 2;
    int y2 = entity->GetY() + MppTile::kSize / 2;
    MppAssert(FacingX || FacingY);
    float fx = FacingX;
    float fy = FacingY;
    float dx = x2 - x1;
    float dy = y2 - y1;
    float facing = std::sqrt(fx * fx + fy * fy);
    float target = std::sqrt(dx * dx + dy * dy);
    if (target <= std::numeric_limits<float>::epsilon())
    {
        return false;
    }
    fx /= facing;
    fy /= facing;
    dx /= target;
    dy /= target;
    float dot = fx * dx + fy * dy;
    return dot >= std::cos(GetFov() * 0.5f);
}

void MppMobEntity::Push(int dx, int dy)
{
    VelocityX += dx;
    VelocityY += dy;
}

void MppMobEntity::PushNow(int dx, int dy)
{
    X += dx;
    Y += dy;
}

int MppMobEntity::GetActionRange() const
{
    return 0;
}

int MppMobEntity::GetSize() const
{
    return 16;
}

std::shared_ptr<MppInventory> MppMobEntity::GetInventory()
{
    return Inventory;
}

int MppMobEntity::GetMaxItems() const
{
    return 0;
}

int MppMobEntity::GetSpeed() const
{
    return 1;
}

std::shared_ptr<MppController> MppMobEntity::GetController()
{
    return nullptr;
}

float MppMobEntity::GetFov() const
{
    return std::numbers::pi_v<float> / 4.0f * 3.0f;
}

int MppMobEntity::GetFacingX() const
{
    return FacingX;
}

int MppMobEntity::GetFacingY() const
{
    return FacingY;
}

void MppMobEntity::DoAction(std::shared_ptr<MppEntity>& entity)
{
    Animation.Update(0, FacingX, FacingY);
    entity->OnAction(*this);
}

bool MppMobEntity::IsMoving()
{
    return VelocityX || VelocityY;
}

int MppMobEntity::GetPose() const
{
    return 0;
}

int MppMobEntity::GetHealth() const
{
    return Health;
}

int MppMobEntity::GetHunger() const
{
    return Hunger;
}

int MppMobEntity::GetEnergy() const
{
    return Energy;
}

int MppMobEntity::GetSpriteTickRate() const
{
    return 10;
}

int MppMobEntity::GetSpritePose2X() const
{
    return 0;
}

int MppMobEntity::GetSpritePose2Y() const
{
    return 0;
}

void MppMobEntity::SetTickAnimation()
{
    TickAnimation = true;
}
