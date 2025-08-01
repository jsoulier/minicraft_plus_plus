#include "fixed_string.hpp"
#include "model.hpp"

static constexpr MppFixedString kStrings[MppModelCount] =
{
#define X(name) MppFixedStringToLower(#name),
    MPP_MODELS
#undef X
};

const char* MppModelToString(int model)
{
    return kStrings[model].data();
}