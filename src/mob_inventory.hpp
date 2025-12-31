#pragma once

#include <savepoint/fwd.hpp>

#include <vector>

#include "inventory.hpp"
#include "item.hpp"

class MppRenderer;

class MppMobInventory : public MppInventory
{
public:
    class Slot
    {
        friend class MppMobInventory;

    public:
        Slot();
        void Visit(SavepointVisitor& visitor);
        int GetIndex() const;
        bool operator==(const Slot other) const;
        bool IsValid() const;

    public:
        int Index;
    };

    MppMobInventory(int slots);
    void Visit(SavepointVisitor& visitor);

protected:
    void Draw(MppRenderer& renderer, int y, int index) override;
    const MppItem* GetItemFromSlot(int slot) const;
    void SetSlot(int slot);
    void RemoveInternal(int index) override;
    virtual void OnSetSlot(int position, const Slot& slot) {}

private:
    std::vector<Slot> Slots;
};