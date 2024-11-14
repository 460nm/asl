#pragma once

#include "asl/integers.hpp"

constexpr void* operator new(size_t, void* ptr)
{
    return ptr;
}

namespace asl
{

constexpr isize_t memcmp(const void* a, const void* b, isize_t size)
{
    return __builtin_memcmp(a, b, static_cast<size_t>(size));
}

constexpr isize_t strlen(const char* s)
{
    return static_cast<isize_t>(__builtin_strlen(s));
}

} // namespace asl

