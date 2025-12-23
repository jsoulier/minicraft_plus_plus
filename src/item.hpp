#pragma once

#include <savepoint_fwd.hpp>

#include <string_view>

#include "renderer.hpp"

enum MppItemID
{
    MppItemIDWood,
    MppItemIDIronArmor,
    MppItemIDCount,
    MppItemIDInvalid = MppItemIDCount,
};

enum MppItemType
{
    MppItemTypeMaterial  = 0x01,
    MppItemTypeHeld      = 0x02,
    MppItemTypeArmor     = 0x04,
    MppItemTypeStackable = ~(MppItemTypeHeld | MppItemTypeArmor),
};

enum MppItemToolType
{
    MppItemToolTypeNone,
    MppItemToolTypePickaxe,
    MppItemToolTypeAxe,
    MppItemToolTypeShovel,
    MppItemToolTypeSword,
    MppItemToolTypeHoe,
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
    MppItemType GetType() const;
    MppItemToolType GetToolType() const;
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