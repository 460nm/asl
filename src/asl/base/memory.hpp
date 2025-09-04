// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#include "asl/base/support.hpp"
#include "asl/base/meta.hpp"

// [new.delete]

[[nodiscard]]
constexpr void* operator new(size_t, void* ptr) noexcept
{
    return ptr;
}

[[nodiscard]]
constexpr void* operator new[](size_t, void* ptr) noexcept
{
    return ptr;
}

namespace std
{

template<typename T>
[[nodiscard]] constexpr T* launder(T* p) noexcept
    requires (!asl::is_func<T> && !asl::is_void<T>)
{
    return __builtin_launder(p);
}

} // namespace std

namespace asl
{

template<typename T>
[[nodiscard]]
constexpr T* address_of(T& obj)
{
    return __builtin_addressof(obj);
}

template<typename T>
void address_of(const T&& obj) = delete;

} // namespace asl

