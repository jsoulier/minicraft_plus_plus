#include <minicraft++/color.hpp>
#include <minicraft++/entity/furniture/lantern.hpp>
#include <minicraft++/item.hpp>
#include <minicraft++/renderer.hpp>

void MppLanternEntity::Render() const
{
    MppFurnitureEntity::Render();
    static constexpr int kColor = 553;
    static constexpr int kRadius = 64;
    static constexpr int kStrength = 5;
    auto [x, y] = GetCenter();
    MppRendererDrawLight(kColor, x, y, kRadius, kStrength);
}

MppItemID MppLanternEntity::GetItemID() const
{
    return MppItemIDLantern;
}

int MppLanternEntity::GetSpriteX() const
{
    return 4;
}

int MppLanternEntity::GetSpriteY() const
{
    return 17;
}

int MppLanternEntity::GetColor1() const
{
    return kMppColorLantern1;
}

int MppLanternEntity::GetColor2() const
{
    return kMppColorLantern2;
}

int MppLanternEntity::GetColor3() const
{
    return kMppColorLantern3;
}

int MppLanternEntity::GetColor4() const
{
    return kMppColorLantern4;
}

int MppLanternEntity::GetColor5() const
{
    return kMppColorLantern5;
}
