#include <savepoint/savepoint.hpp>

#include <algorithm>
#include <cmath>
#include <limits>
#include <memory>
#include <numbers>
#include <vector>

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
    , Controller{}
    , Animation{}
    , TickAnimation{false}
    , Health{-1}
    , Hunger{-1}
    , Energy{-1}
    , FacingX{0}
    , FacingY{1}
    , VelocityX{0}
    , VelocityY{0}
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
    Animation.SetTickRate(GetAnimationTickRate());
    Animation.SetPose(0, GetAnimationPose1X(), GetAnimationPose1Y());
    Animation.SetPose(1, GetAnimationPose2X(), GetAnimationPose2Y());
    Animation.SetPose(2, GetAnimationPose3X(), GetAnimationPose3Y());
    Animation.SetPose(3, GetAnimationPose4X(), GetAnimationPose4Y());
    RequestAnimationTick();
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
        Controller->Possess(Cast<MppMobEntity>());
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
        Animation.Update(GetAnimationPose(), FacingX, FacingY, ticks);
    }
    else if (TickAnimation)
    {
        Animation.Update(GetAnimationPose(), FacingX, FacingY, GetAnimationTickRate());
        TickAnimation = false;
    }
    if (ticks % GetMoveTickRate() == 0 && (VelocityX || VelocityY))
    {
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
        Controller->Render();
    }
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

void MppMobEntity::OnSetLevel(int level)
{
    MppEntity::OnSetLevel(level);
    if (Controller)
    {
        Controller->OnSetLevel(level);
    }
}

void MppMobEntity::DoAction()
{
    using EntityT = std::shared_ptr<MppEntity>;
    if (!Controller)
    {
        return;
    }
    const MppItem& held = Inventory->GetBySlot(MppInventorySlotHeld);
    MppItemRecipe actionRecipe = held.GetActionRecipe();
    if (!actionRecipe.CanCraft(MppItemIDInvalid, Inventory))
    {
        return;
    }
    RequestAnimationTick();
    actionRecipe.Craft(GetInventory());
    EntityT self = Cast<MppEntity>();
    if (held.GetActionType() == MppItemActionTypeDefault)
    {
        int thisX = X;
        int thisY = Y;
        auto [x1, y1] = GetCenter();
        X += GetActionOffset() * GetFacingX();
        Y += GetActionOffset() * GetFacingY();
        auto [x2, y2] = GetCenter();
        if (MppConsole::CVarAction.GetBool())
        {
            MppRendererDrawLine(kMppColorDebugAction, x1, y1, x2, y2, MppRendererLayerDebugAction);
        }
        std::vector<EntityT> entities = MppWorldGetEntities(GetX(), GetY());
        std::erase_if(entities, [this](const EntityT& other)
        {
            return this == other.get() || Controller->ActionFilter(other);
        });
        std::sort(entities.begin(), entities.end(), [this](EntityT& lhs, EntityT& rhs)
        {
            return GetDistance(lhs) < GetDistance(rhs);
        });
        bool didAction = false;
        for (EntityT& entity : entities)
        {
            if (GetDistance(entity) > GetActionOffset())
            {
                break;
            }
            didAction = entity->OnAction(self);
            if (didAction)
            {
                break;
            }
        }
        if (!didAction)
        {
            int size = MppTile::kSize;
            int tileX1 = x1 / size;
            int tileY1 = y1 / size;
            int tileX2 = x2 / size;
            int tileY2 = y2 / size;
            MppTile& tile1 = MppWorldGetTile(tileX1, tileY1);
            MppTile& tile2 = MppWorldGetTile(tileX2, tileY2);
            if (tile1.OnAction(self, tileX1, tileY1) && MppConsole::CVarAction.GetBool())
            {
                int tx = tileX1 * size;
                int ty = tileY1 * size;
                MppRendererDrawRect(kMppColorDebugAction, tx, ty, size, size, MppRendererLayerDebugAction);
            }
            else if (tile2.OnAction(self, tileX2, tileY2) && MppConsole::CVarAction.GetBool())
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
        MppAssert(projectile);
        projectile->Setup(self, FacingX, FacingY);
        EntityT entity = projectile->Cast<MppEntity>();
        MppWorldAddEntity(entity);
    }
}

void MppMobEntity::Equip(int index)
{
    const MppItem& item = Inventory->Get(index);
    MppInventorySlot slot = Inventory->GetSlotFromIndex(index);
    if (slot != MppInventorySlotNone)
    {
        Inventory->ResetSlot(slot);
        return;
    }
    if (item.GetType() == MppItemTypeNone)
    {
        MppLog("Tried equipping an MppItemTypeNone: %s", item.GetName().data());
    }
    else if (item.GetType() == MppItemTypeConsumable)
    {
        Inventory->SetSlot(MppInventorySlotHeld, index);
    }
    else if (item.GetType() & MppItemTypeHeld)
    {
        Inventory->SetSlot(MppInventorySlotHeld, index);
    }
}

void MppMobEntity::Push(int dx, int dy, bool useSpeed)
{
    if (useSpeed)
    {
        dx *= GetSpeed();
        dy *= GetSpeed();
    }
    VelocityX += dx;
    VelocityY += dy;
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

int MppMobEntity::GetSize() const
{
    return 16;
}

void MppMobEntity::SetFacingX(int facingX)
{
    MppAssert(!Controller);
    FacingX = facingX;
    RequestAnimationTick();
}

void MppMobEntity::SetFacingY(int facingY)
{
    MppAssert(!Controller);
    FacingY = facingY;
    RequestAnimationTick();
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

int MppMobEntity::GetSpeed() const
{
    return 1;
}

int MppMobEntity::GetActionOffset() const
{
    return 12;
}

std::shared_ptr<MppController> MppMobEntity::GetController()
{
    return Controller;
}

std::shared_ptr<MppInventory> MppMobEntity::GetInventory()
{
    return Inventory;
}

std::shared_ptr<MppController> MppMobEntity::GetDefaultController()
{
    return nullptr;
}

int MppMobEntity::GetMaxItems() const
{
    return 0;
}

int MppMobEntity::GetMoveTickRate() const
{
    return 1;
}

int MppMobEntity::GetAnimationTickRate() const
{
    return 10;
}

void MppMobEntity::RequestAnimationTick()
{
    TickAnimation = true;
}

float MppMobEntity::GetFov() const
{
    return std::numbers::pi_v<float> / 4.0f * 3.0f;
}

int MppMobEntity::GetMaxHealth() const
{
    return 1;
}

int MppMobEntity::GetMaxHunger() const
{
    return 1;
}

int MppMobEntity::GetMaxEnergy() const
{
    return 1;
}

int MppMobEntity::GetAnimationPose() const
{
    return 0;
}

int MppMobEntity::GetAnimationPose2X() const
{
    return 0;
}

int MppMobEntity::GetAnimationPose2Y() const
{
    return 0;
}

int MppMobEntity::GetAnimationPose3X() const
{
    return 0;
}

int MppMobEntity::GetAnimationPose3Y() const
{
    return 0;
}

int MppMobEntity::GetAnimationPose4X() const
{
    return 0;
}

int MppMobEntity::GetAnimationPose4Y() const
{
    return 0;
}
