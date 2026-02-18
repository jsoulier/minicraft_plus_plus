#include <memory>

#include <minicraft++/color.hpp>
#include <minicraft++/entity/furniture/furnace.hpp>
#include <minicraft++/item.hpp>
#include <minicraft++/renderer.hpp>

MppFurnaceEntity::MppFurnaceEntity()
    : MppRecipeEntity()
{
}

void MppFurnaceEntity::Render() const 
{
    MppRecipeEntity::Render();
    static constexpr int kColor = 553;
    static constexpr int kRadius = 16;
    static constexpr int kStrength = 5;
    auto [x, y] = GetCenter();
    MppRendererDrawLight(kColor, x, y, kRadius, kStrength);
}

MppItemID MppFurnaceEntity::GetItemID() const
{
    return MppItemIDFurnace;
}

int MppFurnaceEntity::GetSpriteX() const
{
    return 1;
}

int MppFurnaceEntity::GetSpriteY() const
{
    return 17;
}

int MppFurnaceEntity::GetColor1() const
{
    return kMppColorFurnace1;
}

int MppFurnaceEntity::GetColor2() const
{
    return kMppColorFurnace2;
}

int MppFurnaceEntity::GetColor3() const
{
    return kMppColorFurnace3;
}

int MppFurnaceEntity::GetColor4() const
{
    return kMppColorFurnace4;
}

int MppFurnaceEntity::GetColor5() const
{
    return kMppColorFurnace5;
}
