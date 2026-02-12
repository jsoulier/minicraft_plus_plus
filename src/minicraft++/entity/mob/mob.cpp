#include <savepoint/savepoint.hpp>

#include <cmath>
#include <limits>
#include <memory>
#include <numbers>

#include <minicraft++/assert.hpp>
#include <minicraft++/color.hpp>
#include <minicraft++/console.hpp>
#include <minicraft++/entity/controller/controller.hpp>
#include <minicraft++/entity/mob/mob.hpp>
#include <minicraft++/entity/projectile/projectile.hpp>
#include <minicraft++/inventory.hpp>
#include <minicraft++/item.hpp>
#include <minicraft++/log.hpp>
#include <minicraft++/renderer.hpp>
#include <minicraft++/tile.hpp>
#include <minicraft++/util.hpp>
#include <minicraft++/world.hpp>

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

void MppMobEntity::OnCreate()
{
    MppEntity::OnCreate();
    Inventory->SetMaxItems(GetMaxItems());
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
    SetTickAnimation();
}

void MppMobEntity::OnAdd()
{
    MppEntity::OnAdd();
    if (!Controller)
    {
        Controller = GetDefaultController();
    }
    if (Controller)
    {
        Controller->Possess(std::dynamic_pointer_cast<MppMobEntity>(shared_from_this()));
    }
}

void MppMobEntity::OnPossess(const std::shared_ptr<MppController>& controller)
{
    Controller = controller;
}

void MppMobEntity::OnUnpossess()
{
    Controller = nullptr;
}

std::shared_ptr<MppController>& MppMobEntity::GetController()
{
    return Controller;
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
    if (IsMoving())
    {
        Animation.Update(GetPose(), FacingX, FacingY, ticks);
    }
    else if (TickAnimation)
    {
        Animation.Update(GetPose(), FacingX, FacingY, GetSpriteTickRate());
        TickAnimation = false;
    }
    if (ticks % GetMoveTickRate() == 0 && (VelocityX || VelocityY))
    {
        // TODO: speed applies to a push from another entity. that shouldn't happen
        VelocityX *= GetSpeed();
        VelocityY *= GetSpeed();
        MppEntity::Move(VelocityX, VelocityY);
        FacingY = MppNormalize(VelocityY);
        if (!FacingY)
        {
            FacingX = MppNormalize(VelocityX);
        }
        else
        {
            FacingX = 0;
        }
    }
    MppAssert(std::abs(FacingX) == 1 || std::abs(FacingY) == 1);
    MppAssert(!(FacingX && FacingY));
    VelocityX = 0;
    VelocityY = 0;
}

