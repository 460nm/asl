#pragma once

#include "asl/meta/types.hpp"
#include "asl/meta/funcs.hpp"
#include "asl/meta/refs.hpp"
#include "asl/meta/internal/quals.hpp"

namespace asl {
    
template<typename T> concept is_const = internal::is_const<T>;
template<typename T> concept is_volatile = !is_any_ref<T> && !is_func<T> && same<T, typename internal::volatile_helper<T>::add>;

template<typename T> using as_const_t = internal::const_helper<T>::add;
template<typename T> using un_const_t = internal::const_helper<T>::remove;

template<typename T> using as_volatile_t = internal::volatile_helper<T>::add;
template<typename T> using un_volatile_t = internal::volatile_helper<T>::remove;

template<typename T> using un_qual_t = un_volatile_t<un_const_t<T>>;

} // namespace asl

