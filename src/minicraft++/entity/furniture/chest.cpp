#include <savepoint/savepoint.hpp>

#include <algorithm>
#include <memory>

#include <minicraft++/assert.hpp>
#include <minicraft++/color.hpp>
#include <minicraft++/input.hpp>
#include <minicraft++/inventory.hpp>
#include <minicraft++/item.hpp>
#include <minicraft++/log.hpp>
#include <minicraft++/entity/furniture/chest.hpp>
#include <minicraft++/entity/mob/mob.hpp>

static constexpr int kMaxItems = 32;

MppChestEntity::MppChestEntity()
    : Inventory{std::make_shared<MppInventory>()}
{
    Inventory->SetX1(130);
    Inventory->SetY1(4);
    Inventory->SetX2(246);
    Inventory->SetY2(124);
    Inventory->SetMaxItems(kMaxItems);
}

void MppChestEntity::Visit(SavepointVisitor& visitor)
{
    MppFurnitureEntity::Visit(visitor);
    visitor(Inventory);
}

bool MppChestEntity::OnInteraction(MppEntity& instigator)
{
    MppMobEntity* mob = dynamic_cast<MppMobEntity*>(&instigator);
    MppAssert(mob);
    Other = mob->GetInventory();
    Other.lock()->SetIsFocused(true);
    Inventory->SetIsFocused(false);
    MppInputSetInteraction(std::dynamic_pointer_cast<MppInputHandler>(shared_from_this()));
    return true;
}

void MppChestEntity::OnAction()
{
    std::shared_ptr<MppInventory> from;
    std::shared_ptr<MppInventory> to;
    if (Other.lock()->IsFocused())
    {
        from = Other.lock();
        to = Inventory;
    }
    else
    {
        from = Inventory;
        to = Other.lock();
    }
    if (!from->IsEmpty())
    {
        to->Add(from->Remove(from->GetIndex()));
    }
}

void MppChestEntity::OnRender()
{
    Other.lock()->Render();
    Inventory->Render();
}

void MppChestEntity::OnUpArrow()
{
    if (Other.lock()->IsFocused())
    {
        Other.lock()->OnUpArrow();
    }
    else
    {
        Inventory->OnUpArrow();
    }
}

void MppChestEntity::OnDownArrow()
{
    if (Other.lock()->IsFocused())
    {
        Other.lock()->OnDownArrow();
    }
    else
    {
        Inventory->OnDownArrow();
    }
}

void MppChestEntity::OnLeftArrow()
{
    if (Inventory->IsFocused())
    {
        Other.lock()->SetIsFocused(true);
        Inventory->SetIsFocused(false);
    }
}

void MppChestEntity::OnRightArrow()
{
    if (Other.lock()->IsFocused())
    {
        Other.lock()->SetIsFocused(false);
        Inventory->SetIsFocused(true);
    }
}

MppItemID MppChestEntity::GetItemID() const
{
    return MppItemIDChest;
}

bool MppChestEntity::IsEmpty() const
{
    return Inventory->IsEmpty();
}

int MppChestEntity::GetSpriteX() const
{
    return 2;
}

int MppChestEntity::GetSpriteY() const
{
    return 17;
}

int MppChestEntity::GetColor1() const
{
    return kMppColorChest1;
}

int MppChestEntity::GetColor2() const
{
    return kMppColorChest2;
}

int MppChestEntity::GetColor3() const
{
    return kMppColorChest3;
}

int MppChestEntity::GetColor4() const
{
    return kMppColorChest4;
}

int MppChestEntity::GetColor5() const
{
    return kMppColorChest5;
}

void MppChestEntity::OnLoseFocus()
{
    Other.lock()->SetIsFocused(true);
    Inventory->SetIsFocused(false);
}
