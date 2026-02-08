#pragma once

#include <SDL3/SDL.h>

#ifndef NDEBUG
#define MppAssert(e) SDL_assert_always(e)
#else
#define MppAssert(e)
#endif