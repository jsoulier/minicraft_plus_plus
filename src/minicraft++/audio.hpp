#pragma once

#include <cstdint>
#include <string>
#include <vector>

class MppAudioHandle
{
public:
    MppAudioHandle(const std::vector<std::string>& names);
    void Play() const;

private:
    std::vector<int> Indices;
};

bool MppAudioInit();
void MppAudioQuit();
void MppAudioUpdate(uint64_t ticks);
