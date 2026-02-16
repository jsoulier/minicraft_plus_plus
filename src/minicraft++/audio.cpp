#include <SDL3/SDL.h>

#include <cstdint>
#include <filesystem>
#include <queue>
#include <span>
#include <string>
#include <unordered_map>
#include <vector>

#include <minicraft++/audio.hpp>
#include <minicraft++/log.hpp>
#include <minicraft++/random.hpp>

static const std::filesystem::path kBasePath = SDL_GetBasePath();

static SDL_AudioStream* stream;
static SDL_AudioSpec spec;
static std::unordered_map<std::string, int> indices;
static std::vector<std::span<uint8_t>> streams;
static std::queue<int> queue;

MppAudioHandle::MppAudioHandle(const std::vector<std::string>& names)
{
    for (const std::string& name : names)
    {
        auto it = indices.find(name);
        if (it != indices.end())
        {
            Indices.push_back(it->second);
            continue;
        }
        std::filesystem::path path = kBasePath / name;
        path += ".wav";
        uint8_t* data = nullptr;
        uint32_t size = 0;
        if (!SDL_LoadWAV(path.string().data(), &spec, &data, &size))
        {
            MppLog("Failed to load WAV: %s, %s", path.string().data(), SDL_GetError());
            continue;
        }
        int index = streams.size();
        streams.emplace_back(data, size);
        indices[name] = index;
        Indices.push_back(index);
    }
}

void MppAudioHandle::Play() const
{
    if (!Indices.empty())
    {
        queue.push(Indices[MppRandomGetInt(0, Indices.size() - 1)]);
    }
}

bool MppAudioInit()
{
    // TODO: handle SDL_EVENT_AUDIO_DEVICE
    stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, nullptr, nullptr);
    if (!stream)
    {
        MppLog("Failed to open audio stream: %s", SDL_GetError());
        return false;
    }
    if (!SDL_ResumeAudioStreamDevice(stream))
    {
        MppLog("Failed to resume audio stream: %s", SDL_GetError());
        return false;
    }
    return true;
}

void MppAudioQuit()
{
    for (std::span<uint8_t>& span : streams)
    {
        SDL_free(span.data());
    }
    indices.clear();
    streams.clear();
    queue = {};
    SDL_DestroyAudioStream(stream);
    stream = nullptr;
}

void MppAudioUpdate(uint64_t ticks)
{
    if (queue.empty())
    {
        return;
    }
    const std::span<uint8_t>& span = streams[queue.front()];
    if (SDL_GetAudioStreamQueued(stream) > span.size())
    {
        return;
    }
    SDL_PutAudioStreamDataNoCopy(stream, span.data(), span.size(), nullptr, nullptr);
    queue.pop();
}
