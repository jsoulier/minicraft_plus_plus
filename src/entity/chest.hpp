#pragma once

#include <savepoint/savepoint.hpp>

#include <memory>

#include "../input.hpp"
#include "furniture.hpp"

class MppChestEntity final
    : public MppFurnitureEntity
    , public MppInputHandler

{
    SAVEPOINT_POLY(MppChestEntity)

public:
    MppChestEntity();
    void Visit(SavepointVisitor& visitor);
    void OnAction(MppEntity& instigator) override;
    void OnAction() override;
    void OnRender() override;
    void OnUpArrow() override;
    void OnDownArrow() override;
    void OnLeftArrow() override;
    void OnRightArrow() override;
    MppItemID GetItemID() const override;
    bool IsEmpty() const override;
    void OnLoseFocus() override;

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
};
