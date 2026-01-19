#pragma once

#include "../entity.hpp"

class MppMobEntity : public MppEntity
{
public:
    MppMobEntity();
    virtual void Visit(SavepointVisitor& visitor) override;
    virtual void Update(uint64_t ticks) override;
    virtual void Render() override;
    int GetSize() const override;

protected:
    int VelocityX;
    int VelocityY;
    int FacingX;
    int FacingY;
};