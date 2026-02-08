#include <cstdint>

#include <minicraft++/assert.hpp>
#include <minicraft++/renderer.hpp>
#include <minicraft++/sprite.hpp>
#include <minicraft++/entity/particle/particle.hpp>

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
