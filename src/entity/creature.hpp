#pragma once

#include <savepoint/savepoint.hpp>

#include <cstdint>

#include "mob.hpp"

class MppCreatureEntity : public MppMobEntity
{
public:
    MppCreatureEntity();
    virtual void Visit(SavepointVisitor& visitor) override;
    virtual void Update(uint64_t ticks) override;
    virtual void Render() const override;

protected:
    virtual int GetSpriteX() const = 0;
    virtual int GetSpriteY() const = 0;
    virtual int GetColor1() const = 0;
    virtual int GetColor2() const = 0;
    virtual int GetColor3() const = 0;
    virtual int GetColor4() const = 0;
    virtual int GetColor5() const = 0;
};
