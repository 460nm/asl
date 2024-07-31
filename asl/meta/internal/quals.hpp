#pragma once

#include "asl/meta/types.hpp"

namespace asl::internal {

template<typename T> struct const_helper { using add = const T; using remove = T; };
template<typename T> struct const_helper<const T> { using add = const T; using remove = T; };

template<typename T> concept is_const = same<T, typename internal::const_helper<T>::add>;

template<typename T> struct volatile_helper { using add = volatile T; using remove = T; };
template<typename T> struct volatile_helper<volatile T> { using add = volatile T; using remove = T; };

} // namespace asl::internal

