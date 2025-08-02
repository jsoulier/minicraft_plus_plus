#include <SDL3/SDL.h>

#include <cstdint>

#include "camera.hpp"
#include "debug_group.hpp"
#include "log.hpp"
#include "model.hpp"
#include "obj_model.hpp"
#include "pipeline.hpp"
#include "renderer.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "ticks.hpp"
#include "transform.hpp"
#include "upload_buffer.hpp"
#include "vox_model.hpp"
#include "window.hpp"

bool MppRenderer::Init(MppWindow& window)
{
    if (!CreateDevice())
    {
        MppLog("Failed to create device");
        return false;
    }
    if (!SDL_ClaimWindowForGPUDevice(Device, window.GetHandle()))
    {
        MppLog("Failed to claim window: %s", SDL_GetError());
        return false;
    }
    if (!CreateResources(window))
    {
        MppLog("Failed to create resources: %s", SDL_GetError());
        return false;
    }
    return true;
}

void MppRenderer::Quit(MppWindow& window)
{
    for (int i = 0; i < MppModelCount; i++)
    {
        ObjModels[i].Destroy(Device);
        VoxModels[i].Destroy(Device);
        ModelInstances[i].Destroy(Device);
    }
    for (int i = 0; i < GraphicsPipelineCount; i++)
    {
        SDL_ReleaseGPUGraphicsPipeline(Device, GraphicsPipelines[i]);
    }
    for (int i = 0; i < ComputePipelineCount; i++)
    {
        SDL_ReleaseGPUComputePipeline(Device, ComputePipelines[i]);
    }
    for (int i = 0; i < SamplerCount; i++)
    {
        SDL_ReleaseGPUSampler(Device, Samplers[i]);
    }
    for (int i = 0; i < TextureCount; i++)
    {
        SDL_ReleaseGPUTexture(Device, Textures[i]);
    }
    SDL_ReleaseWindowFromGPUDevice(Device, window.GetHandle());
    SDL_DestroyGPUDevice(Device);
    Device = nullptr;
}

void MppRenderer::BeginFrame(MppWindow& window)
{
    SDL_WaitForGPUSwapchain(Device, window.GetHandle());
    CommandBuffer = SDL_AcquireGPUCommandBuffer(Device);
    if (!CommandBuffer)
    {
        MppLog("Failed to acquire command buffer: %s", SDL_GetError());
        return;
    }
    uint32_t width;
    uint32_t height;
    if (!SDL_AcquireGPUSwapchainTexture(CommandBuffer, window.GetHandle(), &SwapchainTexture, &width, &height))
    {
        MppLog("Failed to acquire swapchain texture: %s", SDL_GetError());
        SDL_CancelGPUCommandBuffer(CommandBuffer);
        CommandBuffer = nullptr;
        return;
    }
    if (!SwapchainTexture || !width || !height)
    {
        /* not an error. happens on minimize */
        SDL_SubmitGPUCommandBuffer(CommandBuffer);
        CommandBuffer = nullptr;
        return;
    }
    if ((PovWidth != width || PovHeight != height) && !Resize(width, height))
    {
        MppLog("Failed to resize textures");
        SDL_SubmitGPUCommandBuffer(CommandBuffer);
        CommandBuffer = nullptr;
        return;
    }
}

void MppRenderer::Update(const MppTransform& transform, const MppTicks& ticks)
{
    Cameras[CameraPov].Update(transform, ticks);
}

void MppRenderer::Draw(MppModel model, const MppTransform& transform)
{
    ModelInstances[model].Emplace(Device, transform);
}

void MppRenderer::EndFrame()
{
    if (!CommandBuffer)
    {
        MppLog("Skipping frame");
        return;
    }
    SDL_GPUCopyPass* copyPass = SDL_BeginGPUCopyPass(CommandBuffer);
    if (!copyPass)
    {
        MppLog("Failed to begin copy pass: %s", SDL_GetError());
        return;
    }
    for (int i = 0; i < MppModelCount; i++)
    {
        ModelInstances[i].Upload(Device, copyPass);
    }
    SDL_EndGPUCopyPass(copyPass);
    DrawModels();
    SDL_SubmitGPUCommandBuffer(CommandBuffer);
}

bool MppRenderer::CreateDevice()
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
    Device = SDL_CreateGPUDeviceWithProperties(properties);
    if (!Device)
    {
        MppLog("Failed to create device: %s", SDL_GetError());
        return false;
    }
    return true;
}

