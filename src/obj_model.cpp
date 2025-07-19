#include <SDL3/SDL.h>
#include <tiny_obj_loader.h>

#include <cassert>
#include <cmath>
#include <cstdint>
#include <format>
#include <limits>
#include <string>
#include <string_view>
#include <unordered_map>

#include "obj_model.hpp"
#include "texture.hpp"

/* LSB to MSB
 * 00-05: x magnitude (6 bits)
 * 06-06: x direction (1 bits)
 * 07-12: y magnitude (6 bits)
 * 13-13: y direction (1 bits)
 * 14-19: z magnitude (6 bits)
 * 20-20: z direction (1 bits)
 * 21-23: normal (3 bits)
 * 24-31: x texcoord (8 bits)
 */
using Vertex = uint32_t;

static Vertex CreateVertex(const tinyobj::attrib_t& attrib, const tinyobj::index_t& index)
{
    static constexpr int PositionScale = 10;
    static constexpr int TexcoordScale = 256;
    int positionX = attrib.vertices[index.vertex_index * 3 + 0] * PositionScale;
    int positionY = attrib.vertices[index.vertex_index * 3 + 1] * PositionScale;
    int positionZ = attrib.vertices[index.vertex_index * 3 + 2] * PositionScale;
    int normalX = attrib.normals[index.normal_index * 3 + 0];
    int normalY = attrib.normals[index.normal_index * 3 + 1];
    int normalZ = attrib.normals[index.normal_index * 3 + 2];
    int texcoordX = attrib.texcoords[index.texcoord_index * 2 + 0] * TexcoordScale;
    uint32_t magnitudeX = std::abs(positionX);
    uint32_t directionX = positionX < 0 ? 1 : 0;
    uint32_t magnitudeY = std::abs(positionY);
    uint32_t directionY = positionY < 0 ? 1 : 0;
    uint32_t magnitudeZ = std::abs(positionZ);
    uint32_t directionZ = positionZ < 0 ? 1 : 0;
    uint32_t normal;
    if (normalX < 0)
    {
        normal = 0;
    }
    else if (normalX > 0)
    {
        normal = 1;
    }
    else if (normalY < 0)
    {
        normal = 2;
    }
    else if (normalY > 0)
    {
        normal = 3;
    }
    else if (normalZ < 0)
    {
        normal = 4;
    }
    else if (normalZ > 0)
    {
        normal = 5;
    }
    else
    {
        assert(false);
    }
    assert(magnitudeX < 64);
    assert(magnitudeY < 64);
    assert(magnitudeZ < 64);
    assert(texcoordX < 256);
    Vertex voxel{};
    voxel |= (magnitudeX & 0x3F) << 0;
    voxel |= (directionX & 0x01) << 6;
    voxel |= (magnitudeY & 0x3F) << 7;
    voxel |= (directionY & 0x01) << 13;
    voxel |= (magnitudeZ & 0x3F) << 14;
    voxel |= (directionZ & 0x01) << 20;
    voxel |= (normal & 0x07) << 21;
    voxel |= (texcoordX & 0xFF) << 24;
    return voxel;
}

