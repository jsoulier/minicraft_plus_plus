#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "log.hpp"
#include "state.hpp"

int main(int argc, char** argv)
{
    if (!MppStateInit())
    {
        MppLog("Failed to initialize state");
        return 1;
    }
    while (MppStateTick()) {}
    MppStateQuit();
    return 0;
}