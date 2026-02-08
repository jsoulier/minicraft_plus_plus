#pragma once

#include <savepoint/savepoint.hpp>

#include <minicraft++/entity/particle/particle.hpp>

class MppHitParticleEntity : public MppParticleEntity
{
    SAVEPOINT_POLY(MppHitParticleEntity)
    
public:
    int GetSize() const override;

protected:
    int GetMaxLifetime() const override;
    int GetSpriteX() const override;
    int GetSpriteY() const override;
    int GetColor1() const override;
    int GetColor2() const override;
    int GetColor3() const override;
    int GetColor4() const override;
    int GetColor5() const override;
};
