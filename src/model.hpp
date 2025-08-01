#pragma once

#define MPP_MODELS \
    X(Default)

enum MppModel
{
#define X(name) MppModel##name,
    MPP_MODELS
#undef X
    MppModelCount,
};

const char* MppModelToString(int model);