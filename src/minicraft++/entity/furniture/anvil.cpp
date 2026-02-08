#include <savepoint/savepoint.hpp>

#include <minicraft++/color.hpp>
#include <minicraft++/entity/furniture/anvil.hpp>
#include <minicraft++/item.hpp>

MppAnvilEntity::MppAnvilEntity()
    : MppRecipeEntity()
{
}

MppItemID MppAnvilEntity::GetItemID() const
{
    return MppItemIDAnvil;
}

int MppAnvilEntity::GetSpriteX() const
{
    return 3;
}

int MppAnvilEntity::GetSpriteY() const
{
    return 17;
}

int MppAnvilEntity::GetColor1() const
{
    return kMppColorAnvil1;
}

int MppAnvilEntity::GetColor2() const
{
    return kMppColorAnvil2;
}

int MppAnvilEntity::GetColor3() const
{
    return kMppColorAnvil3;
}

int MppAnvilEntity::GetColor4() const
{
    return kMppColorAnvil4;
}

int MppAnvilEntity::GetColor5() const
{
    return kMppColorAnvil5;
}
