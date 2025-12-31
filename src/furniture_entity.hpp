#pragma once

#include "entity.hpp"
#include "item.hpp"

class MppFurnitureEntity : public MppEntity
{
public:
    int GetPhysicsWidth() const override;
    int GetPhysicsHeight() const override;
    int GetSize() const override;
    MppPhysicsType GetPhysicsType() const override;
    virtual MppItemID GetItemID() const = 0;
    virtual bool IsConvertableToItem() const;

protected:
    virtual int GetSpriteX() const = 0;
    virtual int GetSpriteY() const = 0;
    virtual int GetColor1() const = 0;
    virtual int GetColor2() const = 0;
    virtual int GetColor3() const = 0;
    virtual int GetColor4() const = 0;
    virtual int GetColor5() const = 0;
    int GetPhysicsOffsetX() const override;
    int GetPhysicsOffsetY() const override;
};