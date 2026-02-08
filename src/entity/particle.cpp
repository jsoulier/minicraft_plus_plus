#include <cstdint>

#include "../assert.hpp"
#include "../renderer.hpp"
#include "../sprite.hpp"
#include "particle.hpp"

MppParticleEntity::MppParticleEntity()
    : MppEntity()
    , Lifetime{0}
{
}

void MppParticleEntity::OnAddEntity()
{
    Lifetime = GetMaxLifetime();
    MppAssert(Lifetime);
}

void MppParticleEntity::Render() const
{
    MppEntity::Render();
    MppRendererDraw(
        MppSprite{
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
        false,
        MppRendererLayerParticleEntity);
}

void MppParticleEntity::Update(uint64_t ticks)
{
    MppEntity::Update(ticks);
    Move(GetVelocityX(), GetVelocityY());
    Lifetime--;
    if (Lifetime <= 0)
    {
        Kill();
    }
}

int MppParticleEntity::GetVelocityX() const
{
    return 0;
}

int MppParticleEntity::GetVelocityY() const
{
    return 0;
}

int MppParticleEntity::GetPhysicsOffsetX() const
{
    return 0;
}

int MppParticleEntity::GetPhysicsOffsetY() const
{
    return 0;
}

int MppParticleEntity::GetPhysicsWidth() const
{
    return 0;
}

int MppParticleEntity::GetPhysicsHeight() const
{
    return 0;
}

bool MppParticleEntity::HasPhysics() const
{
    return false;
}

bool MppParticleEntity::CanSave() const
{
    return false;
}

int MppParticleEntity::GetColor1() const
{
    return 0;
}

int MppParticleEntity::GetColor2() const
{
    return 0;
}

int MppParticleEntity::GetColor3() const
{
    return 0;
}

int MppParticleEntity::GetColor4() const
{
    return 0;
}

int MppParticleEntity::GetColor5() const
{
    return 0;
}

int MppHitParticleEntity::GetSize() const
{
    return 16;
}

int MppHitParticleEntity::GetMaxLifetime() const
{
    return 8;
}

int MppHitParticleEntity::GetSpriteX() const
{
    return 0;
}

int MppHitParticleEntity::GetSpriteY() const
{
    return 18;
}

int MppHitParticleEntity::GetColor1() const
{
    return 555;
}

int MppHitParticleEntity::GetColor2() const
{
    return 555;
}

int MppHitParticleEntity::GetColor3() const
{
    return 555;
}

int MppHitParticleEntity::GetColor4() const
{
    return 555;
}

int MppHitParticleEntity::GetColor5() const
{
    return 555;
}
