#pragma once

#include <savepoint/base.hpp>

#include "chest_inventory.hpp"
#include "furniture_entity.hpp"

class MppChestEntity : public MppFurnitureEntity
{
    SAVEPOINT_DERIVED(MppChestEntity)

public:
    MppItemID GetItemID() const override;
    bool IsConvertibleToItem() const override;
    
protected:
    int GetSpriteX() const override;
    int GetSpriteY() const override;
    int GetColor1() const override;
    int GetColor2() const override;
    int GetColor3() const override;
    int GetColor4() const override;
    int GetColor5() const override;

private:
    MppChestInventory Inventory;
};