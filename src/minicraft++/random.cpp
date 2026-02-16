#include <random>

#include <minicraft++/random.hpp>

int MppRandomGetInt(int min, int max)
{
    static std::random_device device;
    static std::mt19937 generator(device());
    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(generator);
}

bool MppRandomGetBool()
{
    static std::random_device device;
    static std::mt19937 generator(device());
    static std::bernoulli_distribution distribution(0.5);
    return distribution(generator);
}
