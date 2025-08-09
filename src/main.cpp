#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "window.hpp"

int main(int argc, char** argv)
{
    SDL_SetLogPriorities(SDL_LOG_PRIORITY_VERBOSE);
    SDL_SetAppMetadata("minicraft++", nullptr, nullptr);
    if (!MppCreateWindow())
    {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return 1;
    }
    bool running = true;
    while (running)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_EVENT_QUIT:
                running = false;
                break;
            }
        }
        MppClearWindow();
        MppDraw("fps = 1 billion", 128, 32, kGreen, 4);
        MppPresentWindow();
    }
    MppDestroyWindow();
    return 0;
}