#pragma once

#include <savepoint/savepoint.hpp>

#include <minicraft++/entity/projectile/projectile.hpp>

class MppArrowEntity : public MppProjectileEntity
{
    SAVEPOINT_POLY(MppArrowEntity)

public:

protected:
    int GetSpriteX() const;
    int GetSpriteY() const;
    int GetColor1() const;
    int GetColor2() const;
    int GetColor3() const;
    int GetColor4() const;
    int GetColor5() const;
};