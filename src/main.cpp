#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "renderer.hpp"
#include "time.hpp"
#include "transform.hpp"

static constexpr const char* kAppName = "Minicraft Plus Plus";
static constexpr const char* kWindowTitle = "Minicraft++";
static constexpr int kWindowWidth = 960;
static constexpr int kWindowHeight = 720;

int main(int argc, char** argv)
{
    SDL_SetAppMetadata(kAppName, nullptr, nullptr);
    SDL_SetLogPriorities(SDL_LOG_PRIORITY_VERBOSE);
    SDL_Window* window = SDL_CreateWindow(kWindowTitle, kWindowWidth, kWindowHeight, SDL_WINDOW_RESIZABLE);
    if (!window)
    {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return 1;
    }
    if (!MppRendererInit(window))
    {
        SDL_Log("Failed to initialize renderer");
        return 1;
    }
    MppTime time{0};
    bool running = true;
    while (running)
    {
        time.Tick();
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
        MppRendererBeginFrame(window);
        MppRendererUpdate({} /* TODO: player transform */, time);
        /* TODO: update world */
        MppRendererDraw(MppModelDefault, MppTransform{glm::vec3{0.0f, 0.0f, -16.0f}, 0.0f});
        MppRendererDraw(MppModelDefault, MppTransform{glm::vec3{32.0f, 0.0f, -16.0f}, 0.0f});
        MppRendererDraw(MppModelDefault, MppTransform{glm::vec3{-32.0f, 0.0f, -16.0f}, 0.0f});
        MppRendererDraw(MppModelDefault, MppTransform{glm::vec3{0.0f, -16.0f, 16.0f}, 0.0f});
        MppRendererEndFrame();
    }
    MppRendererQuit(window);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}