#include <SDL3/SDL.h>

#include <cstdint>

#include "camera.hpp"
#include "debug_group.hpp"
#include "model.hpp"
#include "obj_model.hpp"
#include "pipeline.hpp"
#include "renderer.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "time.hpp"
#include "transform.hpp"
#include "upload_buffer.hpp"
#include "vox_model.hpp"

enum Texture
{
    TextureDepth,
    TextureCount,
};

enum Sampler
{
    SamplerNearest,
    SamplerCount,
};

enum GraphicsPipeline
{
    GraphicsPipelineObjModel,
    GraphicsPipelineCount,
};

enum ComputePipeline
{
    ComputePipelineCount,
};

enum Camera
{
    CameraPov,
    CameraCount,
};

static SDL_GPUDevice* device;
static SDL_GPUTexture* textures[TextureCount];
static SDL_GPUSampler* samplers[SamplerCount];
static SDL_GPUGraphicsPipeline*  graphicsPipelines[GraphicsPipelineCount];
// static SDL_GPUComputePipeline*  computePipelines[ComputePipelineCount];
static SDL_GPUCommandBuffer* commandBuffer;
static SDL_GPUTexture* swapchainTexture;
static uint32_t povWidth;
static uint32_t povHeight;
static MppCamera cameras[CameraCount];
static MppObjModel objModels[MppModelCount];
static MppVoxModel voxModels[MppModelCount];
static MppVertexUploadBuffer<MppTransform> modelInstances[MppModelCount];

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

static bool CreatePipelines(SDL_GPUCommandBuffer* commandBuffer, SDL_Window* window)
{
    MPP_DEBUG_GROUP(commandBuffer);
    graphicsPipelines[GraphicsPipelineObjModel] = MppCreateObjModelPipeline(device, window);
    for (int i = GraphicsPipelineCount - 1; i >= 0; i--)
    {
        if (!graphicsPipelines[i])
        {
            SDL_Log("Failed to create graphics pipeline: %d, %s", i, SDL_GetError());
            return false;
        }
    }
    // for (int i = ComputePipelineCount - 1; i >= 0; i--)
    // {
    //     if (!computePipelines[i])
    //     {
    //         SDL_Log("Failed to create compute pipeline: %d, %s", i, SDL_GetError());
    //         return false;
    //     }
    // }
    return true;
}

static bool CreateSamplers(SDL_GPUCommandBuffer* commandBuffer)
{
    MPP_DEBUG_GROUP(commandBuffer);
    SDL_GPUSamplerCreateInfo info{};
    info.address_mode_u = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;
    info.address_mode_v = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;
    info.address_mode_w = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;
    info.min_filter = SDL_GPU_FILTER_NEAREST;
    info.mag_filter = SDL_GPU_FILTER_NEAREST;
    info.mipmap_mode = SDL_GPU_SAMPLERMIPMAPMODE_NEAREST;
    samplers[SamplerNearest] = SDL_CreateGPUSampler(device, &info);
    for (int i = SamplerCount - 1; i >= 0; i--)
    {
        if (!samplers[i])
        {
            SDL_Log("Failed to create sampler: %d, %s", i, SDL_GetError());
            return false;
        }
    }
    return true;
}

static bool LoadModels(SDL_GPUCommandBuffer* commandBuffer, SDL_GPUCopyPass* copyPass)
{
    MPP_DEBUG_GROUP(commandBuffer);
    for (int i = 0; i < MppModelCount; i++)
    {
        if (!objModels[i].Load(device, copyPass, MppModelToString(i)))
        {
            SDL_Log("Failed to load obj model: %s", SDL_GetError());
            return false;
        }
        if (!voxModels[i].Load(device, copyPass, MppModelToString(i)))
        {
            SDL_Log("Failed to load vox model: %s", SDL_GetError());
            return false;
        }
    }
    return true;
}

