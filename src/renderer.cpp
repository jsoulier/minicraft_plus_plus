#include <SDL3/SDL.h>

#include <cstdint>

#include "camera.hpp"
#include "debug_group.hpp"
#include "model.hpp"
#include "obj_model.hpp"
#include "pipeline.hpp"
#include "renderer.hpp"
#include "shader.hpp"
#include "time.hpp"
#include "transform.hpp"
#include "upload_buffer.hpp"

enum Texture
{
    TextureDepth,
    TextureCount,
};

enum GraphicsPipeline
{
    GraphicsPipelineObjModel,
    GraphicsPipelineCount,
};

enum Camera
{
    CameraPov,
    CameraCount,
};

static SDL_GPUDevice* device;
static SDL_GPUTexture* textures[TextureCount];
static SDL_GPUGraphicsPipeline*  graphicsPipelines[GraphicsPipelineCount];
static SDL_GPUCommandBuffer* commandBuffer;
static SDL_GPUTexture* swapchainTexture;
static uint32_t swapchainWidth;
static uint32_t swapchainHeight;
static MppCamera cameras[CameraCount];
static MppObjModel objModels[MppModelCount];

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
    SDL_GPUCommandBuffer* commandBuffer = SDL_AcquireGPUCommandBuffer(device);
    if (!commandBuffer)
    {
        SDL_Log("Failed to acquire command buffer: %s", SDL_GetError());
        return false;
    }
    SDL_GPUCopyPass* copyPass = SDL_BeginGPUCopyPass(commandBuffer);
    if (!copyPass)
    {
        SDL_Log("Failed to begin copy pass: %s", SDL_GetError());
        return false;
    }
    graphicsPipelines[GraphicsPipelineObjModel] = MppCreateObjModelPipeline(device, window);
    for (int i = GraphicsPipelineCount - 1; i >= 0; i--)
    {
        if (!graphicsPipelines[i])
        {
            SDL_Log("Failed to create graphics pipeline: %d, %s", i, SDL_GetError());
            return false;
        }
    }
    for (int i = 0; i < MppModelCount; i++)
    {
        if (!objModels[i].Load(device, copyPass, MppModelToString(i)))
        {
            SDL_Log("Failed to load obj model: %s", SDL_GetError());
            return false;
        }
    }
    SDL_EndGPUCopyPass(copyPass);
    SDL_SubmitGPUCommandBuffer(commandBuffer);
    return true;
}

void MppRendererQuit(SDL_Window* window)
{
    for (int i = 0; i < MppModelCount; i++)
    {
        objModels[i].Destroy(device);
    }
    for (int i = 0; i < GraphicsPipelineCount; i++)
    {
        SDL_ReleaseGPUGraphicsPipeline(device, graphicsPipelines[i]);
    }
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
    cameras[CameraPov].SetViewport(swapchainWidth, swapchainHeight);
}

void MppRendererUpdate(const MppTransform& target, const MppTime& time)
{
    cameras[CameraPov].Update(target, time);
}

void MppRendererEndFrame()
{
    SDL_SubmitGPUCommandBuffer(commandBuffer);
}