#pragma once

#include <memory>

#include "furniture.hpp"
#include "../input.hpp"

class MppInventory;

class MppWorkbenchEntity final
    : public MppFurnitureEntity
    , public MppInputHandler
{
    SAVEPOINT_DERIVED(MppWorkbenchEntity)

public:
    void OnAction(MppEntity& instigator) override;
    void OnAction() override;
    void OnUp() override;
    void OnDown() override;
    void OnHeldUp() override;
    void OnHeldDown() override;

private:
    std::weak_ptr<MppInventory> Inventory;
};
