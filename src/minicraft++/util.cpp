#include <cmath>
#include <random>

#include <minicraft++/util.hpp>

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

int MppGetRandomInt(int min, int max)
{
    static std::random_device device;
    static std::mt19937 generator(device());
    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(generator);
}

bool MppGetRandomBool()
{
    static std::random_device device;
    static std::mt19937 generator(device());
    static std::bernoulli_distribution distribution(0.5);
    return distribution(generator);
}
