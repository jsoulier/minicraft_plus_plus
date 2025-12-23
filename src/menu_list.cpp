// TODO: untested

#include <SDL3/SDL.h>

#include <algorithm>

#include "menu_list.hpp"

MppMenuList::MppMenuList()
    : MppMenu()
    , Index{-1}
    , First{0}
    , Max{0}
{
}

void MppMenuList::Draw(MppRenderer& renderer)
{
    MppMenu::Draw(renderer);
    int rowHeight = GetRowHeight();
    int rows = (Max - First);
    int visibleRows = GetContentHeight() / rowHeight;
    int y = GetContentY();
    for (int i = 0; i < std::min(rows, visibleRows); i++)
    {
        Draw(renderer, y, i);
        y += rowHeight;
    }
}

void MppMenuList::Add()
{
    Max++;
    Index = std::max(Index, 0);
}

void MppMenuList::Remove(int index)
{
    SDL_assert(Max > 0);
    Max--;
    if (Max == 0)
    {
        Index = -1;
    }
    if (index <= First)
    {
        First = std::max(First - 1, 0);
    }
    if (index <= Index)
    {
        Index = std::max(Index - 1, 0);
    }
}

void MppMenuList::Up()
{
    Index = std::max(Index + 1, 0);
    First = std::min(First, Index);
}

void MppMenuList::Down()
{
    Index = std::min(Index + 1, Max - 1);
    int visibleRows = GetContentHeight() / GetRowHeight();
    First = Index - visibleRows;
    if (Index + visibleRows > First)
    {
        First++;
    }
}