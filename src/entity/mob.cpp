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
    , VelocityX{0}
    , VelocityY{0}
    , FacingX{0}
    , FacingY{1}
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
}

void MppMobEntity::Visit(SavepointVisitor& visitor)
{
    MppEntity::Visit(visitor);
    visitor(Inventory);
    visitor(FacingX);
    visitor(FacingY);
    // Controller has state so it should be serialized as well
    visitor(Controller);
    if (Controller && visitor.IsReading())
    {
        Controller->SetEntity(std::dynamic_pointer_cast<MppMobEntity>(shared_from_this()));
    }
}

void MppMobEntity::Update(uint64_t ticks)
{
    MppEntity::Update(ticks);
    if (Controller)
    {
        Controller->Update(ticks);
    }
    PostUpdate(ticks);
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

void MppMobEntity::PostUpdate(uint64_t ticks)
{
}

void MppMobEntity::Render() const
{
    MppAssert(FacingX || FacingY);
    MppEntity::Render();
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

void MppMobEntity::Move(int dx, int dy)
{
    MppAssert(VelocityX == 0);
    MppAssert(VelocityY == 0);
    if (dx)
    {
        VelocityX = dx / std::abs(dx);
    }
    if (dy)
    {
        VelocityY = dy / std::abs(dy);
    }
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
