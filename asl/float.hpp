#pragma once

#include "asl/meta.hpp"

namespace asl
{

template<is_floating_point T> constexpr T infinity() { return __builtin_inf(); }

template<is_floating_point T> constexpr T nan() { return static_cast<T>(__builtin_nanf("")); }

template<is_floating_point T> constexpr bool is_infinity(T f) { return __builtin_isinf(f); }

template<is_floating_point T> constexpr bool is_nan(T f) { return __builtin_isnan(f); }

} // namespace asl

