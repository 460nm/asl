#pragma once

#include "asl/integers.hpp"

namespace asl {

struct empty {};

template<typename T> struct id { using type = T; };

template<typename... Args> static constexpr isize_t types_count = sizeof...(Args);

template<typename T, T kValue> struct integral_constant { static constexpr T value = kValue; };
template<bool B> using bool_constant = integral_constant<bool, B>;

using true_type  = bool_constant<true>;
using false_type = bool_constant<false>;

template<bool kSelect, typename U, typename V> struct _select_helper             { using type = V; };
template<typename U, typename V>               struct _select_helper<true, U, V> { using type = U; };

template<bool kSelect, typename U, typename V> using select_t = _select_helper<kSelect, U, V>::type;

template<typename U, typename V> struct _same_as_helper       : false_type {};
template<typename T>             struct _same_as_helper<T, T> : true_type {};

template<typename U, typename V> concept same_as = _same_as_helper<U, V>::value && _same_as_helper<V, U>::value;

template<typename T> auto _as_lref_helper(int) -> id<T&>;
template<typename T> auto _as_lref_helper(...) -> id<T>;

template<typename T> auto _as_rref_helper(int) -> id<T&&>;
template<typename T> auto _as_rref_helper(...) -> id<T>;

template<typename T> using as_lref_t = decltype(_as_lref_helper<T>(0))::type;
template<typename T> using as_rref_t = decltype(_as_rref_helper<T>(0))::type;

template<typename T> consteval as_rref_t<T> declval() {}

template<typename T> struct _un_ref_t      { using type = T; };
template<typename T> struct _un_ref_t<T&>  { using type = T; };
template<typename T> struct _un_ref_t<T&&> { using type = T; };

template<typename T> using un_ref_t = _un_ref_t<T>::type;

template<typename T, typename... Args> concept constructible_from = __is_constructible(T, Args...);

template<typename T> concept default_constructible = constructible_from<T>;
template<typename T> concept copy_constructible    = constructible_from<T, as_lref_t<const T>>;
template<typename T> concept move_constructible    = constructible_from<T, as_rref_t<T>>;

template<typename T, typename... Args> concept trivially_constructible_from = __is_trivially_constructible(T, Args...);

template<typename T> concept trivially_default_constructible = trivially_constructible_from<T>;
template<typename T> concept trivially_copy_constructible    = trivially_constructible_from<T, as_lref_t<const T>>;
template<typename T> concept trivially_move_constructible    = trivially_constructible_from<T, as_rref_t<T>>;

template<typename T, typename... Args> concept assignable_from = __is_assignable(T, Args...);

template<typename T> concept copy_assignable = assignable_from<as_lref_t<T>, as_lref_t<const T>>;
template<typename T> concept move_assignable = assignable_from<as_lref_t<T>, as_rref_t<T>>;

template<typename T, typename... Args> concept trivially_assignable_from = __is_trivially_assignable(T, Args...);

template<typename T> concept trivially_copy_assignable = trivially_assignable_from<as_lref_t<T>, as_lref_t<const T>>;
template<typename T> concept trivially_move_assignable = trivially_assignable_from<as_lref_t<T>, as_rref_t<T>>;

template<typename T> concept trivially_destructible = __is_trivially_destructible(T);

template<typename T> concept trivially_copyable = __is_trivially_copyable(T);

template<typename To, typename From>
concept convertible_from = __is_convertible(From, To);

template<typename Derived, class Base>
concept derived_from = __is_class(Derived) && __is_class(Base) && convertible_from<const volatile Base*, const volatile Derived*>;

using nullptr_t = decltype(nullptr);

template<typename T> struct _un_const_helper          { using type = T; };
template<typename T> struct _un_const_helper<const T> { using type = T; };

template<typename T> using un_const_t = _un_const_helper<T>::type;

template<typename T> struct _is_const_helper          : false_type {};
template<typename T> struct _is_const_helper<const T> : true_type {};

template<typename T> concept is_const = _is_const_helper<T>::value;

template<typename T> struct _un_volatile_helper             { using type = T; };
template<typename T> struct _un_volatile_helper<volatile T> { using type = T; };

template<typename T> using un_volatile_t = _un_volatile_helper<T>::type;

template<typename T> using un_cv_t = un_volatile_t<un_const_t<T>>;

template<typename T> using un_cvref_t = un_ref_t<un_cv_t<T>>;

template<typename T> concept is_void = same_as<void, un_cv_t<T>>;

template<typename T> struct _is_ref_helper      { static constexpr bool l = false; static constexpr bool r = false; };
template<typename T> struct _is_ref_helper<T&>  { static constexpr bool l = true;  static constexpr bool r = false; };
template<typename T> struct _is_ref_helper<T&&> { static constexpr bool l = false; static constexpr bool r = true;  };

template<typename T> concept is_ref = _is_ref_helper<T>::l || _is_ref_helper<T>::r;

template<typename T> struct _is_ptr_helper     : false_type {};
template<typename T> struct _is_ptr_helper<T*> : true_type {};

template<typename T> concept is_ptr = _is_ptr_helper<un_cv_t<T>>::value;

template<typename T> struct _tame_helper { using type = T; };

#define TAME_HELPER_IMPL(TRAILING)                                  \
    template<typename R, typename... Args>                          \
    struct _tame_helper<R(Args...) TRAILING> { using type = R(Args...); }

TAME_HELPER_IMPL();
TAME_HELPER_IMPL(&);
TAME_HELPER_IMPL(&&);
TAME_HELPER_IMPL(const);
TAME_HELPER_IMPL(const &);
TAME_HELPER_IMPL(const &&);
TAME_HELPER_IMPL(volatile);
TAME_HELPER_IMPL(volatile &);
TAME_HELPER_IMPL(volatile &&);
TAME_HELPER_IMPL(const volatile);
TAME_HELPER_IMPL(const volatile &);
TAME_HELPER_IMPL(const volatile &&);
TAME_HELPER_IMPL(noexcept);
TAME_HELPER_IMPL(& noexcept);
TAME_HELPER_IMPL(&& noexcept);
TAME_HELPER_IMPL(const noexcept);
TAME_HELPER_IMPL(const & noexcept);
TAME_HELPER_IMPL(const && noexcept);
TAME_HELPER_IMPL(volatile noexcept);
TAME_HELPER_IMPL(volatile & noexcept);
TAME_HELPER_IMPL(volatile && noexcept);
TAME_HELPER_IMPL(const volatile noexcept);
TAME_HELPER_IMPL(const volatile & noexcept);
TAME_HELPER_IMPL(const volatile && noexcept);

#undef TAME_HELPER_IMPL

template<typename T> using tame_t = _tame_helper<T>::type;

template<typename T>                   struct _is_func_helper             : false_type {};
template<typename R, typename... Args> struct _is_func_helper<R(Args...)> : true_type {};

template<typename T> concept is_func = _is_func_helper<tame_t<T>>::value;

template<typename T> concept is_object = !is_void<T> && !is_ref<T> && !is_func<T>;

template<typename T>        struct _is_array_helper       : false_type {};
template<typename T>        struct _is_array_helper<T[]>  : true_type  {};
template<typename T, int N> struct _is_array_helper<T[N]> : true_type  {};

template<typename T> concept is_array = _is_array_helper<T>::value;

} // namespace asl
