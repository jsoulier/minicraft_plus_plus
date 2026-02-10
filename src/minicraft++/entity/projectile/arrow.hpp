#pragma once

#include <savepoint/savepoint.hpp>

#include <minicraft++/entity/projectile/projectile.hpp>

class MppArrowEntity final : public MppProjectileEntity
{
    SAVEPOINT_POLY(MppArrowEntity)

public:

protected:
    int GetSpeed() const override;
    int GetSpriteX() const override;
    int GetSpriteY() const override;
    int GetColor1() const override;
    int GetColor2() const override;
    int GetColor3() const override;
    int GetColor4() const override;
    int GetColor5() const override;
};