bool MppObjModel::Load(SDL_GPUDevice* device, SDL_GPUCopyPass* copyPass, const std::string_view& name)
{
    std::string objPath = std::format("{}.obj", name);
    std::string pngPath = std::format("{}.png", name);
    tinyobj::ObjReader reader;
    if (!reader.ParseFromFile(objPath))
    {
        SDL_Log("Failed to parse obj: %s", name.data());
        return false;
    }
    const tinyobj::attrib_t& attrib = reader.GetAttrib();
    const tinyobj::shape_t& shape = reader.GetShapes()[0];
    uint32_t maxIndexCount = shape.mesh.num_face_vertices.size() * 3;
    assert(maxIndexCount <= std::numeric_limits<uint16_t>::max());
    SDL_GPUTransferBuffer* vertexTransferBuffer;
    SDL_GPUTransferBuffer* indexTransferBuffer;
    {
        SDL_GPUTransferBufferCreateInfo info{};
        info.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
        info.size = maxIndexCount * sizeof(Vertex);
        vertexTransferBuffer = SDL_CreateGPUTransferBuffer(device, &info);
        info.size = maxIndexCount * sizeof(uint16_t);
        indexTransferBuffer = SDL_CreateGPUTransferBuffer(device, &info);
        if (!vertexTransferBuffer || !indexTransferBuffer)
        {
            SDL_Log("Failed to create transfer buffer(s): %s, %s", name.data(), SDL_GetError());
            return false;
        }
    }
    Vertex* vertexData = static_cast<Vertex*>(SDL_MapGPUTransferBuffer(device, vertexTransferBuffer, false));
    uint16_t* indexData = static_cast<uint16_t*>(SDL_MapGPUTransferBuffer(device, indexTransferBuffer, false));
    if (!vertexData || !indexData)
    {
        SDL_Log("Failed to map transfer buffer(s): %s, %s", name.data(), SDL_GetError());
        return false;
    }
    uint32_t vertexCount = 0;
    indexCount = 0;
    std::unordered_map<Vertex, uint16_t> vertexToIndex;
    for (uint16_t i = 0; i < maxIndexCount; i++)
    {
        tinyobj::index_t index = shape.mesh.indices[i];
        Vertex vertex = CreateVertex(attrib, index);
        auto [it, inserted] = vertexToIndex.try_emplace(vertex, indexCount);
        if (inserted)
        {
            vertexData[vertexCount] = vertex;
            indexData[indexCount++] = vertexCount++;
        }
        else
        {
            indexData[indexCount++] = it->second;
        }
    }
    SDL_UnmapGPUTransferBuffer(device, vertexTransferBuffer);
    SDL_UnmapGPUTransferBuffer(device, indexTransferBuffer);
    {
        SDL_GPUBufferCreateInfo info{};
        info.usage = SDL_GPU_BUFFERUSAGE_VERTEX;
        info.size = vertexCount * sizeof(Vertex);
        vertexBuffer = SDL_CreateGPUBuffer(device, &info);
        info.usage = SDL_GPU_BUFFERUSAGE_INDEX;
        info.size = indexCount * sizeof(uint16_t);
        indexBuffer = SDL_CreateGPUBuffer(device, &info);
        if (!vertexBuffer || !indexBuffer)
        {
            SDL_Log("Failed to create buffer(s): %s, %s", name.data(), SDL_GetError());
            return false;
        }
    }
    {
        SDL_GPUTransferBufferLocation location{};
        SDL_GPUBufferRegion region{};
        location.transfer_buffer = vertexTransferBuffer;
        region.buffer = vertexBuffer;
        region.size = vertexCount * sizeof(Vertex);
        SDL_UploadToGPUBuffer(copyPass, &location, &region, false);
        location.transfer_buffer = indexTransferBuffer;
        region.buffer = indexBuffer;
        region.size = indexCount * sizeof(uint16_t);
        SDL_UploadToGPUBuffer(copyPass, &location, &region, false);
    }
    SDL_ReleaseGPUTransferBuffer(device, vertexTransferBuffer);
    SDL_ReleaseGPUTransferBuffer(device, indexTransferBuffer);
    paletteTexture = MppLoadTexture(device, copyPass, pngPath);
    if (!paletteTexture)
    {
        SDL_Log("Failed to load texture: %s", name.data());
        return false;
    }
    return true;
}

void MppObjModel::Destroy(SDL_GPUDevice* device)
{
    SDL_ReleaseGPUBuffer(device, vertexBuffer);
    SDL_ReleaseGPUBuffer(device, indexBuffer);
    SDL_ReleaseGPUTexture(device, paletteTexture);
    vertexBuffer = nullptr;
    indexBuffer = nullptr;
    paletteTexture = nullptr;
}

SDL_GPUBufferBinding MppObjModel::GetVertexBufferBinding() const
{
    return SDL_GPUBufferBinding{vertexBuffer};
}

SDL_GPUBufferBinding MppObjModel::GetIndexBufferBinding() const
{
    return SDL_GPUBufferBinding{indexBuffer};
}

SDL_GPUTexture* MppObjModel::GetPaletteTexture() const
{
    return paletteTexture;
}

uint16_t MppObjModel::GetIndexCount() const
{
    return indexCount;
}

SDL_GPUIndexElementSize MppObjModel::GetIndexElementSize()
{
    return SDL_GPU_INDEXELEMENTSIZE_16BIT;
}