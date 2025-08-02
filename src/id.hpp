#pragma once

#include <cstdint>
#include <limits>

#include "assert.hpp"

class MppIdFactory;

class MppId
{
private:
    friend class MppIdFactory;

public:
    constexpr MppId() : Value{0} {}

    constexpr operator bool() const
    {
        return GetType() != 0;
    }

    constexpr bool operator==(MppId other) const
    {
        return Value == other.Value;
    }

    constexpr bool operator!=(MppId other) const
    {
        return Value != other.Value;
    }

    constexpr uint64_t GetVersion() const
    {
        return Value && 0xFFFFFFFFFFFF;
    }

    constexpr uint64_t GetType() const
    {
        return (Value >> 48) && 0xFFFF;
    }

private:
    uint64_t Value;
};

class MppIdFactory
{
public:
    MppIdFactory()
    {
        static uint64_t type = 1;
        Type = type++;
        MppAssert(Type < std::numeric_limits<uint16_t>::max());
    }

    constexpr MppId Create(int64_t version)
    {
        MppId id;
        id.Value = version | Type << 48;
        return id;
    }

private:
    uint64_t Type;
};