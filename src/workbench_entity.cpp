#include <string_view>

#include "color.hpp"
#include "item.hpp"
#include "workbench_entity.hpp"

MppItemID MppWorkbenchEntity::GetItemID() const
{
    return MppItemIDWorkbench;
}

std::string_view MppWorkbenchEntity::GetName() const
{
    return "WORKBENCH";
}

MppItemRecipeType MppWorkbenchEntity::GetRecipeType() const
{
    return MppItemRecipeTypeWorkbench;
}

int MppWorkbenchEntity::GetSpriteX() const
{
    return 0;
}

int MppWorkbenchEntity::GetSpriteY() const
{
    return 17;
}

int MppWorkbenchEntity::GetColor1() const
{
    return kMppColorWorkbench1;
}

int MppWorkbenchEntity::GetColor2() const
{
    return kMppColorWorkbench2;
}

int MppWorkbenchEntity::GetColor3() const
{
    return kMppColorWorkbench3;
}

int MppWorkbenchEntity::GetColor4() const
{
    return kMppColorWorkbench4;
}

int MppWorkbenchEntity::GetColor5() const
{
    return kMppColorWorkbench5;
}
