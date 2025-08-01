#pragma once

#include <SDL3/SDL.h>

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <utility>

struct MppUploadBufferTraits
{
    static constexpr uint32_t kStartingCapacity = 10;
    static constexpr uint32_t kGrowthRate = 2;
};

template<typename T, SDL_GPUBufferUsageFlags U, typename Traits = MppUploadBufferTraits>
class MppUploadBuffer;
template<typename T, typename Traits = MppUploadBufferTraits>
using MppVertexUploadBuffer = MppUploadBuffer<T, SDL_GPU_BUFFERUSAGE_VERTEX, Traits>;
template<typename T, typename Traits = MppUploadBufferTraits>
using MppIndexUploadBuffer = MppUploadBuffer<T, SDL_GPU_BUFFERUSAGE_INDEX, Traits>;
template<typename T, typename Traits = MppUploadBufferTraits>
using MppComputeUploadBuffer = MppUploadBuffer<T, SDL_GPU_BUFFERUSAGE_COMPUTE_STORAGE_READ, Traits>;

template<typename T, SDL_GPUBufferUsageFlags U, typename Traits>
class MppUploadBuffer
{
public:
    MppUploadBuffer()
        : Buffer{nullptr}
        , TransferBuffer{nullptr}
        , BufferSize{0}
        , TransferBufferSize{0}
        , BufferCapacity{0}
        , TransferBufferCapacity{0}
        , Data{nullptr} {}

    void Destroy(SDL_GPUDevice* device)
    {
        SDL_ReleaseGPUBuffer(device, Buffer);
        Buffer = nullptr;
        BufferSize = 0;
        BufferCapacity = 0;
        DestroyTransferBuffer(device);
    }

    void DestroyTransferBuffer(SDL_GPUDevice* device)
    {
        SDL_ReleaseGPUTransferBuffer(device, TransferBuffer);
        TransferBuffer = nullptr;
        TransferBufferSize = 0;
        TransferBufferCapacity = 0;
        Data = nullptr;
    }

    template<typename... Args>
    void Emplace(SDL_GPUDevice* device, Args&&... args)
    {
        if (!Data && TransferBuffer)
        {
            BufferSize = 0;
            SDL_assert(!TransferBufferSize);
            Data = static_cast<T*>(SDL_MapGPUTransferBuffer(device, TransferBuffer, true));
            if (!Data)
            {
                SDL_Log("Failed to map transfer buffer: %s", SDL_GetError());
                return;
            }
        }
        SDL_assert(TransferBufferSize <= TransferBufferCapacity);
        if (TransferBufferSize == TransferBufferCapacity)
        {
            uint32_t capacity = std::max(Traits::kStartingCapacity, TransferBufferSize * Traits::kGrowthRate);
            SDL_GPUTransferBufferCreateInfo info{};
            info.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
            info.size = capacity * sizeof(T);
            SDL_GPUTransferBuffer* transferBuffer = SDL_CreateGPUTransferBuffer(device, &info);
            if (!transferBuffer)
            {
                SDL_Log("Failed to create transfer buffer: %s", SDL_GetError());
                return;
            }
            T* data = static_cast<T*>(SDL_MapGPUTransferBuffer(device, transferBuffer, false));
            if (!data)
            {
                SDL_Log("Failed to map transfer buffer: %s", SDL_GetError());
                SDL_ReleaseGPUTransferBuffer(device, transferBuffer);
                return;
            }
            if (Data)
            {
                std::copy(Data, Data + TransferBufferSize, data);
                SDL_UnmapGPUTransferBuffer(device, TransferBuffer);
            }
            SDL_ReleaseGPUTransferBuffer(device, TransferBuffer);
            TransferBufferCapacity = capacity;
            TransferBuffer = transferBuffer;
            Data = data;
        }
        SDL_assert(Data);
        Data[TransferBufferSize++] = T{std::forward<Args>(args)...};
    }

    void Upload(SDL_GPUDevice* device, SDL_GPUCopyPass* copyPass)
    {
        if (Data)
        {
            SDL_UnmapGPUTransferBuffer(device, TransferBuffer);
            Data = nullptr;
        }
        uint32_t size = TransferBufferSize;
        TransferBufferSize = 0;
        if (!size)
        {
            BufferSize = 0;
            return;
        }
        if (TransferBufferCapacity > BufferCapacity)
        {
            SDL_ReleaseGPUBuffer(device, Buffer);
            Buffer = nullptr;
            BufferCapacity = 0;
            SDL_GPUBufferCreateInfo info{};
            info.usage = U;
            info.size = TransferBufferCapacity * sizeof(T);
            Buffer = SDL_CreateGPUBuffer(device, &info);
            if (!Buffer)
            {
                SDL_Log("Failed to create buffer: %s", SDL_GetError());
                return;
            }
            BufferCapacity = TransferBufferCapacity;
        }
        SDL_GPUTransferBufferLocation location{};
        SDL_GPUBufferRegion region{};
        location.transfer_buffer = TransferBuffer;
        region.buffer = Buffer;
        region.size = size * sizeof(T);
        SDL_UploadToGPUBuffer(copyPass, &location, &region, true);
        BufferSize = size;
    }

    void Upload(SDL_GPUDevice* device, SDL_GPUCommandBuffer* commandBuffer)
    {
        SDL_GPUCopyPass* copyPass = SDL_BeginGPUCopyPass(commandBuffer);
        if (!copyPass)
        {
            SDL_Log("Failed to begin copy pass: %s", SDL_GetError());
            return;
        }
        Upload(device, copyPass);
        SDL_EndGPUCopyPass(copyPass);
    }

    void Upload(SDL_GPUDevice* device)
    {
        SDL_GPUCommandBuffer* commandBuffer = SDL_AcquireGPUCommandBuffer(device);
        if (!commandBuffer)
        {
            SDL_Log("Failed to acquire command buffer: %s", SDL_GetError());
            return;
        }
        Upload(device, commandBuffer);
        SDL_SubmitGPUCommandBuffer(commandBuffer);
    }

    SDL_GPUBuffer* GetBuffer() const
    {
        return Buffer;
    }

    uint32_t GetBufferSize() const
    {
        return BufferSize;
    }

    uint32_t GetTransferBufferSize() const
    {
        return TransferBufferSize;
    }

private:
    SDL_GPUBuffer* Buffer;
    SDL_GPUTransferBuffer* TransferBuffer;
    uint32_t BufferSize;
    uint32_t TransferBufferSize;
    uint32_t BufferCapacity;
    uint32_t TransferBufferCapacity;
    T* Data;
};