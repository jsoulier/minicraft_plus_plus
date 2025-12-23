#include "console.hpp"

#define VARIABLE(N, V) \
    using N##Type = decltype(MppConsole##N); \
    N##Type MppConsole##N{V}; \
    static Variable<N##Type> N(MppConsole##N); \

template<typename T>
class Variable
{
public:
    Variable(T& value)
        : Value{value}
    {
    }

private:
    T& Value;
};

VARIABLE(DebugScreens, false)
VARIABLE(DebugFrustum, false)
VARIABLE(DebugPhysics, false)