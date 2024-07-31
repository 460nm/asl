#pragma once

#include "asl/meta/internal/quals.hpp"
#include "asl/meta/refs.hpp"

namespace asl {

template<typename T> concept is_func = internal::is_const<typename internal::const_helper<T>::remove> && !is_any_ref<T>;

} // namespace asl

