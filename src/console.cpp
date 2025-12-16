#include "console.hpp"

#define VARIABLE(N, V) \
    decltype(MppConsole##N) MppConsole##N{V}; \
    static Variable N(MppConsole##N); \

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

template<typename T>
Variable(T& value) -> Variable<T>;

VARIABLE(DebugFrustum, false)
VARIABLE(DebugPhysics, false)