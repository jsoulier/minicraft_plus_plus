#pragma once

#include <savepoint/savepoint.hpp>

#include <memory>

#include <minicraft++/entity/entity.hpp>
#include <minicraft++/item.hpp>

class MppInventory;

class MppFurnitureEntity : public MppEntity
{
public:
    virtual void Render() const override;
    virtual void OnAction(MppEntity& instigator) override;
    virtual bool OnInteraction(MppEntity& instigator);
    int GetPhysicsOffsetX() const override;
    int GetPhysicsOffsetY() const override;
    int GetPhysicsWidth() const override;
    int GetPhysicsHeight() const override;
    int GetSize() const override;
    virtual MppItemID GetItemID() const = 0;
    virtual bool IsEmpty() const;

protected:
    virtual int GetSpriteX() const = 0;
    virtual int GetSpriteY() const = 0;
    virtual int GetColor1() const = 0;
    virtual int GetColor2() const = 0;
    virtual int GetColor3() const = 0;
    virtual int GetColor4() const = 0;
    virtual int GetColor5() const = 0;
};