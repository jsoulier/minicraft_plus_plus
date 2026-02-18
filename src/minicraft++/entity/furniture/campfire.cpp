#include <minicraft++/color.hpp>
#include <minicraft++/entity/furniture/campfire.hpp>
#include <minicraft++/item.hpp>
#include <minicraft++/renderer.hpp>

void MppCampfireEntity::Render() const
{
    MppFurnitureEntity::Render();
    static constexpr int kColor = 553;
    static constexpr int kRadius = 32;
    static constexpr int kStrength = 5;
    auto [x, y] = GetCenter();
    MppRendererDrawLight(kColor, x, y, kRadius, kStrength);
}

MppItemID MppCampfireEntity::GetItemID() const
{
    return MppItemIDCampfire;
}

int MppCampfireEntity::GetAnimationTickRate() const
{
    return 10;
}

int MppCampfireEntity::GetAnimationPose1X() const
{
    return 4;
}

int MppCampfireEntity::GetAnimationPose1Y() const
{
    return 18;
}

int MppCampfireEntity::GetAnimationPose2X() const
{
    return 5;
}

int MppCampfireEntity::GetAnimationPose2Y() const
{
    return 18;
}

int MppCampfireEntity::GetAnimationPose3X() const
{
    return 6;
}

int MppCampfireEntity::GetAnimationPose3Y() const
{
    return 18;
}

int MppCampfireEntity::GetAnimationPose4X() const
{
    return 7;
}

int MppCampfireEntity::GetAnimationPose4Y() const
{
    return 18;
}

int MppCampfireEntity::GetColor1() const
{
    return kMppColorCampfire1;
}

int MppCampfireEntity::GetColor2() const
{
    return kMppColorCampfire2;
}

int MppCampfireEntity::GetColor3() const
{
    return kMppColorCampfire3;
}

int MppCampfireEntity::GetColor4() const
{
    return kMppColorCampfire4;
}

int MppCampfireEntity::GetColor5() const
{
    return kMppColorCampfire5;
}

