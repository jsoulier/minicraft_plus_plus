#include <SDL3/SDL.h>

#include <cstdint>

#include "camera.hpp"
#include "debug_group.hpp"
#include "obj_model.hpp"
#include "renderer.hpp"
#include "shader.hpp"
#include "time.hpp"
#include "transform.hpp"
#include "upload_buffer.hpp"

static SDL_GPUDevice* device;
static SDL_GPUCommandBuffer* commandBuffer;
static SDL_GPUTexture* swapchainTexture;
static uint32_t swapchainWidth;
static uint32_t swapchainHeight;

static bool CreateDevice()
{
    SDL_PropertiesID properties = SDL_CreateProperties();
    SDL_SetBooleanProperty(properties, SDL_PROP_GPU_DEVICE_CREATE_PREFERLOWPOWER_BOOLEAN, true);
#ifndef NDEBUG
    SDL_SetBooleanProperty(properties, SDL_PROP_GPU_DEVICE_CREATE_DEBUGMODE_BOOLEAN, true);
    SDL_SetBooleanProperty(properties, SDL_PROP_GPU_DEVICE_CREATE_VERBOSE_BOOLEAN, true);
#endif
#if SDL_PLATFORM_WIN32
    /* TODO: waiting on https://github.com/libsdl-org/SDL/issues/12056 */
#ifndef NDEBUG
    SDL_SetBooleanProperty(properties, SDL_PROP_GPU_DEVICE_CREATE_SHADERS_SPIRV_BOOLEAN, true);
#else
    SDL_SetBooleanProperty(properties, SDL_PROP_GPU_DEVICE_CREATE_SHADERS_DXIL_BOOLEAN, true);
#endif // NDEBUG
#elif
    SDL_SetBooleanProperty(properties, SDL_PROP_GPU_DEVICE_CREATE_SHADERS_METAL_BOOLEAN, true);
#else
    SDL_SetBooleanProperty(properties, SDL_PROP_GPU_DEVICE_CREATE_SHADERS_SPIRV_BOOLEAN, true);
#endif
    device = SDL_CreateGPUDeviceWithProperties(properties);
    if (!device)
    {
        SDL_Log("Failed to create device: %s", SDL_GetError());
        return false;
    }
    return true;
}

bool MppRendererInit(SDL_Window* window)
{
    if (!CreateDevice())
    {
        SDL_Log("Failed to create device");
        return false;
    }
    if (!SDL_ClaimWindowForGPUDevice(device, window))
    {
        SDL_Log("Failed to claim window: %s", SDL_GetError());
        return false;
    }
    return true;
}

void MppRendererQuit(SDL_Window* window)
{
    SDL_ReleaseWindowFromGPUDevice(device, window);
    SDL_DestroyGPUDevice(device);
}

void MppRendererBeginFrame(SDL_Window* window)
{
    SDL_WaitForGPUSwapchain(device, window);
    commandBuffer = SDL_AcquireGPUCommandBuffer(device);
    if (!commandBuffer)
    {
        SDL_Log("Failed to acquire command buffer: %s", SDL_GetError());
        return;
    }
    if (!SDL_AcquireGPUSwapchainTexture(commandBuffer, window, &swapchainTexture, &swapchainWidth, &swapchainHeight))
    {
        SDL_Log("Failed to acquire swapchain texture: %s", SDL_GetError());
        SDL_CancelGPUCommandBuffer(commandBuffer);
        return;
    }
    if (!swapchainTexture || !swapchainWidth || !swapchainHeight)
    {
        /* not an error. happens on minimize */
        SDL_CancelGPUCommandBuffer(commandBuffer);
        return;
    }
}

void MppRendererUpdate(const MppTransform& target, const MppTime& time)
{

}

void MppRendererEndFrame(SDL_Window* window)
{
    SDL_SubmitGPUCommandBuffer(commandBuffer);
}