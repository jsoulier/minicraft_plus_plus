#include <savepoint/savepoint.hpp>

#include <memory>

#include "../color.hpp"
#include "../renderer.hpp"
#include "../sprite.hpp"
#include "controller/melee.hpp"
#include "spider.hpp"

static constexpr int kTickRate = 10;

MppSpiderEntity::MppSpiderEntity()
    : MppMobEntity()
{
}

void MppSpiderEntity::OnAddEntity()
{
    MppMobEntity::OnAddEntity();
    Animation.SetTickRate(kTickRate);
    Animation.SetPose(0, GetSpritePose1X(), GetSpritePose1Y());
    // TODO: having to update here is error prone
    Animation.Update(0, FacingX, FacingY, 0);
}

void MppSpiderEntity::PostUpdate(uint64_t ticks)
{
    MppMobEntity::PostUpdate(ticks);
    Animation.Update(0, VelocityX, VelocityY, ticks);
}

void MppSpiderEntity::Render() const
{
    MppMobEntity::Render();
    MppRendererDraw(
        MppSprite{
            GetSpriteBorderColor(),
            GetSpriteEyeColor(),
            GetSpriteLegColor(),
            GetSpriteBodyColor1(),
            GetSpriteBodyColor2(),
            Animation.GetX(),
            Animation.GetY(),
            GetSize(),
        },
        X,
        Y,
        Animation.GetFlip(),
        MppRendererLayerEntity);
}

int MppSpiderEntity::GetSpritePose1X() const
{
    return 8;
}

int MppSpiderEntity::GetSpritePose1Y() const
{
    return 7;
}

int MppSpiderEntity::GetPhysicsOffsetX() const
{
    return 0;
}

int MppSpiderEntity::GetPhysicsOffsetY() const
{
    return 0;
}

int MppSpiderEntity::GetPhysicsWidth() const
{
    return 16;
}

int MppSpiderEntity::GetPhysicsHeight() const
{
    return 16;
}

std::shared_ptr<MppController> MppSpiderEntity::GetController() 
{
    return std::make_shared<MppMeleeController>();
}

int MppSpiderEntity::GetMaxHealth() const
{
    return 1;
}

int MppSpiderEntity::GetMaxHunger() const
{
    return 1;
}

int MppSpiderEntity::GetMaxEnergy() const
{
    return 1;
}

int MppSpiderEntity::GetSpriteBorderColor() const
{
    return 0;
}

int MppSpiderEntity::GetSpriteEyeColor() const
{
    return 500;
}

int MppSpiderEntity::GetSpriteBodyColor1() const
{
    return 111;
}

int MppSpiderEntity::GetSpriteBodyColor2() const
{
    return 222;
}

int MppSpiderEntity::GetSpriteLegColor() const
{
    return 222;
}

int MppSpiderEntity::GetSpeed() const
{
    return 3;
}
