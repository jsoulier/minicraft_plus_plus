#include "world.hpp"

static constexpr int kSize = 256;

bool MppWorldInit()
{
    return true;
}

void MppWorldQuit()
{
}

int MppWorldGetSize()
{
    return kSize;
}