static bool ResizeTextures(SDL_GPUCommandBuffer* commandBuffer, uint32_t width, uint32_t height)
{
    MPP_DEBUG_GROUP(commandBuffer);
    for (int i = 0; i < TextureCount; i++)
    {
        SDL_ReleaseGPUTexture(device, textures[i]);
        textures[i] = nullptr;
    }
    textures[TextureDepth] = MppCreateDepthTexture(device, width, height);
    for (int i = 0; i < TextureDepth; i++)
    {
        if (!textures[i])
        {
            SDL_Log("Failed to create texture: %d", i);
            return false;
        }
    }
    povWidth = width;
    povHeight = height;
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
    if (!CreatePipelines(commandBuffer, window))
    {
        SDL_Log("Failed to create pipelines: %s", SDL_GetError());
        return false;
    }
    if (!CreateSamplers(commandBuffer))
    {
        SDL_Log("Failed to create samplers: %s", SDL_GetError());
        return false;
    }
    if (!LoadModels(commandBuffer, copyPass))
    {
        SDL_Log("Failed to load models: %s", SDL_GetError());
        return false;
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
        voxModels[i].Destroy(device);
        modelInstances[i].Destroy(device);
    }
    for (int i = 0; i < GraphicsPipelineCount; i++)
    {
        SDL_ReleaseGPUGraphicsPipeline(device, graphicsPipelines[i]);
    }
    // for (int i = 0; i < ComputePipelineCount; i++)
    // {
    //     SDL_ReleaseGPUComputePipeline(device, computePipelines[i]);
    // }
    for (int i = 0; i < SamplerCount; i++)
    {
        SDL_ReleaseGPUSampler(device, samplers[i]);
    }
    for (int i = 0; i < TextureCount; i++)
    {
        SDL_ReleaseGPUTexture(device, textures[i]);
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
    uint32_t width;
    uint32_t height;
    if (!SDL_AcquireGPUSwapchainTexture(commandBuffer, window, &swapchainTexture, &width, &height))
    {
        SDL_Log("Failed to acquire swapchain texture: %s", SDL_GetError());
        SDL_CancelGPUCommandBuffer(commandBuffer);
        commandBuffer = nullptr;
        return;
    }
    if (!swapchainTexture || !width || !height)
    {
        /* not an error. happens on minimize */
        SDL_SubmitGPUCommandBuffer(commandBuffer);
        commandBuffer = nullptr;
        return;
    }
    if ((povWidth != width || povHeight != height) && !ResizeTextures(commandBuffer, width, height))
    {
        SDL_Log("Failed to resize textures");
        SDL_SubmitGPUCommandBuffer(commandBuffer);
        commandBuffer = nullptr;
        return;
    }
    cameras[CameraPov].SetViewport(povWidth, povHeight);
}

void MppRendererUpdate(const MppTransform& target, const MppTime& time)
{
    cameras[CameraPov].Update(target, time);
}

void MppRendererDraw(MppModel model, const MppTransform& transform)
{
    modelInstances[model].Emplace(device, transform);
}

static void Upload()
{
    MPP_DEBUG_GROUP(commandBuffer);
    SDL_GPUCopyPass* copyPass = SDL_BeginGPUCopyPass(commandBuffer);
    if (!copyPass)
    {
        SDL_Log("Failed to begin copy pass: %s", SDL_GetError());
        return;
    }
    for (int i = 0; i < MppModelCount; i++)
    {
        modelInstances[i].Upload(device, copyPass);
    }
    SDL_EndGPUCopyPass(copyPass);
}

static void DrawObjModels()
{
    MPP_DEBUG_GROUP(commandBuffer);
    SDL_GPUColorTargetInfo colorInfo{};
    colorInfo.texture = swapchainTexture;
    colorInfo.load_op = SDL_GPU_LOADOP_CLEAR;
    colorInfo.store_op = SDL_GPU_STOREOP_STORE;
    colorInfo.cycle = true;
    SDL_GPUDepthStencilTargetInfo depthInfo{};
    depthInfo.texture = textures[TextureDepth];
    depthInfo.load_op = SDL_GPU_LOADOP_CLEAR;
    depthInfo.stencil_load_op = SDL_GPU_LOADOP_CLEAR;
    depthInfo.store_op = SDL_GPU_STOREOP_STORE;
    depthInfo.clear_depth = 1.0f;
    depthInfo.cycle = true;
    SDL_GPURenderPass* renderPass = SDL_BeginGPURenderPass(commandBuffer, &colorInfo, 1, &depthInfo);
    if (!renderPass)
    {
        SDL_Log("Failed to begin render pass: %s", SDL_GetError());
        return;
    }
    SDL_BindGPUGraphicsPipeline(renderPass, graphicsPipelines[GraphicsPipelineObjModel]);
    SDL_PushGPUVertexUniformData(commandBuffer, 0, &cameras[CameraPov].GetViewProjMatrix(), 64);
    for (int i = 0; i < MppModelCount; i++)
    {
        if (!modelInstances[i].GetSize())
        {
            continue;
        }
        SDL_GPUBufferBinding vertexBuffers[2]{};
        SDL_GPUBufferBinding indexBuffer = objModels[i].GetIndexBufferBinding();
        SDL_GPUTextureSamplerBinding paletteTexture{};
        vertexBuffers[0] = objModels[i].GetVertexBufferBinding();
        vertexBuffers[1] = modelInstances[i].GetBufferBinding();
        paletteTexture.sampler = samplers[SamplerNearest];
        paletteTexture.texture = objModels[i].GetPaletteTexture();
        SDL_BindGPUVertexBuffers(renderPass, 0, vertexBuffers, 2);
        SDL_BindGPUIndexBuffer(renderPass, &indexBuffer, objModels[i].GetIndexElementSize());
        SDL_BindGPUFragmentSamplers(renderPass, 0, &paletteTexture, 1);
        SDL_DrawGPUIndexedPrimitives(renderPass, objModels[i].GetIndexCount(), modelInstances[i].GetSize(), 0, 0, 0);
    }
    SDL_EndGPURenderPass(renderPass);
}

void MppRendererEndFrame()
{
    if (!commandBuffer)
    {
        SDL_Log("Skipping frame");
        return;
    }
    Upload();
    DrawObjModels();
    SDL_SubmitGPUCommandBuffer(commandBuffer);
}