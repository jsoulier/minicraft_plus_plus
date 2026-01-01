#include <SDL3/SDL.h>

#include "color.hpp"
#include "furniture_entity.hpp"
#include "humanoid_entity.hpp"
#include "humanoid_inventory.hpp"
#include "level.hpp"
#include "renderer.hpp"
#include "sprite.hpp"

struct Frames
{
    void GetSprite(int& x, int& y, bool& flip, int dx, int dy, bool tick, bool held) const
    {
        SDL_assert(dx || dy);
        flip = false;
        if (!held)
        {
            x = IdleX;
            y = IdleY;
        }
        else
        {
            x = HeldX;
            y = HeldY;
        }
        if (dy)
        {
            flip = tick;
            if (dy > 0)
            {
                x += DownOffsetX;
            }
            else
            {
                x += UpOffsetX;
            }
        }
        else
        {
            flip = dx < 0;
            if (tick)
            {
                x += Side1OffsetX;
            }
            else
            {
                x += Side2OffsetX;
            }
        }
    }

    int IdleX;
    int IdleY;
    int HeldX;
    int HeldY;
    int DownOffsetX;
    int UpOffsetX;
    int Side1OffsetX;
    int Side2OffsetX;
};

static constexpr Frames kFrames{0, 6, 4, 6, 0, 1, 2, 3};
static constexpr Frames kHelmetFrames{0, 8, 4, 8, 0, 1, 2, 2};
static constexpr Frames kCuirassFrames{0, 9, 4, 9, 0, 1, 2, 3};
static constexpr Frames kLeggingsFrames{0, 10, 4, 10, 0, 1, 2, 3};
static constexpr Frames kBootsFrames{0, 11, 4, 11, 0, 1, 2, 3};

MppHumanoidEntity::MppHumanoidEntity()
    : MppMobEntity()
    , Held{}
    , IsInHeldCallback{false}
    , Flip{false}
{
    std::shared_ptr<MppHumanoidInventory> inventory = std::make_shared<MppHumanoidInventory>();
    SDL_assert(inventory);
    SetInventory(inventory);
    inventory->SetHeldCallback([this](int index, const MppItem* item)
    {
        if (IsInHeldCallback)
        {
            return;
        }
        IsInHeldCallback = true;
        std::shared_ptr<MppHumanoidInventory> inventory = std::dynamic_pointer_cast<MppHumanoidInventory>(GetInventory());
        SDL_assert(inventory);
        if (Held && Held->IsConvertibleToItem())
        {
            inventory->Add(MppItem(Held->GetItemID()));
            Held = nullptr;
        }
        if (item && !Held)
        {
            if (item->GetType() != MppItemTypeFurniture)
            {
                return;
            }
            Held = item->CreateFurnitureEntity();
            inventory->Remove(index);
        }
        IsInHeldCallback = false;
    });
}

void MppHumanoidEntity::Update(MppLevel& level, MppRenderer& renderer, int ticks)
{
    MppMobEntity::Update(level, renderer, ticks);
    MppHumanoidInventory* inventory = dynamic_cast<MppHumanoidInventory*>(GetInventory().get());
    SDL_assert(inventory);
    bool held = Held.get() != nullptr;
    bool flip = false;
    int x = 0;
    int y = 0;
    kFrames.GetSprite(x, y, flip, DeltaX, DeltaY, Flip, held);
    renderer.Draw(
        MppSprite{
            GetSpriteBorderColor(),
            GetSpriteShirtColor(),
            GetSpritePantColor(),
            GetSpriteSkinColor(),
            GetSpriteShoeColor(),
            x,
            y,
            GetSize(),
        },
        X,
        Y,
        flip,
        MppRenderer::LayerMobEntity);
    if (Held)
    {
        int y = GetY() - GetSize() + GetPhysicsOffsetY();
        int offsetY = Held->GetSize() - Held->GetPhysicsOffsetY() - Held->GetPhysicsHeight();
        Held->SetX(GetX());
        Held->SetY(y + offsetY);
        Held->Draw(renderer);
    }
    if (const MppItem* item = inventory->GetHelmet())
    {
        kHelmetFrames.GetSprite(x, y, flip, DeltaX, DeltaY, Flip, held);
        renderer.Draw(
            MppSprite{
                item->GetColor1(),
                item->GetColor2(),
                item->GetColor3(),
                item->GetColor4(),
                item->GetColor5(),
                x,
                y,
                GetSize(),
            },
            X,
            Y,
            flip,
            MppRenderer::LayerMobEntityOverlay);
    }
    if (const MppItem* item = inventory->GetCuirass())
    {
        kCuirassFrames.GetSprite(x, y, flip, DeltaX, DeltaY, Flip, held);
        renderer.Draw(
            MppSprite{
                item->GetColor1(),
                item->GetColor2(),
                item->GetColor3(),
                item->GetColor4(),
                item->GetColor5(),
                x,
                y,
                GetSize(),
            },
            X,
            Y,
            flip,
            MppRenderer::LayerMobEntityOverlay);
    }
    if (const MppItem* item = inventory->GetLeggings())
    {
        kLeggingsFrames.GetSprite(x, y, flip, DeltaX, DeltaY, Flip, held);
        renderer.Draw(
            MppSprite{
                item->GetColor1(),
                item->GetColor2(),
                item->GetColor3(),
                item->GetColor4(),
                item->GetColor5(),
                x,
                y,
                GetSize(),
            },
            X,
            Y,
            flip,
            MppRenderer::LayerMobEntityOverlay);
    }
    if (const MppItem* item = inventory->GetBoots())
    {
        kBootsFrames.GetSprite(x, y, flip, DeltaX, DeltaY, Flip, held);
        renderer.Draw(
            MppSprite{
                item->GetColor1(),
                item->GetColor2(),
                item->GetColor3(),
                item->GetColor4(),
                item->GetColor5(),
                x,
                y,
                GetSize(),
            },
            X,
            Y,
            flip,
            MppRenderer::LayerMobEntityOverlay);
    }
}

void MppHumanoidEntity::Visit(SavepointVisitor& visitor)
{
    MppMobEntity::Visit(visitor);
    visitor(Held);
}

void MppHumanoidEntity::Action(MppLevel& level, MppRenderer& renderer, int ticks)
{
    if (Held)
    {
        level.Add(Held);
        Held = nullptr;
    }
    else
    {
        // TODO:
    }
}

void MppHumanoidEntity::Move(MppLevel& level, int dx, int dy, int ticks)
{
    MppMobEntity::Move(level, dx, dy, ticks);
    if (ticks % GetAnimationSpeed() == 0)
    {
        Flip = !Flip;
    }
}

int MppHumanoidEntity::GetPhysicsWidth() const
{
    return 12;
}

int MppHumanoidEntity::GetPhysicsHeight() const
{
    return 14;
}

int MppHumanoidEntity::GetPhysicsOffsetX() const
{
    return 2;
}

int MppHumanoidEntity::GetPhysicsOffsetY() const
{
    return 1;
}

int MppHumanoidEntity::GetAnimationSpeed() const
{
    return 10;
}
