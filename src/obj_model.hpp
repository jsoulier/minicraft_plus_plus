#pragma once

#include <SDL3/SDL.h>

#include <string_view>

class MppObjModel
{
public:
    bool Load(SDL_GPUDevice* device, SDL_GPUCopyPass* copyPass, const std::string_view& name);
    void Destroy(SDL_GPUDevice* device);
    SDL_GPUBufferBinding GetVertexBufferBinding() const;
    SDL_GPUBufferBinding GetIndexBufferBinding() const;
    SDL_GPUTexture* GetPaletteTexture() const;
    uint16_t GetIndexCount() const;
    static SDL_GPUIndexElementSize GetIndexElementSize();

private:
    SDL_GPUBuffer* vertexBuffer;
    SDL_GPUBuffer* indexBuffer;
    SDL_GPUTexture* paletteTexture;
    uint16_t indexCount;
};