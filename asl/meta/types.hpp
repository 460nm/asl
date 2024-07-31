#pragma once

#include "asl/meta/internal/types.hpp"

namespace asl {
    
template<typename T> using void_t = void;

template<typename U, typename V>
concept same = internal::is_same<U, V> && internal::is_same<V, U>;

} // namespace asl

