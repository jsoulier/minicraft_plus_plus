#pragma once

#include "renderer.hpp"
#include "world.hpp"

class MppApp
{
public:
    bool Init();
    void Quit();
    bool Update();
    bool Handle(SDL_Event* event);

private:
    MppRenderer Renderer;
    MppWorld World;
};