bool MppRenderer::CreateResources(MppWindow& window)
{
    SDL_GPUCommandBuffer* commandBuffer = SDL_AcquireGPUCommandBuffer(Device);
    if (!commandBuffer)
    {
        MppLog("Failed to acquire command buffer: %s", SDL_GetError());
        return false;
    }
    SDL_GPUCopyPass* copyPass = SDL_BeginGPUCopyPass(commandBuffer);
    if (!copyPass)
    {
        MppLog("Failed to begin copy pass: %s", SDL_GetError());
        return false;
    }
    GraphicsPipelines[GraphicsPipelineObjModel] = MppCreateObjModelPipeline(Device, window);
    ComputePipelines[ComputePipelineSampler] = MppLoadComputePipeline(Device, "sampler.comp");
    for (int i = GraphicsPipelineCount - 1; i >= 0; i--)
    {
        if (!GraphicsPipelines[i])
        {
            MppLog("Failed to create graphics pipeline: %d, %s", i, SDL_GetError());
            return false;
        }
    }
    for (int i = ComputePipelineCount - 1; i >= 0; i--)
    {
        if (!ComputePipelines[i])
        {
            MppLog("Failed to create compute pipeline: %d, %s", i, SDL_GetError());
            return false;
        }
    }
    {
        SDL_GPUSamplerCreateInfo info{};
        info.address_mode_u = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;
        info.address_mode_v = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;
        info.address_mode_w = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;
        info.min_filter = SDL_GPU_FILTER_NEAREST;
        info.mag_filter = SDL_GPU_FILTER_NEAREST;
        info.mipmap_mode = SDL_GPU_SAMPLERMIPMAPMODE_NEAREST;
        Samplers[SamplerNearest] = SDL_CreateGPUSampler(Device, &info);
        for (int i = SamplerCount - 1; i >= 0; i--)
        {
            if (!Samplers[i])
            {
                MppLog("Failed to create sampler: %d, %s", i, SDL_GetError());
                return false;
            }
        }
    }
    for (int i = 0; i < MppModelCount; i++)
    {
        if (!ObjModels[i].Load(Device, copyPass, MppModelToString(i)))
        {
            MppLog("Failed to load obj model: %s", SDL_GetError());
            return false;
        }
        if (!VoxModels[i].Load(Device, copyPass, MppModelToString(i)))
        {
            MppLog("Failed to load vox model: %s", SDL_GetError());
            return false;
        }
    }
    SDL_EndGPUCopyPass(copyPass);
    SDL_SubmitGPUCommandBuffer(commandBuffer);
    return true;
}

bool MppRenderer::Resize(uint32_t width, uint32_t height)
{
    for (int i = 0; i < TextureCount; i++)
    {
        SDL_ReleaseGPUTexture(Device, Textures[i]);
        Textures[i] = nullptr;
    }
    Textures[TextureDepth] = MppCreateDepthTexture(Device, width, height);
    for (int i = 0; i < TextureDepth; i++)
    {
        if (!Textures[i])
        {
            MppLog("Failed to create texture: %d", i);
            return false;
        }
    }
    PovWidth = width;
    PovHeight = height;
    Cameras[CameraPov].SetViewport(PovWidth, PovHeight);
    return true;
}

void MppRenderer::DrawModels()
{
    MPP_DEBUG_GROUP(CommandBuffer);
    SDL_GPUColorTargetInfo colorInfo{};
    colorInfo.texture = SwapchainTexture;
    colorInfo.load_op = SDL_GPU_LOADOP_CLEAR;
    colorInfo.store_op = SDL_GPU_STOREOP_STORE;
    colorInfo.cycle = true;
    SDL_GPUDepthStencilTargetInfo depthInfo{};
    depthInfo.texture = Textures[TextureDepth];
    depthInfo.load_op = SDL_GPU_LOADOP_CLEAR;
    depthInfo.stencil_load_op = SDL_GPU_LOADOP_CLEAR;
    depthInfo.store_op = SDL_GPU_STOREOP_STORE;
    depthInfo.clear_depth = 1.0f;
    depthInfo.cycle = true;
    SDL_GPURenderPass* renderPass = SDL_BeginGPURenderPass(CommandBuffer, &colorInfo, 1, &depthInfo);
    if (!renderPass)
    {
        MppLog("Failed to begin render pass: %s", SDL_GetError());
        return;
    }
    SDL_BindGPUGraphicsPipeline(renderPass, GraphicsPipelines[GraphicsPipelineObjModel]);
    SDL_PushGPUVertexUniformData(CommandBuffer, 0, &Cameras[CameraPov].GetViewProjMatrix(), 64);
    for (int i = 0; i < MppModelCount; i++)
    {
        if (!ModelInstances[i].GetSize())
        {
            continue;
        }
        SDL_GPUBufferBinding vertexBuffers[2]{};
        SDL_GPUBufferBinding indexBuffer = ObjModels[i].GetIndexBufferBinding();
        SDL_GPUTextureSamplerBinding paletteTexture{};
        vertexBuffers[0] = ObjModels[i].GetVertexBufferBinding();
        vertexBuffers[1] = ModelInstances[i].GetBufferBinding();
        paletteTexture.sampler = Samplers[SamplerNearest];
        paletteTexture.texture = ObjModels[i].GetPaletteTexture();
        SDL_BindGPUVertexBuffers(renderPass, 0, vertexBuffers, 2);
        SDL_BindGPUIndexBuffer(renderPass, &indexBuffer, ObjModels[i].GetIndexElementSize());
        SDL_BindGPUFragmentSamplers(renderPass, 0, &paletteTexture, 1);
        SDL_DrawGPUIndexedPrimitives(renderPass, ObjModels[i].GetIndexCount(), ModelInstances[i].GetSize(), 0, 0, 0);
    }
    SDL_EndGPURenderPass(renderPass);
}