#pragma once

#include "asl/meta/internal/refs.hpp"

namespace asl {

template<typename T> concept referenceable = internal::is_referenceable<T>;

template<typename T> concept is_ref = internal::is_ref_helper<T>::lref;
template<typename T> concept is_rref = internal::is_ref_helper<T>::rref;
template<typename T> concept is_any_ref = is_ref<T> || is_rref<T>;

template<typename T> using as_ref_t = internal::as_ref_helper<T>::lvalue;
template<typename T> using as_rref_t = internal::as_ref_helper<T>::rvalue;

template<typename T> using un_ref_t = internal::un_ref_helper<T>::type;

#define AslMove(expr_) (static_cast<::asl::as_rref_t<::asl::un_ref_t<decltype(expr_)>>>(expr_))
#define AslForward(expr_) (static_cast<::asl::as_rref_t<decltype(expr_)>>(expr_))

} // namespace asl
