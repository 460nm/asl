// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#include "asl/base/support.hpp"
#include "asl/base/meta.hpp"

namespace asl
{
[[nodiscard]]
constexpr isize_t memcmp(const void* a, const void* b, isize_t size)
{
    return __builtin_memcmp(a, b, static_cast<size_t>(size));
}

constexpr void memcpy(void* dst, const void* src, isize_t size)
{
    __builtin_memcpy(dst, src, static_cast<size_t>(size));
}

inline void memzero(void* dst, isize_t size)
{
    __builtin_memset(dst, 0, static_cast<size_t>(size));
}

constexpr isize_t strlen(const char* s)
{
    return static_cast<isize_t>(__builtin_strlen(s));
}

template<typename T, typename... Args>
constexpr T* construct_at(void* ptr, Args&&... args)
    requires constructible_from<T, Args&&...>
{
    return new (ptr) T{ std::forward<Args>(args)... }; // NOLINT(*-owning-memory)
}

template<typename T>
constexpr void destroy_at(T* data)
{
    if constexpr (!is_trivially_destructible<T>)
    {
        data->~T();
    }
}

template<typename T>
constexpr void destroy_n(T* data, isize_t n)
{
    if constexpr (!is_trivially_destructible<T>)
    {
        for (isize_t i = 0; i < n; ++i)
        {
            destroy_at(data + i);
        }
    }
}

template<is_copy_constructible T>
constexpr void copy_uninit_n(T* to, const T* from, isize_t n)
{
    if constexpr (is_trivially_copy_constructible<T>)
    {
        memcpy(to, from, static_cast<isize_t>(sizeof(T)) * n);
    }
    else
    {
        for (isize_t i = 0; i < n; ++i)
        {
            // NOLINTNEXTLINE(*-pointer-arithmetic)
            construct_at<T>(to + i, from[i]);
        }
    }
}

template<is_copy_assignable T>
constexpr void copy_assign_n(T* to, const T* from, isize_t n)
{
    if constexpr (is_trivially_copy_assignable<T>)
    {
        memcpy(to, from, static_cast<isize_t>(sizeof(T)) * n);
    }
    else
    {
        for (isize_t i = 0; i < n; ++i)
        {
            // NOLINTNEXTLINE(*-pointer-arithmetic)
            to[i] = from[i];
        }
    }
}

template<is_move_constructible T>
constexpr void move_uninit_n(T* to, T* from, isize_t n)
{
    if constexpr (is_trivially_move_constructible<T>)
    {
        static_assert(is_trivially_destructible<T>);
        memcpy(to, from, static_cast<isize_t>(sizeof(T)) * n);
    }
    else
    {
        for (isize_t i = 0; i < n; ++i)
        {
            // NOLINTNEXTLINE(*-pointer-arithmetic)
            construct_at<T>(to + i, std::move(from[i]));
        }
    }
}

template<is_move_assignable T>
constexpr void move_assign_n(T* to, T* from, isize_t n)
{
    if constexpr (is_trivially_move_assignable<T>)
    {
        static_assert(is_trivially_destructible<T>);
        memcpy(to, from, static_cast<isize_t>(sizeof(T)) * n);
    }
    else
    {
        for (isize_t i = 0; i < n; ++i)
        {
            // NOLINTNEXTLINE(*-pointer-arithmetic)
            to[i] = std::move(from[i]);
        }
    }
}

} // namespace asl
