#include <SDL3/SDL.h>
#include <stb_image.h>

#include <cassert>
#include <cstring>
#include <string_view>

#include "texture.hpp"

SDL_GPUTextureFormat MppGetDepthTextureFormat(SDL_GPUDevice* device)
{
    static constexpr SDL_GPUTextureType Type = SDL_GPU_TEXTURETYPE_2D;
    static constexpr SDL_GPUTextureUsageFlags Usage = SDL_GPU_TEXTUREUSAGE_DEPTH_STENCIL_TARGET;
    if (SDL_GPUTextureSupportsFormat(device, SDL_GPU_TEXTUREFORMAT_D24_UNORM, Type, Usage))
    {
        return SDL_GPU_TEXTUREFORMAT_D24_UNORM;
    }
    else
    {
        return SDL_GPU_TEXTUREFORMAT_D32_FLOAT;
    }
}

SDL_GPUTexture* MppLoadTexture(SDL_GPUDevice* device, SDL_GPUCopyPass* copyPass, const std::string_view& path)
{
    int width;
    int height;
    int channels;
    void* srcData = stbi_load(path.data(), &width, &height, &channels, 4);
    if (!srcData)
    {
        SDL_Log("Failed to load image: %s, %s", path.data(), stbi_failure_reason());
        return nullptr;
    }
    SDL_GPUTexture* texture;
    SDL_GPUTransferBuffer* transferBuffer;
    {
        SDL_GPUTextureCreateInfo info{};
        info.usage = SDL_GPU_TEXTUREUSAGE_SAMPLER;
        info.type = SDL_GPU_TEXTURETYPE_2D;
        info.format = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM;
        info.width = width;
        info.height = height;
        info.layer_count_or_depth = 1;
        info.num_levels = 1;
        texture = SDL_CreateGPUTexture(device, &info);
        if (!texture)
        {
            SDL_Log("Failed to create texture: %s, %s", path.data(), SDL_GetError());
            stbi_image_free(srcData);
            return nullptr;
        }
    }
    {
        SDL_GPUTransferBufferCreateInfo info{};
        info.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
        info.size = width * height * 4;
        transferBuffer = SDL_CreateGPUTransferBuffer(device, &info);
        if (!transferBuffer)
        {
            SDL_Log("Failed to create transfer buffer: %s, %s", path.data(), SDL_GetError());
            stbi_image_free(srcData);
            SDL_ReleaseGPUTexture(device, texture);
            return nullptr;
        }
    }
    void* dstData = SDL_MapGPUTransferBuffer(device, transferBuffer, false);
    if (!dstData)
    {
        SDL_Log("Failed to map transfer buffer: %s, %s", path.data(), SDL_GetError());
        stbi_image_free(srcData);
        SDL_ReleaseGPUTexture(device, texture);
        return nullptr;
    }
    std::memcpy(dstData, srcData, width * height * 4);
    stbi_image_free(srcData);
    SDL_UnmapGPUTransferBuffer(device, transferBuffer);
    SDL_GPUTextureTransferInfo info{};
    SDL_GPUTextureRegion region{};
    info.transfer_buffer = transferBuffer;
    region.texture = texture;
    region.w = width;
    region.h = height;
    region.d = 1;
    SDL_UploadToGPUTexture(copyPass, &info, &region, true);
    SDL_ReleaseGPUTransferBuffer(device, transferBuffer);
    return texture;
}

SDL_GPUTexture* MppCreateColorTexture(SDL_GPUDevice* device, SDL_Window* window, int width, int height, SDL_GPUTextureUsageFlags usage)
{
    SDL_GPUTextureCreateInfo info{};
    info.type = SDL_GPU_TEXTURETYPE_2D;
    info.usage = usage | SDL_GPU_TEXTUREUSAGE_COLOR_TARGET;
    info.format = SDL_GetGPUSwapchainTextureFormat(device, window);
    info.width = width;
    info.height = height;
    info.layer_count_or_depth = 1;
    info.num_levels = 1;
#if SDL_PLATFORM_WIN32
    info.props = SDL_CreateProperties();
    if (!info.props)
    {
        SDL_Log("Failed to create properties: %s", SDL_GetError());
        return nullptr;
    }
    SDL_SetFloatProperty(info.props, SDL_PROP_GPU_TEXTURE_CREATE_D3D12_CLEAR_R_FLOAT, 0.0f);
    SDL_SetFloatProperty(info.props, SDL_PROP_GPU_TEXTURE_CREATE_D3D12_CLEAR_G_FLOAT, 0.0f);
    SDL_SetFloatProperty(info.props, SDL_PROP_GPU_TEXTURE_CREATE_D3D12_CLEAR_B_FLOAT, 0.0f);
    SDL_SetFloatProperty(info.props, SDL_PROP_GPU_TEXTURE_CREATE_D3D12_CLEAR_A_FLOAT, 0.0f);
#endif
    SDL_GPUTexture* texture = SDL_CreateGPUTexture(device, &info);
#if SDL_PLATFORM_WIN32
    SDL_DestroyProperties(info.props);
#endif
    if (!texture)
    {
        SDL_Log("Failed to create texture: %s", SDL_GetError());
        return nullptr;
    }
    return texture;
}

SDL_GPUTexture* MppCreateDepthTexture(SDL_GPUDevice* device, int width, int height, SDL_GPUTextureUsageFlags usage)
{
    SDL_GPUTextureCreateInfo info{};
    info.type = SDL_GPU_TEXTURETYPE_2D;
    info.usage = usage | SDL_GPU_TEXTUREUSAGE_DEPTH_STENCIL_TARGET;
    info.format = MppGetDepthTextureFormat(device);
    info.width = width;
    info.height = height;
    info.layer_count_or_depth = 1;
    info.num_levels = 1;
#if SDL_PLATFORM_WIN32
    info.props = SDL_CreateProperties();
    if (!info.props)
    {
        SDL_Log("Failed to create properties: %s", SDL_GetError());
        return nullptr;
    }
    SDL_SetFloatProperty(info.props, SDL_PROP_GPU_TEXTURE_CREATE_D3D12_CLEAR_DEPTH_FLOAT, 1.0f);
    SDL_SetFloatProperty(info.props, SDL_PROP_GPU_TEXTURE_CREATE_D3D12_CLEAR_STENCIL_NUMBER, 0.0f);
#endif
    SDL_GPUTexture* texture = SDL_CreateGPUTexture(device, &info);
#if SDL_PLATFORM_WIN32
    SDL_DestroyProperties(info.props);
#endif
    if (!texture)
    {
        SDL_Log("Failed to create texture: %s", SDL_GetError());
        return nullptr;
    }
    return texture;
}