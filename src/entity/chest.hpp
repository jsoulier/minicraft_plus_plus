#pragma once

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

private:
    std::shared_ptr<MppInventory> Inventory;
    std::weak_ptr<MppInventory> Other;
    std::weak_ptr<MppInventory> Focused;
};
