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

bool MppChestEntity::OnInteraction(std::shared_ptr<MppEntity>& instigator)
{
    std::shared_ptr<MppMobEntity> mob = instigator->Cast<MppMobEntity>();
    MppAssert(mob);
    Other = mob->GetInventory();
    Other.lock()->SetIsFocused(true);
    Inventory->SetIsFocused(false);
    MppInputAddHandler(Cast<MppInputHandler>());
    return true;
}

void MppChestEntity::OnInputAction()
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

void MppChestEntity::OnInputRender() const
{
    Other.lock()->Render();
    Inventory->Render();
}

void MppChestEntity::OnInputUpArrow()
{
    if (Other.lock()->IsFocused())
    {
        Other.lock()->OnInputUpArrow();
    }
    else
    {
        Inventory->OnInputUpArrow();
    }
}

void MppChestEntity::OnInputDownArrow()
{
    if (Other.lock()->IsFocused())
    {
        Other.lock()->OnInputDownArrow();
    }
    else
    {
        Inventory->OnInputDownArrow();
    }
}

void MppChestEntity::OnInputLeftArrow()
{
    if (Inventory->IsFocused())
    {
        Other.lock()->SetIsFocused(true);
        Inventory->SetIsFocused(false);
    }
}

void MppChestEntity::OnInputRightArrow()
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

int MppChestEntity::GetAnimationPose1X() const
{
    return 2;
}

int MppChestEntity::GetAnimationPose1Y() const
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

void MppChestEntity::OnInputLoseFocus()
{
    Other.lock()->SetIsFocused(true);
    Inventory->SetIsFocused(false);
}
