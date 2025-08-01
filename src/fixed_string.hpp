#pragma once

#include <array>
#include <cstddef>

template<std::size_t N>
using MppBasicFixedString = std::array<char, N>;
using MppFixedString = MppBasicFixedString<256>;

template<typename T>
struct MppFixedStringTraits;
template<std::size_t N>
struct MppFixedStringTraits<MppBasicFixedString<N>>
{
    static constexpr std::size_t kSize = N;
};

template<std::size_t N>
constexpr MppFixedString MppFixedStringToLower(const char (&string)[N])
{
    static_assert(N < MppFixedStringTraits<MppFixedString>::kSize);
    MppFixedString result{};
    for (std::size_t i = 0; i < N; i++)
    {
        char c = string[i];
        if (c >= 'A' && c <= 'Z')
        {
            result[i] = c + 32;
        }
        else
        {
            result[i] = c;
        }
    }
    return result;
}