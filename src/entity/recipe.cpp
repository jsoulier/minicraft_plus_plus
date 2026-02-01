#include <savepoint/savepoint.hpp>

#include <memory>

#include "../color.hpp"
#include "../input.hpp"
#include "../inventory.hpp"
#include "../item.hpp"
#include "../log.hpp"
#include "mob.hpp"
#include "recipe.hpp"

MppRecipeEntity::MppRecipeEntity()
    : Inventory{std::make_shared<MppInventory>()}
{
    Inventory->SetIsFocused(true);
    Inventory->SetX2(124);
    Inventory->SetY2(120);
    Inventory->SetMaxItems(MppItemIDCount);
}

void MppRecipeEntity::OnAction(MppEntity& instigator)
{
    MppFurnitureEntity::OnAction(instigator);
    MppMobEntity* mob = dynamic_cast<MppMobEntity*>(&instigator);
    if (!mob)
    {
        MppLog("Instigator wasn't an MppMobEntity");
        return;
    }
    Other = mob->GetInventory();
    Build();
    MppInputSetInteraction(std::dynamic_pointer_cast<MppInputHandler>(shared_from_this()));
}

void MppRecipeEntity::OnAction()
{
    if (Inventory->IsEmpty())
    {
        return;
    }
    const MppItem& item = Inventory->Get(Inventory->GetIndex());
    MppItemRecipe recipe = item.GetRecipe();
    if (!recipe.CanCraft(GetItemID(), Other.lock()))
    {
        return;
    }
    recipe.Craft(Other.lock());
    Other.lock()->Add(item);
    Build();
}

void MppRecipeEntity::OnUpArrow()
{
    Inventory->OnUpArrow();
}

void MppRecipeEntity::OnDownArrow()
{
    Inventory->OnDownArrow();
}

void MppRecipeEntity::OnRender()
{
    Inventory->Render();
    if (Inventory->IsEmpty())
    {
        return;
    }
    const MppItem& item = Inventory->Get(Inventory->GetIndex());
    MppItemRecipe recipe = item.GetRecipe();
    recipe.Render();
}

void MppRecipeEntity::Build()
{
    Inventory->Clear();
    // TODO: tons of hidden nested loops
    for (int i = 0; i < MppItemIDCount; i++)
    {
        MppItem item{MppItemID(i)};
        MppItemRecipe recipe = item.GetRecipe();
        if (!recipe.CanCraft(GetItemID()))
        {
            continue;
        }
        if (!recipe.CanCraft(GetItemID(), Other.lock()))
        {
            item.Remove();
        }
        Inventory->Add(item);
    }
}
