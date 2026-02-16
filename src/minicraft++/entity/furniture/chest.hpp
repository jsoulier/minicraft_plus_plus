#pragma once

#include <savepoint/savepoint.hpp>

#include <memory>

#include <minicraft++/input.hpp>
#include <minicraft++/entity/furniture/furniture.hpp>

class MppChestEntity final
    : public MppFurnitureEntity
    , public MppInputHandler

{
    SAVEPOINT_POLY(MppChestEntity)

public:
    MppChestEntity();
    void Visit(SavepointVisitor& visitor);
    bool OnInteraction(std::shared_ptr<MppEntity>& instigator) override;
    void OnInputAction() override;
    void OnInputRender() const override;
    void OnInputUpArrow() override;
    void OnInputDownArrow() override;
    void OnInputLeftArrow() override;
    void OnInputRightArrow() override;
    MppItemID GetItemID() const override;
    bool IsEmpty() const override;
    void OnInputLoseFocus() override;

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
