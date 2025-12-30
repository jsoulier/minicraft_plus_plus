#pragma once

#include <savepoint/fwd.hpp>

#include <string_view>

enum MppItemID
{
    MppItemIDWood,
    MppItemIDApple,
    MppItemIDIronHelmet,
    MppItemIDIronChestplate,
    MppItemIDIronLeggings,
    MppItemIDIronBoots,
    MppItemIDCount,
    MppItemIDInvalid = MppItemIDCount,
};

enum MppItemFlag
{
    MppItemFlagMaterial   = 0x01,
    MppItemFlagConsumable = 0x02,
    MppItemFlagHeld       = 0x04,
    MppItemFlagArmor      = 0x08,
    MppItemFlagStackable  = ~(MppItemFlagHeld | MppItemFlagArmor),
};

enum MppItemType
{
    MppItemTypeNone,
    MppItemTypePickaxe,
    MppItemTypeAxe,
    MppItemTypeShovel,
    MppItemTypeSword,
    MppItemTypeHoe,
    MppItemTypeChestplate,
    MppItemTypeHelmet,
    MppItemTypeBoots,
    MppItemTypeLeggings,
};

class MppItem
{
public:
    static constexpr int kWidth = 8;

    MppItem() : ID{MppItemIDInvalid} {}
    MppItem(MppItemID id);
    void Visit(SavepointVisitor& visitor);
    void AddItem();
    void RemoveItem();
    int GetItems() const;
    std::string_view GetName() const;
    MppItemID GetID() const;
    MppItemFlag GetFlag() const;
    MppItemType GetType() const;
    int GetColor1() const;
    int GetColor2() const;
    int GetColor3() const;
    int GetColor4() const;
    int GetColor5() const;
    int GetSpriteX() const;
    int GetSpriteY() const;
    int GetPhysicsOffsetX() const;
    int GetPhysicsOffsetY() const;
    int GetPhysicsWidth() const;
    int GetPhysicsHeight() const;
    bool operator==(const MppItem& other) const;
    bool IsValid() const;

private:
    MppItemID ID;
    int Count;
};

static const MppItem kMppItemInvalid;