// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

namespace asl
{

// [meta.help]

template<typename T, T kValue>
struct integral_constant
{
    static constexpr T value{kValue};
};

template<bool kValue>
struct bool_constant : integral_constant<bool, kValue> {};

using true_type  = bool_constant<true>;
using false_type = bool_constant<false>;

template<typename T> struct type_identity { using type = T; };

// [meta.trans.cv]

template<typename T> struct _remove_const_impl : type_identity<T> {};
template<typename T> struct _remove_const_impl<const T> : type_identity<T> {};

template<typename T>
using remove_const_t = _remove_const_impl<T>::type;

template<typename T> struct _remove_volatile_impl : type_identity<T> {};
template<typename T> struct _remove_volatile_impl<volatile T> : type_identity<T> {};

template<typename T>
using remove_volatile_t = _remove_volatile_impl<T>::type;

template<typename T>
using remove_cv_t = remove_const_t<remove_volatile_t<T>>;

// @Todo(base_remake) template<class T> struct add_const;
// @Todo(base_remake) template<class T> struct add_volatile;
// @Todo(base_remake) template<class T> struct add_cv;

// [meta.rel]

template<typename U, typename V> struct _is_same_impl : false_type {};
template<typename T> struct _is_same_impl<T, T> : true_type {};

template<typename U, typename V>
concept is_same = _is_same_impl<U, V>::value;

// @Todo(base_remake) template<class Base, class Derived> struct is_base_of;
// @Todo(base_remake) template<class From, class To> struct is_convertible;

// @Todo(base_remake) template<class Fn, class... ArgTypes> struct is_invocable;
// @Todo(base_remake) template<class R, class Fn, class... ArgTypes> struct is_invocable_r;

// [meta.unary.cat]

template<typename U, typename V>
concept _is_cv_same_v = is_same<remove_cv_t<U>, remove_cv_t<V>>;

template<typename T, typename... A>
concept _is_cv_any_v = (_is_cv_same_v<T, A> || ...);

template<typename T>
concept is_void = _is_cv_same_v<T, void>;

template<typename T>
concept is_null_pointer = _is_cv_same_v<T, decltype(nullptr)>;

template<typename T>
concept is_integral = _is_cv_any_v<T,
    bool,
    char,
    wchar_t,
    char8_t,
    char16_t,
    char32_t,
    signed char,
    signed short int,
    signed int,
    signed long int,
    signed long long int,
    unsigned char,
    unsigned short int,
    unsigned int,
    unsigned long int,
    unsigned long long int>;

template<typename T>
concept is_floating_point = _is_cv_any_v<T, float, double, long double>;

template<typename T> struct _is_array_impl : false_type {};
template<typename T> struct _is_array_impl<T[]> : true_type {};
template<typename T, is_integral auto N> struct _is_array_impl<T[N]> : true_type {};

template<typename T>
concept is_array = _is_array_impl<T>::value;

template<typename T> struct _is_pointer_impl : false_type {};
template<typename T> struct _is_pointer_impl<T*> : true_type {};

template<typename T>
concept is_pointer = _is_pointer_impl<remove_cv_t<T>>::value;

template<typename T> struct _is_lvalue_reference_impl : false_type {};
template<typename T> struct _is_lvalue_reference_impl<T&> : true_type {};

template<typename T>
concept is_lvalue_reference = _is_lvalue_reference_impl<T>::value;

template<typename T> struct _is_rvalue_reference_impl : false_type {};
template<typename T> struct _is_rvalue_reference_impl<T&&> : true_type {};

template<typename T>
concept is_rvalue_reference = _is_rvalue_reference_impl<T>::value;

template<typename T>
concept is_enum = __is_enum(T);

template<typename T>
concept is_class = __is_class(T);

template<typename T>
concept is_union = __is_union(T);

template<typename T> struct _is_function_impl : false_type {};
template<typename R, typename... Args> struct _is_function_impl<R (Args...)> : true_type {};
template<typename R, typename... Args> struct _is_function_impl<R (Args...) &> : true_type {};
template<typename R, typename... Args> struct _is_function_impl<R (Args...) &&> : true_type {};
template<typename R, typename... Args> struct _is_function_impl<R (Args...) const> : true_type {};
template<typename R, typename... Args> struct _is_function_impl<R (Args...) const &> : true_type {};
template<typename R, typename... Args> struct _is_function_impl<R (Args...) const &&> : true_type {};
template<typename R, typename... Args> struct _is_function_impl<R (Args...) volatile> : true_type {};
template<typename R, typename... Args> struct _is_function_impl<R (Args...) volatile &> : true_type {};
template<typename R, typename... Args> struct _is_function_impl<R (Args...) volatile &&> : true_type {};
template<typename R, typename... Args> struct _is_function_impl<R (Args...) const volatile> : true_type {};
template<typename R, typename... Args> struct _is_function_impl<R (Args...) const volatile &> : true_type {};
template<typename R, typename... Args> struct _is_function_impl<R (Args...) const volatile &&> : true_type {};
template<typename R, typename... Args> struct _is_function_impl<R (Args...) noexcept> : true_type {};
template<typename R, typename... Args> struct _is_function_impl<R (Args...) & noexcept> : true_type {};
template<typename R, typename... Args> struct _is_function_impl<R (Args...) && noexcept> : true_type {};
template<typename R, typename... Args> struct _is_function_impl<R (Args...) const noexcept> : true_type {};
template<typename R, typename... Args> struct _is_function_impl<R (Args...) const & noexcept> : true_type {};
template<typename R, typename... Args> struct _is_function_impl<R (Args...) const && noexcept> : true_type {};
template<typename R, typename... Args> struct _is_function_impl<R (Args...) volatile noexcept> : true_type {};
template<typename R, typename... Args> struct _is_function_impl<R (Args...) volatile & noexcept> : true_type {};
template<typename R, typename... Args> struct _is_function_impl<R (Args...) volatile && noexcept> : true_type {};
template<typename R, typename... Args> struct _is_function_impl<R (Args...) const volatile noexcept> : true_type {};
template<typename R, typename... Args> struct _is_function_impl<R (Args...) const volatile & noexcept> : true_type {};
template<typename R, typename... Args> struct _is_function_impl<R (Args...) const volatile && noexcept> : true_type {};

template<typename T>
concept is_function = _is_function_impl<T>::value;

template<typename T> struct _is_member_function_pointer_impl : false_type {};
template<is_function T, typename C> struct _is_member_function_pointer_impl<T C::*> : true_type {};

template<typename T>
concept is_member_function_pointer = _is_member_function_pointer_impl<remove_cv_t<T>>::value;

template<typename T> struct _is_member_pointer_impl : false_type {};
template<typename T, typename C> struct _is_member_pointer_impl<T C::*> : true_type {};

template<typename T>
concept is_member_object_pointer = _is_member_pointer_impl<remove_cv_t<T>>::value && !is_member_function_pointer<T>;

// [meta.unary.comp]

template<typename T>
concept is_reference = is_lvalue_reference<T> || is_rvalue_reference<T>;

template<typename T>
concept is_arithmetic = is_integral<T> || is_floating_point<T>;

template<typename T>
concept is_fundamental = is_arithmetic<T> || is_null_pointer<T> || is_void<T>;

template<typename T>
concept is_object = !is_void<T> && !is_reference<T> && !is_function<T>;

template<typename T>
concept is_member_pointer = is_member_function_pointer<T> || is_member_object_pointer<T>;

template<typename T>
concept is_scalar = is_arithmetic<T> || is_enum<T> || is_pointer<T> || is_member_pointer<T> || is_null_pointer<T>;

template<typename T>
concept is_compound = is_array<T> || is_function<T> || is_pointer<T> || is_reference<T> || is_class<T> || is_union<T> || is_member_pointer<T> || is_enum<T>;


// [meta.trans.arr]

template<typename T> struct _remove_extent_impl : type_identity<T> {};
template<typename T> struct _remove_extent_impl<T[]> : type_identity<T> {};
template<typename T, is_integral auto N> struct _remove_extent_impl<T[N]> : type_identity<T> {};

template<typename T>
using remove_extent_t = _remove_extent_impl<T>::type;

template<typename T> struct _remove_all_extents_impl : type_identity<T> {};
template<typename T> struct _remove_all_extents_impl<T[]> : _remove_all_extents_impl<T> {};
template<typename T, is_integral auto N> struct _remove_all_extents_impl<T[N]> : _remove_all_extents_impl<T> {};

template<typename T>
using remove_all_extents_t = _remove_all_extents_impl<T>::type;

// [meta.unary.prop]

template<typename T> struct _is_const_impl : false_type {};
template<typename T> struct _is_const_impl<const T> : true_type {};

template<typename T>
concept is_const = _is_const_impl<T>::value;

template<typename T> struct _is_volatile_impl : false_type {};
template<typename T> struct _is_volatile_impl<volatile T> : true_type {};

template<typename T>
concept is_volatile = _is_volatile_impl<T>::value;

template<typename T>
concept is_signed = is_arithmetic<T> && (T{-1} < T{0});

template<typename T>
concept is_unsigned = is_arithmetic<T> && !is_signed<T>;

template<typename T> struct _is_bounded_array_impl : false_type {};
template<typename T, is_integral auto N> struct _is_bounded_array_impl<T[N]> : true_type {};

template<typename T>
concept is_bounded_array = _is_bounded_array_impl<T>::value;

template<typename T>
concept is_unbounded_array = is_array<T> && !is_bounded_array<T>;

template<typename T>
concept is_scoped_enum = __is_scoped_enum(T);

template<typename T, typename... Args>
concept is_constructible = __is_constructible(T, Args...);

template<typename T>
concept is_default_constructible = is_constructible<T>;

template<typename T>
concept is_copy_constructible = is_constructible<T, const T&>;

template<typename T>
concept is_move_constructible = is_constructible<T, T&&>;

template<typename T, typename... Args>
concept is_assignable = __is_assignable(T&, Args...);

template<typename T>
concept is_copy_assignable = is_assignable<T, const T&>;

template<typename T>
concept is_move_assignable = is_assignable<T, T&&>;

template<typename T>
concept is_destructible = __is_destructible(T);

template<typename T, typename... Args>
concept is_trivially_constructible = __is_trivially_constructible(T, Args...);

template<typename T>
concept is_trivially_default_constructible = is_trivially_constructible<T>;

template<typename T>
concept is_trivially_copy_constructible = is_trivially_constructible<T, const T&>;

template<typename T>
concept is_trivially_move_constructible = is_trivially_constructible<T, T&&>;

template<typename T, typename... Args>
concept is_trivially_assignable = __is_trivially_assignable(T&, Args...);

template<typename T>
concept is_trivially_copy_assignable = is_trivially_assignable<T, const T&>;

template<typename T>
concept is_trivially_move_assignable = is_trivially_assignable<T, T&&>;

template<typename T>
concept is_trivially_destructible = __is_trivially_destructible(T);

template<typename T>
concept _is_trivially_copyable_class =
    (
        (
            is_copy_constructible<T> ||
            is_move_constructible<T> ||
            is_copy_assignable<T> ||
            is_move_assignable<T>
        ) &&
        (
            (!is_copy_constructible<T> || is_trivially_copy_constructible<T>) &&
            (!is_move_constructible<T> || is_trivially_move_constructible<T>) &&
            (!is_copy_assignable<T> || is_trivially_copy_assignable<T>) &&
            (!is_move_assignable<T> || is_trivially_move_assignable<T>)
        ) && is_trivially_destructible<T>
    );

template<typename T>
concept is_trivially_copyable =
    is_scalar<remove_all_extents_t<T>> ||
    _is_trivially_copyable_class<remove_all_extents_t<T>>;

// @Todo(base_remake) template<class T> struct has_unique_object_representations;

// @Todo(base_remake) [meta.unary.prop.query]
// @Todo(base_remake) [meta.trans.ref]
// @Todo(base_remake) [meta.trans.sign]
// @Todo(base_remake) [meta.trans.ptr]

} // namespace asl
