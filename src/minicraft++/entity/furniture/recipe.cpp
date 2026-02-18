#include <savepoint/savepoint.hpp>

#include <memory>

#include <minicraft++/assert.hpp>
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

bool MppRecipeEntity::OnInteraction(std::shared_ptr<MppEntity>& instigator)
{
    MppFurnitureEntity::OnInteraction(instigator);
    std::shared_ptr<MppMobEntity> mob = instigator->Cast<MppMobEntity>();
    MppAssert(mob);
    Other = mob->GetInventory();
    Build();
    MppInputAddHandler(Cast<MppInputHandler>());
    return true;
}

void MppRecipeEntity::OnInputAction()
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

void MppRecipeEntity::OnInputUpArrow()
{
    Inventory->OnInputUpArrow();
}

void MppRecipeEntity::OnInputDownArrow()
{
    Inventory->OnInputDownArrow();
}

void MppRecipeEntity::OnInputRender() const
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
