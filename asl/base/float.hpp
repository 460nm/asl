// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#include "asl/base/meta.hpp"

using float32_t = float;
using float64_t = double;

namespace asl
{

template<typename T> struct float_traits {};

#define ASL_FLOAT_TRAITS(T, INF, NAN, EPS, SMALLEST) \
    template<> struct float_traits<T> \
    { \
        static constexpr T kInfinity{__builtin_bit_cast(T, INF)}; \
        static constexpr T kNaN{__builtin_bit_cast(T, NAN)}; \
        static constexpr T kEpsilon{EPS}; \
        static constexpr T kSmallest{__builtin_bit_cast(T, SMALLEST)}; \
    };

ASL_FLOAT_TRAITS(
    float32_t,
    0x7F800000,
    0x7FC00000,
    __builtin_bit_cast(float32_t, 0x3F800001) - float32_t{1},
    0x00800000
);

ASL_FLOAT_TRAITS(
    float64_t,
    0x7FF0000000000000,
    0x7FF8000000000000,
    __builtin_bit_cast(float64_t, 0x3FF0000000000001) - float64_t{1},
    0x0010000000000000
);

template<is_floating_point T> constexpr T infinity() { return float_traits<T>::kInfinity; }

template<is_floating_point T> constexpr T nan() { return float_traits<T>::kNaN; }

template<is_floating_point T> constexpr bool is_infinity(T f) { return __builtin_isinf(f); }

template<is_floating_point T> constexpr bool is_nan(T f) { return __builtin_isnan(f); }

} // namespace asl

