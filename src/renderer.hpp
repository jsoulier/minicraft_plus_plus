#pragma once

#include <SDL3/SDL.h>

#include <cstdint>

#include "camera.hpp"
#include "model.hpp"
#include "obj_model.hpp"
#include "upload_buffer.hpp"
#include "vox_model.hpp"

class MppTicks;
class MppWindow;
struct MppTransform;

class MppRenderer
{
public:
    bool Init(MppWindow& window);
    void Quit(MppWindow& window);
    void BeginFrame(MppWindow& window);
    void Update(const MppTransform& transform, const MppTicks& ticks);
    void Draw(MppModel model, const MppTransform& transform);
    void EndFrame();

private:
    bool CreateDevice();
    bool CreateResources(MppWindow& window);
    bool Resize(uint32_t width, uint32_t height);
    void DrawModels();

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
        ComputePipelineSampler,
        ComputePipelineCount,
    };

    enum Camera
    {
        CameraPov,
        CameraCount,
    };

    SDL_GPUDevice* Device;
    SDL_GPUCommandBuffer* CommandBuffer;
    SDL_GPUTexture* SwapchainTexture;
    SDL_GPUTexture* Textures[TextureCount];
    SDL_GPUSampler* Samplers[SamplerCount];
    SDL_GPUGraphicsPipeline*  GraphicsPipelines[GraphicsPipelineCount];
    SDL_GPUComputePipeline* ComputePipelines[ComputePipelineCount];
    MppObjModel ObjModels[MppModelCount];
    MppVoxModel VoxModels[MppModelCount];
    MppVertexUploadBuffer<MppTransform> ModelInstances[MppModelCount];
    MppCamera Cameras[CameraCount];
    uint32_t PovWidth;
    uint32_t PovHeight;
};