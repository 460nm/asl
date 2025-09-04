// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#include "asl/base/meta.hpp"
#include "asl/base/assert.hpp"
#include "asl/base/bits.hpp"
#include "asl/base/floats.hpp"

namespace asl
{

template<integral T>
constexpr bool is_pow2(T x)
{
    using unsigned_type = make_unsigned_t<T>;
    return x > 0 && has_single_bit(static_cast<unsigned_type>(x));
}

template<integral T>
constexpr T round_down_pow2(T x, T div)
{
    ASL_ASSERT(is_pow2(div));
    return x & (-div);
}

template<integral T>
constexpr T round_up_pow2(T x, T div)
{
    ASL_ASSERT(is_pow2(div));
    return (x + (div - 1)) & (-div);
}

template<is_arithmetic T>
constexpr T min(T a, T b)
{
    return (a < b) ? a : b;

}

template<is_arithmetic T>
constexpr T max(T a, T b)
{
    return (a > b) ? a : b;

}

template<is_arithmetic T>
constexpr T clamp(T x, T a, T b)
{
    return min(max(x, a), b);
}

constexpr float  abs(float x)  { return __builtin_fabsf(x); }
constexpr double abs(double x) { return __builtin_fabs(x); }

template<floating_point T>
bool are_nearly_equal(T a, T b)
{
    // This is a fast path for identical values and correctly handles
    // the case where +0.0 == -0.0.
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wfloat-equal"
    if (a == b) { return true; }
    #pragma clang diagnostic pop

    // NaNs are never equal to anything, including themselves.
    if (is_nan(a) || is_nan(b)) { return false; }

    // Infinities are only equal if they are identical (which is handled by the
    // `a == b` check above). If one is infinity and the other is not, they
    // are not equal. The relative comparison below would fail with infinities.
    if (is_infinity(a) || is_infinity(b)) { return false; }

    static constexpr T kMin = float_traits<T>::kSmallest;
    static constexpr T kEps = float_traits<T>::kEpsilon;

    const T abs_a = abs(a);
    const T abs_b = abs(b);
    const T abs_diff = abs(a - b);

    // The relative error comparison (`|a-b| <= ε * max(|a|, |b|)`) breaks
    // down when `a` and `b` are near zero. In this case, we switch to an
    // absolute error comparison. `DBL_MIN` is the smallest positive
    // normalized double, so it's a good threshold for this check.
    if (abs_a < kMin || abs_b < kMin)
    {
        return abs_diff < kEps;
    }

    // For all other cases, we use the standard relative error formula.
    // The error is scaled by the magnitude of the numbers.
    return abs_diff <= kEps * max(abs_a, abs_b);
}

} // namespace asl

