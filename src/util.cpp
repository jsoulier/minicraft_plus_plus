#include <cmath>

#include "util.hpp"

int MppGetNumberOfDigits(int value)
{
    if (value == 0)
    {
        return 1;
    }
    value = std::abs(value);
    int count = 0;
    while (value > 0)
    {
        value /= 10;
        count++;
    }
    return count;
}
