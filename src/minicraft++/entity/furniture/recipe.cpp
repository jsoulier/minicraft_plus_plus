#include <savepoint/savepoint.hpp>

#include <memory>

#include <minicraft++/assert.hpp>
#include <minicraft++/color.hpp>
#include <minicraft++/input.hpp>
#include <minicraft++/inventory.hpp>
#include <minicraft++/item.hpp>
#include <minicraft++/log.hpp>
#include <minicraft++/entity/mob/mob.hpp>
#include <minicraft++/entity/furniture/recipe.hpp>

MppRecipeEntity::MppRecipeEntity()
    : Inventory{std::make_shared<MppInventory>()}
{
    Inventory->SetIsFocused(true);
    Inventory->SetX1(4);
    Inventory->SetY1(4);
    Inventory->SetX2(126);
    Inventory->SetY2(124);
    Inventory->SetMaxItems(MppItemIDCount);
}

bool MppRecipeEntity::OnInteraction(MppEntity& instigator)
{
    MppMobEntity* mob = dynamic_cast<MppMobEntity*>(&instigator);
    MppAssert(mob);
    Other = mob->GetInventory();
    Build();
    MppInputSetInteraction(std::dynamic_pointer_cast<MppInputHandler>(shared_from_this()));
    return true;
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
