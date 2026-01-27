#include <savepoint/savepoint.hpp>

#include <memory>
#include <numbers>

#include "../assert.hpp"
#include "../color.hpp"
#include "../inventory.hpp"
#include "../renderer.hpp"
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
    MppEntity::Render();
}

void MppMobEntity::Move(int dx, int dy)
{
    MppAssert(VelocityX == 0);
    MppAssert(VelocityY == 0);
    if (dx && std::abs(dx) < std::abs(dy))
    {
        VelocityX = dx / std::abs(dx);
    }
    else if (dy && std::abs(dy) < std::abs(dx))
    {
        VelocityY = dy / std::abs(dy);
    }
    else if (dx)
    {
        MppAssert(dy == 0);
        VelocityX = dx / std::abs(dx);
    }
    else if (dy)
    {
        MppAssert(dx == 0);
        VelocityY = dy / std::abs(dy);
    }
    else
    {
        MppAssert(false);
    }
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

std::shared_ptr<MppMobController> MppMobEntity::GetController()
{
    return nullptr;
}

float MppMobEntity::GetFov() const
{
    return std::numbers::pi_v<float> / 4.0f;
}
