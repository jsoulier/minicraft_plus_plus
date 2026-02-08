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
    bool OnInteraction(MppEntity& instigator) override;
    void OnAction() override;
    void OnUpArrow() override;
    void OnDownArrow() override;
    void OnRender() override;

private:
    void Build();

private:
    std::shared_ptr<MppInventory> Inventory;
    std::weak_ptr<MppInventory> Other;
};