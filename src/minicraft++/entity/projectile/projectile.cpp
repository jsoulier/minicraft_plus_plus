#include <savepoint/savepoint.hpp>

#include <cstdint>
#include <memory>

#include <minicraft++/assert.hpp>
#include <minicraft++/entity/entity.hpp>
#include <minicraft++/entity/projectile/projectile.hpp>
#include <minicraft++/renderer.hpp>
#include <minicraft++/sprite.hpp>

MppProjectileEntity::MppProjectileEntity()
    : MppEntity()
    , Source{}
    , VelocityX{0}
    , VelocityY{0}
{
}

void MppProjectileEntity::Visit(SavepointVisitor& visitor)
{

}

void MppProjectileEntity::Update(uint64_t ticks)
{
    MppEntity::Update(ticks);
    if (Move(VelocityX, VelocityY))
    {
        OnCollision();
    }
}

void MppProjectileEntity::Render() const
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
        MppRendererFlipNone,
        MppRendererLayerParticleEntity);
}

int MppProjectileEntity::GetSize() const
{
    return 8;
}

int MppProjectileEntity::GetPhysicsOffsetX() const
{
    return 2;
}

int MppProjectileEntity::GetPhysicsOffsetY() const
{
    return 2;
}

int MppProjectileEntity::GetPhysicsWidth() const
{
    return 3;
}

int MppProjectileEntity::GetPhysicsHeight() const
{
    return 3;
}

void MppProjectileEntity::SetSource(const std::shared_ptr<MppEntity>& source, int x, int y)
{
    Source = source->GetReference();
    MppAssert(!(x && y));
    VelocityX = x * GetSpeed();
    VelocityY = y * GetSpeed();
}

void MppProjectileEntity::OnCollision()
{
    Kill();
}

int MppProjectileEntity::GetSpeed() const
{
    return 1;
}

int MppProjectileEntity::GetColor1() const
{
    return 0;
}

int MppProjectileEntity::GetColor2() const
{
    return 0;
}

int MppProjectileEntity::GetColor3() const
{
    return 0;
}

int MppProjectileEntity::GetColor4() const
{
    return 0;
}

int MppProjectileEntity::GetColor5() const
{
    return 0;
}
