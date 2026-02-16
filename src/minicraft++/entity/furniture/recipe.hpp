#pragma once

#include <savepoint/savepoint.hpp>

#include <memory>

#include <minicraft++/entity/furniture/furniture.hpp>
#include <minicraft++/input.hpp>

class MppInventory;

class MppRecipeEntity
    : public MppFurnitureEntity
    , public MppInputHandler
{
public:
    MppRecipeEntity();
    bool OnInteraction(std::shared_ptr<MppEntity>& instigator) override;
    void OnInputAction() override;
    void OnInputUpArrow() override;
    void OnInputDownArrow() override;
    void OnInputRender() const override;

private:
    void Build();

private:
    std::shared_ptr<MppInventory> Inventory;
    std::weak_ptr<MppInventory> Other;
};