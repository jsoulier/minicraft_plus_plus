#pragma once

#include <savepoint/savepoint.hpp>

#include <array>
#include <functional>
#include <vector>

#include "input.hpp"
#include "item.hpp"
#include "menu.hpp"

enum MppInventorySlot
{
    MppInventorySlotHeld,
    MppInventorySlotHelmet,
    MppInventorySlotCuirass,
    MppInventorySlotLeggings,
    MppInventorySlotBoots,
    MppInventorySlotCount,
};

class MppInventory
    : public MppMenu
    , public MppInputHandler
    , public SavepointBase
{
    SAVEPOINT_DERIVED(MppInventory)

public:
    MppInventory();
    void Visit(SavepointVisitor& visitor);
    void Render() const;
    bool Add(const MppItem& item);
    int GetIndex() const;
    const MppItem& Get(int index);
    MppItem Remove(int index, int count = 1);
    const MppItem& GetByID(MppItemID id);
    MppItem RemoveByID(MppItemID id, int count);
    void SetMaxItems(int max);
    bool IsEmpty() const;
    void OnAction() override;
    void OnDrop() override;
    void OnUpArrow() override;
    void OnDownArrow() override;
    void OnRender() override;
    void SetOnActionCallback(const std::function<void(int index)>& callback);
    void SetOnDropCallback(const std::function<void(int index)>& callback);
    void SetIsFocused(bool focused);
    bool IsFocused() const;
    void Clear();
    // TODO: dangerous
    void ClearItems();

private:
    std::vector<MppItem> Items;
    std::array<int, MppInventorySlotCount> Slots;
    std::function<void(int index)> OnActionCallback;
    std::function<void(int index)> OnDropCallback;
    int MaxItems;
    int Top;
    int Index;
    bool Focused;
};
