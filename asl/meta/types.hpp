#pragma once

namespace asl {

template<typename T> using void_t = void;

template<typename T, typename = void>
inline constexpr bool is_referenceable = false;

template<typename T>
inline constexpr bool is_referenceable<T, void_t<T&>> = true;

namespace internal {

template<typename T, bool = is_referenceable<T>>
struct as_ref_helper { using lvalue = T; using rvalue = T; };

template<typename T>
struct as_ref_helper<T, true> { using lvalue = T&; using rvalue = T&&; };

template<typename T> struct un_ref_helper { using type = T; };
template<typename T> struct un_ref_helper<T&> { using type = T; };
template<typename T> struct un_ref_helper<T&&> { using type = T; };

} // namespace internal

template<typename T> using as_ref_t = internal::as_ref_helper<T>::lvalue;
template<typename T> using as_rref_t = internal::as_ref_helper<T>::rvalue;

template<typename T> using un_ref_t = internal::un_ref_helper<T>::type;

#define AslMove(expr_) (static_cast<::asl::as_rref_t<::asl::un_ref_t<decltype(expr_)>>>(expr_))
#define AslForward(expr_) (static_cast<::asl::as_rref_t<decltype(expr_)>>(expr_))

} // namespace asl