void MppMobEntity::Render() const
{
    MppEntity::Render();
    if (Controller)
    {
        Controller->RenderFromEntity();
    }
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
        Animation.GetMod(),
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

void MppMobEntity::DoAction()
{
    // TODO: should eventually become an assert
    if (!Controller)
    {
        return;
    }
    const MppItem& held = Inventory->GetBySlot(MppInventorySlotHeld);
    MppItemRecipe actionRecipe = held.GetActionRecipe();
    if (!actionRecipe.CanCraft(MppItemIDInvalid, Inventory))
    {
        // TODO: play a sound (e.g. "out of arrows")
        return;
    }
    if (held.GetActionType() == MppItemActionTypeAttackOrInteract)
    {
        auto [centerX1, centerY1] = GetCenter();
        int thisX = X;
        int thisY = Y;
        X += GetActionOffset() * GetFacingX();
        Y += GetActionOffset() * GetFacingY();
        auto [centerX2, centerY2] = GetCenter();
        if (MppConsole::CVarAction.GetBool())
        {
            MppRendererDrawLine(kMppColorDebugAction, centerX1, centerY1, centerX2, centerY2, MppRendererLayerDebugAction);
        }
        std::vector<std::shared_ptr<MppEntity>> entities = MppWorldGetEntities(GetX(), GetY());
        std::erase_if(entities, [this](const std::shared_ptr<MppEntity>& other)
        {
            return this == other.get() || Controller->ActionFilter(other);
        });
        std::sort(entities.begin(), entities.end(), [this](std::shared_ptr<MppEntity>& lhs, std::shared_ptr<MppEntity>& rhs)
        {
            return GetDistance(lhs) < GetDistance(rhs);
        });
        bool didAction = false;
        for (std::shared_ptr<MppEntity>& entity : entities)
        {
            if (GetDistance(entity) > GetActionRange())
            {
                break;
            }
            didAction = entity->OnAction(*this);
            if (didAction)
            {
                break;
            }
        }
        if (!didAction)
        {
            int size = MppTile::kSize;
            int tileX1 = centerX1 / size;
            int tileY1 = centerY1 / size;
            int tileX2 = centerX2 / size;
            int tileY2 = centerY2 / size;
            MppTile& tile1 = MppWorldGetTile(tileX1, tileY1);
            MppTile& tile2 = MppWorldGetTile(tileX2, tileY2);
            if (tile1.IsValid() && tile1.OnAction(*this, tileX1, tileY1) && MppConsole::CVarAction.GetBool())
            {
                int tx = tileX1 * size;
                int ty = tileY1 * size;
                MppRendererDrawRect(kMppColorDebugAction, tx, ty, size, size, MppRendererLayerDebugAction);
            }
            else if (tile2.IsValid() && tile2.OnAction(*this, tileX2, tileY2) && MppConsole::CVarAction.GetBool())
            {
                int tx = tileX2 * size;
                int ty = tileY2 * size;
                MppRendererDrawRect(kMppColorDebugAction, tx, ty, size, size, MppRendererLayerDebugAction);
            }
        }
        X = thisX;
        Y = thisY;
    }
    else if (held.GetActionType() == MppItemActionTypeProjectile)
    {
        std::shared_ptr<MppProjectileEntity> projectile = held.CreateProjectileEntity();
        projectile->Setup(shared_from_this(), FacingX, FacingY);
        std::shared_ptr<MppEntity> entity = std::static_pointer_cast<MppEntity>(projectile);
        MppWorldAddEntity(entity);
    }
    else
    {
        MppAssert(false);
    }
    SetTickAnimation();
    actionRecipe.Craft(GetInventory());
}

void MppMobEntity::EquipItemFromInventory(int index)
{
    const MppItem& item = Inventory->Get(index);
    if (item.GetType() & MppItemTypeEquipment)
    {
        MppInventorySlot slot = Inventory->GetSlotFromIndex(index);
        if (slot != MppInventorySlotNone)
        {
            Inventory->ResetSlot(slot);
            return;
        }
    }
    if (item.GetType() == MppItemTypeNone)
    {
        MppLog("Tried equipping an MppItemTypeNone: %s", item.GetName().data());
    }
    else if (item.GetType() == MppItemTypeConsumable)
    {
        // TODO:
    }
    else if (item.GetType() & MppItemTypeHeld)
    {
        Inventory->SetSlot(MppInventorySlotHeld, index);
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

int MppMobEntity::GetMoveTickRate() const
{
    return 1;
}

int MppMobEntity::GetSpeed() const
{
    return 1;
}

std::shared_ptr<MppController> MppMobEntity::GetDefaultController()
{
    return nullptr;
}

float MppMobEntity::GetFov() const
{
    return std::numbers::pi_v<float> / 4.0f * 3.0f;
}

void MppMobEntity::SetFacingX(int facingX)
{
    MppAssert(!Controller);
    FacingX = facingX;
    SetTickAnimation();
}

void MppMobEntity::SetFacingY(int facingY)
{
    MppAssert(!Controller);
    FacingY = facingY;
    SetTickAnimation();
}

int MppMobEntity::GetFacingX() const
{
    return FacingX;
}

int MppMobEntity::GetFacingY() const
{
    return FacingY;
}

bool MppMobEntity::IsFacing(int facingX, int facingY) const
{
    return FacingX == facingX && FacingY == facingY;
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

int MppMobEntity::GetActionOffset() const
{
    return 12;
}
