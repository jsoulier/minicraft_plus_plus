#pragma once

#include <savepoint/base.hpp>

#include <memory>

#include "../input.hpp"
#include "furniture.hpp"

class MppChestEntity final
    : public MppFurnitureEntity
    , public MppInputHandler

{
    SAVEPOINT_DERIVED(MppChestEntity)

public:
    MppChestEntity();
    void OnAction(MppEntity& instigator) override;
    void OnAction() override;
    void OnUp() override;
    void OnDown() override;
    void OnLeft() override;
    void OnRight() override;
    void OnHeldUp() override;
    void OnHeldDown() override;
    MppItemID GetItemID() const override;
    bool IsEmpty() const override;

protected:
    int GetSpriteX() const override;
    int GetSpriteY() const override;
    int GetColor1() const override;
    int GetColor2() const override;
    int GetColor3() const override;
    int GetColor4() const override;
    int GetColor5() const override;

private:
    std::shared_ptr<MppInventory> Inventory;
    std::weak_ptr<MppInventory> Other;
    std::weak_ptr<MppInventory> Focused;
};
