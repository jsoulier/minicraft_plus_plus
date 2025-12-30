#include "item.hpp"
#include "workbench_entity.hpp"

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
    return 0;
}

int MppWorkbenchEntity::GetColor1() const
{
    return 0;
}

int MppWorkbenchEntity::GetColor2() const
{
    return 0;
}

int MppWorkbenchEntity::GetColor3() const
{
    return 0;
}

int MppWorkbenchEntity::GetColor4() const
{
    return 0;
}

int MppWorkbenchEntity::GetColor5() const
{
    return 0;
}
