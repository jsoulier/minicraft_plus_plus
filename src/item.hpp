#pragma once

#include <savepoint_fwd.hpp>

#include "renderer.hpp"

enum MppItemID
{
    MppItemIDCount,
    MppItemIDInvalid = MppItemIDCount,
};

enum MppItemType
{
    MppItemTypeMaterial = 0x01,
    MppItemTypeTool     = 0x02,
    MppItemTypeArmor    = 0x04,
};

class MppItem
{
public:
    static constexpr int kWidth = 8;

    constexpr MppItem() : ID{MppItemIDInvalid} {}
    MppItem(MppItemID id);
    void Visit(SavepointVisitor& visitor);
    int GetColor1() const;
    int GetColor2() const;
    int GetColor3() const;
    int GetColor4() const;
    int GetColor5() const;
    int GetPhysicsOffsetX() const;
    int GetPhysicsOffsetY() const;
    int GetPhysicsWidth() const;
    int GetPhysicsHeight() const;
    bool operator==(const MppItem& other) const;
    bool IsValid() const;

private:
    MppItemID ID;
};

static constexpr MppItem kMppItemInvalid{};