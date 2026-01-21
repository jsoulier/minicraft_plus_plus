#pragma once

#include <savepoint/base.hpp>

#include <memory>

#include "furniture.hpp"
#include "../input.hpp"

class MppInventory;

class MppFurnaceEntity final
    : public MppFurnitureEntity
    , public MppInputHandler
{
    SAVEPOINT_DERIVED(MppFurnaceEntity)

public:
    void OnAction(MppEntity& instigator) override;
    void OnAction() override;
    void OnUp() override;
    void OnDown() override;
    void OnHeldUp() override;
    void OnHeldDown() override;
    MppItemID GetItemID() const override;
    
protected:
    int GetSpriteX() const override;
    int GetSpriteY() const override;
    int GetColor1() const override;
    int GetColor2() const override;
    int GetColor3() const override;
    int GetColor4() const override;
    int GetColor5() const override;

private:
    std::weak_ptr<MppInventory> Inventory;
};
