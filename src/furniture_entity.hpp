#pragma once

#include "entity.hpp"
#include "item.hpp"

class MppLevel;
class MppRenderer;

class MppFurnitureEntity : public MppEntity
{
public:
    void Update(MppLevel& level, MppRenderer& renderer, int ticks) override;
    void Draw(MppRenderer& renderer);
    int GetPhysicsWidth() const override;
    int GetPhysicsHeight() const override;
    int GetPhysicsOffsetX() const override;
    int GetPhysicsOffsetY() const override;
    int GetSize() const override;
    MppPhysicsType GetPhysicsType() const override;
    virtual MppItemID GetItemID() const = 0;
    virtual bool IsConvertibleToItem() const;

protected:
    virtual int GetSpriteX() const = 0;
    virtual int GetSpriteY() const = 0;
    virtual int GetColor1() const = 0;
    virtual int GetColor2() const = 0;
    virtual int GetColor3() const = 0;
    virtual int GetColor4() const = 0;
    virtual int GetColor5() const = 0;
};