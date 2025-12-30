#include <SDL3/SDL.h>

#include "color.hpp"
#include "humanoid_entity.hpp"
#include "humanoid_inventory.hpp"
#include "renderer.hpp"
#include "sprite.hpp"

struct Frames
{
    void GetSprite(int& x, int& y, bool& flip, int dx, int dy, bool tick) const
    {
        SDL_assert(dx || dy);
        flip = false;
        x = X;
        y = Y;
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

    int X;
    int Y;
    int DownOffsetX;
    int UpOffsetX;
    int Side1OffsetX;
    int Side2OffsetX;
};

static constexpr Frames kFrames{0, 6, 0, 1, 2, 3};
static constexpr Frames kHelmetFrames{0, 8, 0, 1, 2, 2};
static constexpr Frames kChestplateFrames{0, 9, 0, 1, 2, 3};
static constexpr Frames kLeggingsFrames{0, 10, 0, 1, 2, 3};
static constexpr Frames kBootsFrames{0, 11, 0, 1, 2, 3};

MppHumanoidEntity::MppHumanoidEntity()
    : MppMobEntity()
    , Flip{false}
{
    SetInventory(std::make_shared<MppHumanoidInventory>());
}

void MppHumanoidEntity::Update(MppLevel& level, MppRenderer& renderer, int ticks)
{
    MppMobEntity::Update(level, renderer, ticks);
    bool flip = false;
    int x = 0;
    int y = 0;
    kFrames.GetSprite(x, y, flip, DeltaX, DeltaY, Flip);
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
    MppHumanoidInventory* inventory = dynamic_cast<MppHumanoidInventory*>(GetInventory().get());
    SDL_assert(inventory);
    if (const MppItem* item = inventory->GetHelmet())
    {
        kHelmetFrames.GetSprite(x, y, flip, DeltaX, DeltaY, Flip);
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
        kChestplateFrames.GetSprite(x, y, flip, DeltaX, DeltaY, Flip);
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
        kLeggingsFrames.GetSprite(x, y, flip, DeltaX, DeltaY, Flip);
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
        kBootsFrames.GetSprite(x, y, flip, DeltaX, DeltaY, Flip);
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
