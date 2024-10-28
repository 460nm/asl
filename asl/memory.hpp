#pragma once

#include "asl/integers.hpp"

constexpr void* operator new(size_t, void* ptr)
{
    return ptr;
}
