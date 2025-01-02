#pragma once

#include "asl/integers.hpp"
#include "asl/meta.hpp"
#include "asl/layout.hpp"
#include "asl/utility.hpp"

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

constexpr void memcpy(void* dst, const void* src, isize_t size)
{
    __builtin_memcpy(dst, src, static_cast<size_t>(size));
}

constexpr isize_t strlen(const char* s)
{
    return static_cast<isize_t>(__builtin_strlen(s));
}

template<typename T, typename... Args>
constexpr T* construct_at(void* ptr, Args&&... args)
    requires constructible_from<T, Args&&...>
{
    return new (ptr) T{ ASL_FWD(args)... };
}

template<typename T>
constexpr void destroy(T* data)
{
    if constexpr (!trivially_destructible<T>)
    {
        data->~T();
    }
}

template<typename T>
constexpr void destroy_n(T* data, isize_t n)
{
    if constexpr (!trivially_destructible<T>)
    {
        for (isize_t i = 0; i < n; ++i)
        {
            destroy(data + i);
        }
    }
}

template<move_constructible T>
constexpr void relocate_uninit_n(T* to, T* from, isize_t n)
{
    if constexpr (trivially_move_constructible<T>)
    {
        static_assert(trivially_destructible<T>);
        memcpy(to, from, size_of<T> * n);
    }
    else
    {
        for (isize_t i = 0; i < n; ++i)
        {
            // NOLINTNEXTLINE(*-pointer-arithmetic)
            construct_at<T>(to + i, ASL_MOVE(from[i]));
        }
        destroy_n(from, n);
    }
}

template<move_assignable T>
constexpr void relocate_assign_n(T* to, T* from, isize_t n)
{
    if constexpr (trivially_move_assignable<T>)
    {
        static_assert(trivially_destructible<T>);
        memcpy(to, from, size_of<T> * n);
    }
    else
    {
        for (isize_t i = 0; i < n; ++i)
        {
            // NOLINTNEXTLINE(*-pointer-arithmetic)
            to[i] = ASL_MOVE(from[i]);
        }
        destroy_n(from, n);
    }
}

} // namespace asl

