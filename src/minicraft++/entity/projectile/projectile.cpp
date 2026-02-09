#include <savepoint/savepoint.hpp>

#include <cstdint>
#include <memory>

#include <minicraft++/assert.hpp>
#include <minicraft++/entity/entity.hpp>
#include <minicraft++/entity/projectile/projectile.hpp>
#include <minicraft++/renderer.hpp>
#include <minicraft++/sprite.hpp>

static constexpr int kVelocity = 2;

MppProjectileEntity::MppProjectileEntity()
    : MppEntity()
    , Source{}
    , VelocityX{0}
    , VelocityY{0}
{
}

void MppProjectileEntity::Visit(SavepointVisitor& visitor)
{
    MppEntity::Visit(visitor);
    visitor(Source);
    visitor(VelocityX);
    visitor(VelocityY);
}

void MppProjectileEntity::Update(uint64_t ticks)
{
    MppEntity::Update(ticks);
    if (ticks % GetSpeed() == 0)
    {
        if (!Move(VelocityX * kVelocity, VelocityY * kVelocity))
        {
            OnCollision();
        }
    }
}

void MppProjectileEntity::Render() const
{
    MppRendererMod mod = MppRendererModNone;
    if (VelocityY < 0)
    {
        mod = MppRendererModFlipVertically;
    }
    else if (VelocityX < 0)
    {
        mod = MppRendererModRotate90CW;
    }
    else if (VelocityX > 0)
    {
        mod = MppRendererModRotate90CCW;
    }
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
        mod,
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
    return 4;
}

int MppProjectileEntity::GetPhysicsHeight() const
{
    return 4;
}

void MppProjectileEntity::Setup(const std::shared_ptr<MppEntity>& source, int facingX, int facingY)
{
    static constexpr int kOffset = 16;
    Source = source->GetReference();
    MppAssert(!(facingX && facingY));
    X = 0;
    Y = 0;
    auto [thisX, thisY] = GetCenter();
    auto [x, y] = source->GetCenter();
    X = x - thisX + facingX * kOffset;
    Y = y - thisY + facingY * kOffset;
    VelocityX = facingX;
    VelocityY = facingY;
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
