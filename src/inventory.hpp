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
{
public:
    MppInventory();
    void Visit(SavepointVisitor& visitor);
    void Render() const override;
    bool Add(const MppItem& item);
    int GetIndex() const;
    MppItem& Get(int index);
    MppItem Remove(int index);
    void SetMaxItems(int max);
    bool IsEmpty() const;
    void OnAction() override;
    void OnDrop() override;
    void OnUpArrow() override;
    void OnDownArrow() override;
    void OnHeldUp() override;
    void OnHeldDown() override;
    void OnRender() override;
    void SetOnActionCallback(const std::function<void(int index)>& callback);
    void SetOnDropCallback(const std::function<void(int index)>& callback);
    void SetIsFocused(bool focused);
    bool IsFocused() const;

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
