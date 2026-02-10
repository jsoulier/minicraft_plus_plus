#pragma once

#include <savepoint/savepoint.hpp>

#include <cstdint>
#include <memory>

#include <minicraft++/entity/entity.hpp>

class MppProjectileEntity : public MppEntity
{
public:
    MppProjectileEntity();
    void Visit(SavepointVisitor& visitor) override;
    void Update(uint64_t ticks) override;
    void Render() const override;
    int GetSize() const override;
    int GetPhysicsOffsetX() const;
    int GetPhysicsOffsetY() const;
    int GetPhysicsWidth() const;
    int GetPhysicsHeight() const;
    void Setup(const std::shared_ptr<MppEntity>& source, int facingX, int facingY);

protected:
    virtual void OnCollision();
    virtual int GetMoveTickRate() const;
    virtual int GetSpeed() const;
    virtual int GetSpriteX() const = 0;
    virtual int GetSpriteY() const = 0;
    virtual int GetColor1() const;
    virtual int GetColor2() const;
    virtual int GetColor3() const;
    virtual int GetColor4() const;
    virtual int GetColor5() const;

private:
    MppEntityReference Source;
    int VelocityX;
    int VelocityY;
};