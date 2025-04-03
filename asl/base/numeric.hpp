// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#include "asl/base/integers.hpp"
#include "asl/base/bit.hpp"
#include "asl/base/meta.hpp"

namespace asl
{

template<is_integer T>
constexpr bool is_pow2(T x)
{
    using unsigned_type = select_t<is_unsigned_integer<T>, T, as_unsigned_integer<T>>;
    return x > 0 && has_single_bit(static_cast<unsigned_type>(x));
}

template<typename T>
concept is_numeric = is_integer<T> || is_floating_point<T>;

template<is_numeric T>
constexpr T min(T a, T b)
{
    return (a <= b) ? a : b;
}

template<is_numeric T>
constexpr T max(T a, T b)
{
    return (a >= b) ? a : b;
}

template<is_numeric T>
constexpr T clamp(T x, T a, T b)
{
    return min(max(x, a), b);
}

} // namespace asl

