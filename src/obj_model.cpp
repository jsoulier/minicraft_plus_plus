#include <SDL3/SDL.h>
#include <tiny_obj_loader.h>

#include <cmath>
#include <cstdint>
#include <format>
#include <limits>
#include <string>
#include <string_view>
#include <unordered_map>

#include "assert.hpp"
#include "log.hpp"
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
    static constexpr int kPositionScale = 10;
    static constexpr int kTexcoordScale = 255;
    int positionX = attrib.vertices[index.vertex_index * 3 + 0] * kPositionScale;
    int positionY = attrib.vertices[index.vertex_index * 3 + 1] * kPositionScale;
    int positionZ = attrib.vertices[index.vertex_index * 3 + 2] * kPositionScale;
    int normalX = attrib.normals[index.normal_index * 3 + 0];
    int normalY = attrib.normals[index.normal_index * 3 + 1];
    int normalZ = attrib.normals[index.normal_index * 3 + 2];
    uint32_t texcoordX = attrib.texcoords[index.texcoord_index * 2 + 0] * kTexcoordScale;
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
        MppAssert(false);
    }
    MppAssert(magnitudeX < 64);
    MppAssert(magnitudeY < 64);
    MppAssert(magnitudeZ < 64);
    MppAssert(texcoordX < 256);
    Vertex vertex{};
    vertex |= (magnitudeX & 0x3F) << 0;
    vertex |= (directionX & 0x01) << 6;
    vertex |= (magnitudeY & 0x3F) << 7;
    vertex |= (directionY & 0x01) << 13;
    vertex |= (magnitudeZ & 0x3F) << 14;
    vertex |= (directionZ & 0x01) << 20;
    vertex |= (normal & 0x07) << 21;
    vertex |= (texcoordX & 0xFF) << 24;
    return vertex;
}

bool MppObjModel::Load(SDL_GPUDevice* device, SDL_GPUCopyPass* copyPass, const std::string_view& name)
{
    std::string objPath = std::format("{}.obj", name);
    std::string pngPath = std::format("{}.png", name);
    tinyobj::ObjReader reader;
    if (!reader.ParseFromFile(objPath))
    {
        MppLog("Failed to parse obj: %s", name.data());
        return false;
    }
    const tinyobj::attrib_t& attrib = reader.GetAttrib();
    const tinyobj::shape_t& shape = reader.GetShapes()[0];
    uint32_t maxIndexCount = shape.mesh.num_face_vertices.size() * 3;
    MppAssert(maxIndexCount <= std::numeric_limits<uint16_t>::max());
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
            MppLog("Failed to create transfer buffer(s): %s, %s", name.data(), SDL_GetError());
            return false;
        }
    }
    Vertex* vertexData = static_cast<Vertex*>(SDL_MapGPUTransferBuffer(device, vertexTransferBuffer, false));
    uint16_t* indexData = static_cast<uint16_t*>(SDL_MapGPUTransferBuffer(device, indexTransferBuffer, false));
    if (!vertexData || !indexData)
    {
        MppLog("Failed to map transfer buffer(s): %s, %s", name.data(), SDL_GetError());
        return false;
    }
    uint32_t vertexCount = 0;
    IndexCount = 0;
    std::unordered_map<Vertex, uint16_t> vertexToIndex;
    for (uint16_t i = 0; i < maxIndexCount; i++)
    {
        tinyobj::index_t index = shape.mesh.indices[i];
        Vertex vertex = CreateVertex(attrib, index);
        auto [it, inserted] = vertexToIndex.try_emplace(vertex, vertexCount);
        if (inserted)
        {
            vertexData[vertexCount] = vertex;
            indexData[IndexCount++] = vertexCount++;
        }
        else
        {
            indexData[IndexCount++] = it->second;
        }
    }
    SDL_UnmapGPUTransferBuffer(device, vertexTransferBuffer);
    SDL_UnmapGPUTransferBuffer(device, indexTransferBuffer);
    {
        SDL_GPUBufferCreateInfo info{};
        info.usage = SDL_GPU_BUFFERUSAGE_VERTEX;
        info.size = vertexCount * sizeof(Vertex);
        VertexBuffer = SDL_CreateGPUBuffer(device, &info);
        info.usage = SDL_GPU_BUFFERUSAGE_INDEX;
        info.size = IndexCount * sizeof(uint16_t);
        IndexBuffer = SDL_CreateGPUBuffer(device, &info);
        if (!VertexBuffer || !IndexBuffer)
        {
            MppLog("Failed to create buffer(s): %s, %s", name.data(), SDL_GetError());
            return false;
        }
    }
    {
        SDL_GPUTransferBufferLocation location{};
        SDL_GPUBufferRegion region{};
        location.transfer_buffer = vertexTransferBuffer;
        region.buffer = VertexBuffer;
        region.size = vertexCount * sizeof(Vertex);
        SDL_UploadToGPUBuffer(copyPass, &location, &region, false);
        location.transfer_buffer = indexTransferBuffer;
        region.buffer = IndexBuffer;
        region.size = IndexCount * sizeof(uint16_t);
        SDL_UploadToGPUBuffer(copyPass, &location, &region, false);
    }
    SDL_ReleaseGPUTransferBuffer(device, vertexTransferBuffer);
    SDL_ReleaseGPUTransferBuffer(device, indexTransferBuffer);
    PaletteTexture = MppLoadTexture(device, copyPass, pngPath);
    if (!PaletteTexture)
    {
        MppLog("Failed to load texture: %s", name.data());
        return false;
    }
    return true;
}

void MppObjModel::Destroy(SDL_GPUDevice* device)
{
    SDL_ReleaseGPUBuffer(device, VertexBuffer);
    SDL_ReleaseGPUBuffer(device, IndexBuffer);
    SDL_ReleaseGPUTexture(device, PaletteTexture);
    VertexBuffer = nullptr;
    IndexBuffer = nullptr;
    PaletteTexture = nullptr;
}

SDL_GPUBufferBinding MppObjModel::GetVertexBufferBinding() const
{
    return SDL_GPUBufferBinding{VertexBuffer};
}

SDL_GPUBufferBinding MppObjModel::GetIndexBufferBinding() const
{
    return SDL_GPUBufferBinding{IndexBuffer};
}

SDL_GPUTexture* MppObjModel::GetPaletteTexture() const
{
    return PaletteTexture;
}

uint16_t MppObjModel::GetIndexCount() const
{
    return IndexCount;
}

SDL_GPUIndexElementSize MppObjModel::GetIndexElementSize()
{
    return SDL_GPU_INDEXELEMENTSIZE_16BIT;
}