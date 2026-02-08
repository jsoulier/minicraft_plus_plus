#pragma once

#include <savepoint/savepoint.hpp>

#include <cstdint>

#include <minicraft++/entity/entity.hpp>

class MppParticleEntity : public MppEntity
{
public:
    MppParticleEntity();
    void OnAddEntity() override;
    void Render() const override;
    void Update(uint64_t ticks) override;
    int GetPhysicsOffsetX() const;
    int GetPhysicsOffsetY() const;
    int GetPhysicsWidth() const;
    int GetPhysicsHeight() const;
    bool HasPhysics() const override;
    bool CanSave() const override;

protected:
    virtual int GetVelocityX() const;
    virtual int GetVelocityY() const;
    virtual int GetMaxLifetime() const = 0;
    virtual int GetSpriteX() const = 0;
    virtual int GetSpriteY() const = 0;
    virtual int GetColor1() const;
    virtual int GetColor2() const;
    virtual int GetColor3() const;
    virtual int GetColor4() const;
    virtual int GetColor5() const;

private:
    int Lifetime;
};
