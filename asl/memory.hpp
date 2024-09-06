#pragma once

#include "asl/integers.hpp"

constexpr void* operator new(uint64_t, void* ptr)
{
    return ptr;
}
