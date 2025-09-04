// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

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

// [meta.trans.other]

template<typename T> struct type_identity { using type = T; };

template<typename...>
using void_t = void;

struct empty {};

struct in_place_t {};

template<bool B, typename T, typename F> struct _conditional_impl : type_identity<F> {};
template<typename T, typename F> struct _conditional_impl<true, T, F> : type_identity<T> {};

template<bool B, typename T, typename F>
using conditional_t = _conditional_impl<B, T, F>::type;

template<typename T>
using underlying_type_t = __underlying_type(T);

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

template<typename T>
using add_const_t = const T;

template<typename T>
using add_volatile_t = volatile T;

template<typename T>
using add_cv_t = const volatile T;

// [meta.rel]

template<typename U, typename V> struct _is_same_impl : false_type {};
template<typename T> struct _is_same_impl<T, T> : true_type {};

template<typename U, typename V>
concept is_same = _is_same_impl<U, V>::value;

// [meta.unary.cat]

template<typename U, typename V>
concept _is_cv_same = is_same<remove_cv_t<U>, remove_cv_t<V>>;

template<typename T, typename... A>
concept _is_cv_any = (_is_cv_same<T, A> || ...);

template<typename T>
concept is_void = _is_cv_same<T, void>;

template<typename T>
concept is_nullptr = _is_cv_same<T, decltype(nullptr)>;

template<typename T>
concept is_integral = _is_cv_any<T,
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
concept is_floating_point = _is_cv_any<T, float, double, long double>;

template<typename T> struct _is_array_impl : false_type {};
template<typename T> struct _is_array_impl<T[]> : true_type {};
template<typename T, is_integral auto N> struct _is_array_impl<T[N]> : true_type {};

template<typename T>
concept is_array = _is_array_impl<T>::value;

template<typename T> struct _is_ptr_impl : false_type {};
template<typename T> struct _is_ptr_impl<T*> : true_type {};

template<typename T>
concept is_ptr = _is_ptr_impl<remove_cv_t<T>>::value;

template<typename T> struct _is_lref_impl : false_type {};
template<typename T> struct _is_lref_impl<T&> : true_type {};

template<typename T>
concept is_lref = _is_lref_impl<T>::value;

template<typename T> struct _is_rref_impl : false_type {};
template<typename T> struct _is_rref_impl<T&&> : true_type {};

template<typename T>
concept is_rref = _is_rref_impl<T>::value;

template<typename T>
concept is_enum = __is_enum(T);

template<typename T>
concept is_class = __is_class(T);

template<typename T>
concept is_union = __is_union(T);

template<typename T> struct _is_fun_impl : false_type {};
template<typename R, typename... Args> struct _is_fun_impl<R (Args...)> : true_type {};
template<typename R, typename... Args> struct _is_fun_impl<R (Args...) &> : true_type {};
template<typename R, typename... Args> struct _is_fun_impl<R (Args...) &&> : true_type {};
template<typename R, typename... Args> struct _is_fun_impl<R (Args...) const> : true_type {};
template<typename R, typename... Args> struct _is_fun_impl<R (Args...) const &> : true_type {};
template<typename R, typename... Args> struct _is_fun_impl<R (Args...) const &&> : true_type {};
template<typename R, typename... Args> struct _is_fun_impl<R (Args...) volatile> : true_type {};
template<typename R, typename... Args> struct _is_fun_impl<R (Args...) volatile &> : true_type {};
template<typename R, typename... Args> struct _is_fun_impl<R (Args...) volatile &&> : true_type {};
template<typename R, typename... Args> struct _is_fun_impl<R (Args...) const volatile> : true_type {};
template<typename R, typename... Args> struct _is_fun_impl<R (Args...) const volatile &> : true_type {};
template<typename R, typename... Args> struct _is_fun_impl<R (Args...) const volatile &&> : true_type {};
template<typename R, typename... Args> struct _is_fun_impl<R (Args...) noexcept> : true_type {};
template<typename R, typename... Args> struct _is_fun_impl<R (Args...) & noexcept> : true_type {};
template<typename R, typename... Args> struct _is_fun_impl<R (Args...) && noexcept> : true_type {};
template<typename R, typename... Args> struct _is_fun_impl<R (Args...) const noexcept> : true_type {};
template<typename R, typename... Args> struct _is_fun_impl<R (Args...) const & noexcept> : true_type {};
template<typename R, typename... Args> struct _is_fun_impl<R (Args...) const && noexcept> : true_type {};
template<typename R, typename... Args> struct _is_fun_impl<R (Args...) volatile noexcept> : true_type {};
template<typename R, typename... Args> struct _is_fun_impl<R (Args...) volatile & noexcept> : true_type {};
template<typename R, typename... Args> struct _is_fun_impl<R (Args...) volatile && noexcept> : true_type {};
template<typename R, typename... Args> struct _is_fun_impl<R (Args...) const volatile noexcept> : true_type {};
template<typename R, typename... Args> struct _is_fun_impl<R (Args...) const volatile & noexcept> : true_type {};
template<typename R, typename... Args> struct _is_fun_impl<R (Args...) const volatile && noexcept> : true_type {};

template<typename T>
concept is_func = _is_fun_impl<T>::value;

template<typename T> struct _is_member_func_ptr_impl : false_type {};
template<is_func T, typename C> struct _is_member_func_ptr_impl<T C::*> : true_type {};

template<typename T>
concept is_member_func_ptr = _is_member_func_ptr_impl<remove_cv_t<T>>::value;

template<typename T> struct _is_member_ptr_impl : false_type {};
template<typename T, typename C> struct _is_member_ptr_impl<T C::*> : true_type {};

template<typename T>
concept is_member_object_ptr = _is_member_ptr_impl<remove_cv_t<T>>::value && !is_member_func_ptr<T>;

// [meta.unary.comp]

template<typename T>
concept is_ref = is_lref<T> || is_rref<T>;

template<typename T>
concept is_arithmetic = is_integral<T> || is_floating_point<T>;

template<typename T>
concept is_fundamental = is_arithmetic<T> || is_nullptr<T> || is_void<T>;

template<typename T>
concept is_object = !is_void<T> && !is_ref<T> && !is_func<T>;

template<typename T>
concept is_member_ptr = is_member_func_ptr<T> || is_member_object_ptr<T>;

template<typename T>
concept is_scalar = is_arithmetic<T> || is_enum<T> || is_ptr<T> || is_member_ptr<T> || is_nullptr<T>;

template<typename T>
concept is_compound = is_array<T> || is_func<T> || is_ptr<T> || is_ref<T> || is_class<T> || is_union<T> || is_member_ptr<T> || is_enum<T>;

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
concept is_trivially_copyable = __is_trivially_copyable(T);

template<typename T>         struct has_unique_object_representations       : false_type {};
template<is_integral T>      struct has_unique_object_representations<T>    : true_type {};
template<is_enum T>          struct has_unique_object_representations<T>    : true_type {};
template<is_ptr T>           struct has_unique_object_representations<T>    : true_type {};
template<typename T>         struct has_unique_object_representations<T[]>  : has_unique_object_representations<remove_all_extents_t<T>> {};
template<typename T, int N>  struct has_unique_object_representations<T[N]> : has_unique_object_representations<remove_all_extents_t<T>> {};

template<typename T>
static constexpr bool has_unique_object_representations_v = has_unique_object_representations<T>::value;

// [meta.unary.prop.query]

template<typename T> struct _rank_impl    : integral_constant<int, 0> {};
template<is_array T> struct _rank_impl<T> : integral_constant<int, _rank_impl<remove_extent_t<T>>::value + 1> {};

template<typename T>
static constexpr int rank_v = _rank_impl<T>::value;


template<typename T, int N> struct _extent_impl : integral_constant<int, 0> {};
template<typename T, int N> struct _extent_impl<T[N], 0> : integral_constant<int, N> {};
template<typename T, int N> requires (N > 0) struct _extent_impl<T, N> : _extent_impl<remove_extent_t<T>, N - 1> {};

template<typename T, int N = 0>
static constexpr int extent_v = _extent_impl<T, N>::value;

// [meta.trans.ref]

template<typename T> struct _remove_ref_impl : type_identity<T> {};
template<typename T> struct _remove_ref_impl<T&> : type_identity<T> {};
template<typename T> struct _remove_ref_impl<T&&> : type_identity<T> {};

template<typename T>
using remove_ref_t = _remove_ref_impl<T>::type;

template<typename T> struct _add_lref_impl : type_identity<T> {};
template<typename T> requires requires { typename void_t<T&>; }
struct _add_lref_impl<T> : type_identity<T&> {};

template<typename T>
using add_lref_t = _add_lref_impl<T>::type;

template<typename T> struct _add_rref_impl : type_identity<T> {};
template<typename T> requires requires { typename void_t<T&&>; }
struct _add_rref_impl<T> : type_identity<T&&> {};

template<typename T>
using add_rref_t = _add_rref_impl<T>::type;

template<typename T>
using remove_cvref_t = remove_cv_t<remove_ref_t<T>>;

template<typename T, typename U>
using copy_const_t = conditional_t<is_const<T>, add_const_t<U>, U>;

template<typename T, typename U>
using copy_volatile_t = conditional_t<is_volatile<T>, add_volatile_t<U>, U>;

template<typename T, typename U>
using copy_cv_t = copy_volatile_t<T, copy_const_t<T, U>>;

template<typename T, typename U> struct _copy_cvref : type_identity<copy_cv_t<T, U>> {};
template<typename T, typename U> struct _copy_cvref<T&, U> : type_identity<add_lref_t<copy_cv_t<T, U>>> {};
template<typename T, typename U> struct _copy_cvref<T&&, U> : type_identity<add_rref_t<copy_cv_t<T, U>>> {};

template<typename T, typename U>
using _copy_cvref_t = _copy_cvref<T, U>::type;

template<typename T, typename U> struct _copy_cref : type_identity<copy_const_t<T, U>> {};
template<typename T, typename U> struct _copy_cref<T&, U> : type_identity<add_lref_t<copy_const_t<T, U>>> {};
template<typename T, typename U> struct _copy_cref<T&&, U> : type_identity<add_rref_t<copy_const_t<T, U>>> {};

template<typename T, typename U>
using copy_cref_t = _copy_cref<T, U>::type;

// [meta.trans.sign]

template<typename... Ts> struct _type_list {};

template<typename T> struct _type_list_first {};

template<typename T1, typename... Ts>
struct _type_list_first<_type_list<T1, Ts...>> : type_identity<T1> {};

template<
    typename Ts,
    int N,
    bool kIsCompatible = N <= sizeof(typename _type_list_first<Ts>::type) * 8>
struct _first_compatible {};

template<int N, typename Ts>
struct _first_compatible<Ts, N, true>
    : type_identity<typename _type_list_first<Ts>::type>
{};

template<int N, typename T1, typename... Ts>
struct _first_compatible<_type_list<T1, Ts...>, N, false>
    : _first_compatible<_type_list<Ts...>, N>
{};

template<int N>
using smallest_signed_integer_for_width_t = _first_compatible<_type_list<
    signed char,
    signed short int,
    signed int,
    signed long int,
    signed long long int>, N>::type;

template<int N>
using smallest_unsigned_integer_for_width_t = _first_compatible<_type_list<
    unsigned char,
    unsigned short int,
    unsigned int,
    unsigned long int,
    unsigned long long int>, N>::type;

template<typename T> struct _make_unsigned_impl {};

template<> struct _make_unsigned_impl<bool> {};
template<> struct _make_unsigned_impl<signed char>            : type_identity<unsigned char> {};
template<> struct _make_unsigned_impl<signed short int>       : type_identity<unsigned short int> {};
template<> struct _make_unsigned_impl<signed int>             : type_identity<unsigned int> {};
template<> struct _make_unsigned_impl<signed long int>        : type_identity<unsigned long int> {};
template<> struct _make_unsigned_impl<signed long long int>   : type_identity<unsigned long long int> {};
template<> struct _make_unsigned_impl<unsigned char>          : type_identity<unsigned char> {};
template<> struct _make_unsigned_impl<unsigned short int>     : type_identity<unsigned short int> {};
template<> struct _make_unsigned_impl<unsigned int>           : type_identity<unsigned int> {};
template<> struct _make_unsigned_impl<unsigned long int>      : type_identity<unsigned long int> {};
template<> struct _make_unsigned_impl<unsigned long long int> : type_identity<unsigned long long int> {};

template<typename T>
    requires (is_enum<T> || is_integral<T>)
struct _make_unsigned_impl<T>
    : type_identity<smallest_unsigned_integer_for_width_t<sizeof(T) * 8>>
{};

template<typename T>
using make_unsigned_t = copy_cv_t<T, typename _make_unsigned_impl<remove_cv_t<T>>::type>;

template<typename T> struct _make_signed_impl {};

template<> struct _make_signed_impl<bool> {};
template<> struct _make_signed_impl<signed char>            : type_identity<signed char> {};
template<> struct _make_signed_impl<signed short int>       : type_identity<signed short int> {};
template<> struct _make_signed_impl<signed int>             : type_identity<signed int> {};
template<> struct _make_signed_impl<signed long int>        : type_identity<signed long int> {};
template<> struct _make_signed_impl<signed long long int>   : type_identity<signed long long int> {};
template<> struct _make_signed_impl<unsigned char>          : type_identity<signed char> {};
template<> struct _make_signed_impl<unsigned short int>     : type_identity<signed short int> {};
template<> struct _make_signed_impl<unsigned int>           : type_identity<signed int> {};
template<> struct _make_signed_impl<unsigned long int>      : type_identity<signed long int> {};
template<> struct _make_signed_impl<unsigned long long int> : type_identity<signed long long int> {};

template<typename T>
    requires (is_enum<T> || is_integral<T>)
struct _make_signed_impl<T>
    : type_identity<smallest_signed_integer_for_width_t<sizeof(T) * 8>>
{};

template<typename T>
using make_signed_t = copy_cv_t<T, typename _make_signed_impl<remove_cv_t<T>>::type>;

// [meta.trans.ptr]

template<typename T> struct _remove_ptr_impl : type_identity<T> {};
template<typename T> struct _remove_ptr_impl<T*> : type_identity<T> {};
template<typename T> struct _remove_ptr_impl<T* const> : type_identity<T> {};
template<typename T> struct _remove_ptr_impl<T* volatile> : type_identity<T> {};
template<typename T> struct _remove_ptr_impl<T* const volatile> : type_identity<T> {};

template<typename T>
using remove_ptr_t = _remove_ptr_impl<T>::type;

template<typename T> auto _add_ptr_impl(int) -> type_identity<remove_ref_t<T>*>;
template<typename T> auto _add_ptr_impl(...) -> type_identity<T>;

template<typename T>
using add_ptr_t = decltype(_add_ptr_impl<T>(0))::type;

// [meta.trans.other] again

template<typename T>
struct _decay_impl
{
    using U = remove_ref_t<T>;
    using type = conditional_t<
        is_array<U>,
        add_ptr_t<remove_extent_t<U>>,
        conditional_t<
            is_func<U>,
            add_ptr_t<U>,
            remove_cv_t<U>
        >
    >;
};

template<typename T>
using decay_t = _decay_impl<T>::type;

// [declval]

template<typename T>
add_rref_t<T> declval() { static_assert(false); }

// [meta.rel] again

template<typename Base, typename Derived>
concept is_base_of = __is_base_of(Base, Derived);

template<typename From, typename To>
concept is_convertible = __is_convertible(From, To);

template<typename... T> struct common_type;

template<typename... T>
using common_type_t = common_type<T...>::type;

template<typename... T>
struct common_type : __builtin_common_type<common_type_t, type_identity, empty, T...> {};

template<
    typename T,
    typename U,
    template<typename> typename TQual,
    template<typename> typename UQual>
struct basic_common_reference {};

template<typename... T>
struct common_reference;

template<typename... T>
using common_reference_t = common_reference<T...>::type;

template<> struct common_reference<> {};

template<typename T> struct common_reference<T> : type_identity<T> {};

template<typename A>
struct _cr_xref
{
    template<typename U>
    using apply = _copy_cvref_t<A, U>;
};

template<typename X, typename Y>
using _cr_cond_res_t = decltype(false ? declval<X(&)()>()() : declval<Y(&)()>()());

template<
    typename A,
    typename B,
    typename X = remove_ref_t<A>,
    typename Y = remove_ref_t<B>>
struct _cr_common_ref {};

template<typename A, typename B, typename X, typename Y>
requires requires { typename _cr_cond_res_t<copy_cv_t<X, Y>&, copy_cv_t<Y, X>&>; }
struct _cr_common_ref<A&, B&, X, Y>
    : type_identity<_cr_cond_res_t<copy_cv_t<X, Y>&, copy_cv_t<Y, X>&>>
{};

template<typename X, typename Y>
using _cr_common_ref_C_t = remove_ref_t<typename _cr_common_ref<X&, Y&>::type>&&;

template<typename A, typename B, typename X, typename Y>
requires (
    requires { typename _cr_common_ref_C_t<X, Y>; } &&
    is_convertible<A&&, _cr_common_ref_C_t<X, Y>> &&
    is_convertible<B&&, _cr_common_ref_C_t<X, Y>>
)
struct _cr_common_ref<A&&, B&&, X, Y> : type_identity<_cr_common_ref_C_t<X, Y>> {};

template<typename X, typename Y>
using _cr_common_ref_D_t = _cr_common_ref<const X&, Y&>::type;

template<typename A, typename B, typename X, typename Y>
requires (
    requires { typename _cr_common_ref_D_t<X, Y>; } &&
    is_convertible<A&&, _cr_common_ref_D_t<X, Y>>
)
struct _cr_common_ref<A&&, B&, X, Y> : type_identity<_cr_common_ref_D_t<X, Y>> {};

template<typename A, typename B, typename X, typename Y>
struct _cr_common_ref<A&, B&&, X, Y> : _cr_common_ref<B&&, A&> {};

template<typename T1, typename T2>
struct _common_reference_3_b4;

template<typename T1, typename T2>
struct _common_reference_3_b3 : _common_reference_3_b4<T1, T2> {};

template<typename T1, typename T2>
struct _common_reference_3_b2 : _common_reference_3_b3<T1, T2> {};

template<typename T1, typename T2>
struct _common_reference_3_b1 : _common_reference_3_b2<T1, T2> {};

template<typename T1, typename T2>
requires (
    is_ref<T1> && is_ref<T2> &&
    requires { typename _cr_common_ref<T1, T2>::type; } &&
    is_convertible<add_ptr_t<T1>, add_ptr_t<typename _cr_common_ref<T1, T2>::type>> &&
    is_convertible<add_ptr_t<T2>, add_ptr_t<typename _cr_common_ref<T1, T2>::type>>
)
struct _common_reference_3_b1<T1, T2> : _cr_common_ref<T1, T2> {};

template<typename T1, typename T2>
using _basic_common_reference_t = basic_common_reference<
        remove_cvref_t<T1>,
        remove_cvref_t<T2>,
        _cr_xref<T1>::template apply,
        _cr_xref<T2>::template apply
    >::type;

template<typename T1, typename T2>
requires requires { typename _basic_common_reference_t<T1, T2>; }
struct _common_reference_3_b2<T1, T2> : type_identity<_basic_common_reference_t<T1, T2>> {};

template<typename T1, typename T2>
requires requires { typename _cr_cond_res_t<T1, T2>; }
struct _common_reference_3_b3<T1, T2> : type_identity<_cr_cond_res_t<T1, T2>> {};

template<typename T1, typename T2>
struct _common_reference_3_b4 : common_type<T1, T2> {};

template<typename T1, typename T2>
struct common_reference<T1, T2> : _common_reference_3_b1<T1, T2> {};

template<typename... T>
struct _common_reference_3 {};

template<typename T1, typename T2, typename... R>
requires requires { typename common_reference_t<T1, T2>; }
struct _common_reference_3<T1, T2, R...> : common_reference<common_reference_t<T1, T2>, R...> {};

template<typename T1, typename T2, typename... R>
struct common_reference<T1, T2, R...> : _common_reference_3<T1, T2, R...> {};

} // namespace asl

namespace std
{

// [forward]

template<typename T>
constexpr T&& forward(asl::remove_ref_t<T>& t) noexcept
{
    return static_cast<T&&>(t);
}

template<typename T>
constexpr T&& forward(asl::remove_ref_t<T>&& t) noexcept // NOLINT(*-param-not-moved)
{
    return static_cast<T&&>(t);
}

template<typename T>
constexpr asl::remove_ref_t<T>&& move(T&& t) noexcept // NOLINT(*-missing-std-forward)
{
    return static_cast<asl::remove_ref_t<T>&&>(t);
}

// [utility.exchange]

template<typename T, typename U = T>
constexpr T exchange(T& obj, U&& new_val)
{
    T old_val = std::move(obj);
    obj = std::forward<U>(new_val);
    return old_val;
}

template<typename T, typename U>
using _forward_like_copy_const_t = asl::copy_const_t<asl::remove_ref_t<T>, asl::remove_ref_t<U>>;

template<typename T, typename U>
using _forward_like_override_ref_t = asl::conditional_t<asl::is_rref<T>, asl::remove_ref_t<U>&&, U&>;

template<typename T, typename U>
using _forward_like_t = _forward_like_override_ref_t<T&&, _forward_like_copy_const_t<T, U>>;

template<typename T, typename U>
[[nodiscard]] constexpr auto forward_like(U&& x) noexcept // NOLINT(*-missing-std-forward)
    -> _forward_like_t<T, U>
{
    return static_cast<_forward_like_t<T, U>>(x);
}

// [utility.swap]

template<class T>
constexpr void swap(T& a, T& b)
requires (asl::is_move_constructible<T> && asl::is_move_assignable<T>)
{
    T tmp = std::move(a);
    a = std::move(b);
    b = std::move(tmp);
}

// [utility.as.const]

template<typename T>
constexpr asl::add_const_t<T>& as_const(T& t) noexcept
{
    return t;
}

template<typename T>
void as_const(const T&&) = delete;

} // namespace std

namespace asl
{

// [func.require]

template<typename Base, typename Derived>
concept _is_same_or_derived_from = is_same<Base, remove_cvref_t<Derived>> || is_base_of<Base, remove_cvref_t<Derived>>;

template<typename C, typename Self, typename... Args>
auto invoke(is_func auto C::* f, Self&& self, Args&&... args)
    -> decltype((declval<Self>().* f)(declval<Args>()...))
    requires (
        _is_same_or_derived_from<C, Self> &&
        requires { (declval<Self>().* f)(declval<Args>()...); }
    )
{
    return (std::forward<Self>(self).* f)(std::forward<Args>(args)...);
}

template<typename C, typename Self, typename... Args>
auto invoke(is_func auto C::* f, Self&& self, Args&&... args)
    -> decltype(((*declval<Self>()).* f)(declval<Args>()...))
    requires (
        !_is_same_or_derived_from<C, Self> &&
        requires { ((*declval<Self>()).* f)(declval<Args>()...); }
    )
{
    return ((*std::forward<Self>(self)).* f)(std::forward<Args>(args)...);
}

template<typename C, typename Self>
auto&& invoke(auto C::* f, Self&& self)
    requires (
        is_member_object_ptr<decltype(f)> &&
        _is_same_or_derived_from<C, Self> &&
        requires { declval<Self>().* f; }
    )
{
    return std::forward<Self>(self).* f;
}

template<typename C, typename Self>
auto&& invoke(auto C::* f, Self&& self)
    requires (
        is_member_object_ptr<decltype(f)> &&
        !_is_same_or_derived_from<C, Self> &&
        requires { (*declval<Self>()).* f; }
    )
{
    return (*std::forward<Self>(self)).* f;
}

template<typename F, typename... Args>
auto invoke(F&& f, Args&&... args)
    -> decltype(declval<F>()(declval<Args>()...))
    requires (
        !is_member_ptr<F> &&
        requires { declval<F>()(declval<Args>()...); }
    )
{
    return std::forward<F>(f)(std::forward<Args>(args)...);
}

template<typename R, typename F, typename... Args>
R invoke_r(F&& f, Args&&... args)
    requires (
        is_void<R> ||
        requires { declval<void (R)>()(invoke(declval<F>(), declval<Args>()...)); }
    )
{
    if constexpr (is_void<R>)
    {
        static_cast<void>(invoke(std::forward<F>(f), std::forward<Args>(args)...));
    }
    else
    {
        return invoke(std::forward<F>(f), std::forward<Args>(args)...);
    }
}

// [meta.trans.other] again

struct _invoke_tag {};

template<typename... Args> struct _invoke_result_impl {};

template<typename F, typename... Args> struct _invoke_result_impl<_invoke_tag, F, Args...>
    : type_identity<decltype(invoke(declval<F>(), declval<Args>()...))>
{};

template<typename F, typename...Args>
using invoke_result_t = _invoke_result_impl<_invoke_tag, F, Args...>::type;

// [meta.rel] again

template<typename F, typename... Args>
concept is_invocable = requires { invoke(declval<F>(), declval<Args>()...); };

template<typename R, typename F, typename... Args>
concept is_invocable_r = requires { invoke_r<R>(declval<F>(), declval<Args>()...); };

// [concept.same]

template<typename T, typename U>
concept same_as = is_same<T, U>;

// [concept.derived]

template<typename Derived, typename Base>
concept derived_from = is_base_of<Base, Derived> &&
    is_convertible<const volatile Derived*, const volatile Base*>;

// [concept.convertible]

template<typename From, typename To>
concept convertible_to = is_convertible<From, To> &&
    requires {
        static_cast<To>(declval<From>());
    };

// [concept.commonref]

template<typename T, typename U>
concept common_reference_with =
    same_as<common_reference_t<T, U>, common_reference_t<U, T>> &&
    convertible_to<T, common_reference_t<T, U>> &&
    convertible_to<U, common_reference_t<T, U>>;

// [concept.common]

template<typename T, typename U>
concept common_with =
    same_as<common_type_t<T, U>, common_type_t<U, T>> &&
    requires {
        static_cast<common_type_t<T, U>>(declval<T>());
        static_cast<common_type_t<T, U>>(declval<U>());
    } &&
    common_reference_with<
        add_lref_t<const T>,
        add_lref_t<const U>> &&
    common_reference_with<
        add_lref_t<common_type_t<T, U>>,
        common_reference_t<
            add_lref_t<const T>,
            add_lref_t<const U>>>;

// [concepts.arithmetic]

template<typename T>
concept integral = is_integral<T>;

template<typename T>
concept signed_integral = integral<T> && is_signed<T>;

template<typename T>
concept unsigned_integral = integral<T> && !is_signed<T>;

template<typename T>
concept floating_point = is_floating_point<T>;

// [concept.assignable]

template<typename LHS, typename RHS>
concept assignable_from =
    is_lref<LHS> &&
    common_reference_with<const remove_ref_t<LHS>&, const remove_ref_t<RHS>&> &&
    requires (LHS lhs, RHS&& rhs) {
        { lhs = std::forward<RHS>(rhs) } -> same_as<LHS>;
    };

// [concept.swappable]

template<typename T>
concept swappable = requires(T& a, T& b) { std::swap(a, b); };

template<typename T, typename U>
concept swappable_with =
    common_reference_with<T, U> &&
    requires(T&& t, U&& u) {
        std::swap(std::forward<T>(t), std::forward<T>(t));
        std::swap(std::forward<U>(u), std::forward<U>(u));
        std::swap(std::forward<T>(t), std::forward<U>(u));
        std::swap(std::forward<U>(u), std::forward<T>(t));
    };

// [concept.destructible]

template<typename T>
concept destructible = is_destructible<T>;

// [concept.constructible]

template<typename T, typename... Args>
concept constructible_from = destructible<T> && is_constructible<T, Args...>;

// [concept.default.init]

template<typename T>
concept default_initializable = constructible_from<T> &&
    requires {
        new T{};
        new T;
    };

// [concept.moveconstructible]

template<typename T>
concept move_constructible = constructible_from<T, T> && convertible_to<T, T>;

// [concept.copyconstructible]

template<typename T>
concept copy_constructible =
    move_constructible<T> &&
    constructible_from<T, T&> && convertible_to<T&, T> &&
    constructible_from<T, const T&> && convertible_to<const T&, T> &&
    constructible_from<T, const T> && convertible_to<const T, T>;

// [concept.booleantestable]

template<typename T>
concept _boolean_testable_impl = convertible_to<T, bool>;

template<typename T>
concept _boolean_testable =
    _boolean_testable_impl<T> && requires (T&& t) {
        { !std::forward<T>(t) } -> _boolean_testable_impl;
    };

// [concept.comparisoncommontype]

template<typename T, typename U, typename C = common_reference_t<const T&, const U&>>
concept _comparison_common_type_with_impl =
    same_as<
        common_reference_t<const T&, const U&>,
        common_reference_t<const U&, const T&>> &&
    requires {
        requires convertible_to<const T&, const C&> || convertible_to<T, const C&>;
        requires convertible_to<const U&, const C&> || convertible_to<U, const C&>;
    };

template<typename T, typename U>
concept _comparison_common_type_with =
    _comparison_common_type_with_impl<remove_cvref_t<T>, remove_cvref_t<U>>;

// [concept.equalitycomparable]

template<typename T, typename U>
concept weakly_equality_comparable_with =
    requires(const remove_ref_t<T>& t, const remove_ref_t<U>& u) {
        { t == u } -> _boolean_testable;
        { t != u } -> _boolean_testable;
        { u == t } -> _boolean_testable;
        { u != t } -> _boolean_testable;
    };

template<typename T>
concept equality_comparable = weakly_equality_comparable_with<T, T>;

template<class T, class U>
concept equality_comparable_with =
    equality_comparable<T> && equality_comparable<U> &&
    _comparison_common_type_with<T, U> &&
    equality_comparable<
        common_reference_t<
            const remove_ref_t<T>&,
            const remove_ref_t<U>&>> &&
    weakly_equality_comparable_with<T, U>;

// [cmp.concept]

template<typename T, typename U>
concept _partially_ordered_with =
    requires(const remove_ref_t<T>& t, const remove_ref_t<U>& u) {
        { t <  u } -> _boolean_testable;
        { t >  u } -> _boolean_testable;
        { t <= u } -> _boolean_testable;
        { t >= u } -> _boolean_testable;
        { u <  t } -> _boolean_testable;
        { u >  t } -> _boolean_testable;
        { u <= t } -> _boolean_testable;
        { u >= t } -> _boolean_testable;
    };

// [concept.totallyordered]

template<typename T>
concept totally_ordered =
    equality_comparable<T> && _partially_ordered_with<T, T>;

template<typename T, typename U>
concept totally_ordered_with =
    totally_ordered<T> && totally_ordered<U> &&
    equality_comparable_with<T, U> &&
    totally_ordered<
        common_reference_t<
            const remove_ref_t<T>&,
            const remove_ref_t<U>&>> &&
    _partially_ordered_with<T, U>;

// [concepts.object]

template<typename T>
concept movable =
    is_object<T> && move_constructible<T> &&
    assignable_from<T&, T> && swappable<T>;

template<typename T>
concept copyable =
    copy_constructible<T> && movable<T> && assignable_from<T&, T&> &&
    assignable_from<T&, const T&> && assignable_from<T&, const T>;

template<typename T>
concept semiregular = copyable<T> && default_initializable<T>;

template<typename T>
concept regular = semiregular<T> && equality_comparable<T>;

// [concept.invocable]

template<typename F, typename... Args>
concept invocable = requires(F&& f, Args&&... args) {
    invoke(std::forward<F>(f), std::forward<Args>(args)...);
};

// [concept.regularinvocable]

template<typename F, typename... Args>
concept regular_invocable = invocable<F, Args...>;

// [concept.predicate]

template<typename F, typename... Args>
concept predicate =
    regular_invocable<F, Args...> && _boolean_testable<invoke_result_t<F, Args...>>;

// [concept.relation]

template<typename R, typename T, typename U>
concept relation =
    predicate<R, T, T> && predicate<R, U, U> &&
    predicate<R, T, U> && predicate<R, U, T>;

// [concept.equiv]

template<typename R, typename T, typename U>
concept equivalence_relation = relation<R, T, U>;

// [concept.strictweakorder]

template<typename R, typename T, typename U>
concept strict_weak_order = relation<R, T, U>;

struct niche_t {};

template<typename T>
concept has_niche = constructible_from<T, niche_t> && weakly_equality_comparable_with<T, niche_t>;

template<typename T>
concept is_niche = same_as<remove_cvref_t<T>, niche_t>;

template<typename From, typename To>
concept _dereferenceable_as_convertible = requires(From& t)
{
    { *t } -> convertible_to<To&>;
};

template<typename From, typename To>
concept derefs_as = is_object<To> &&
    (convertible_to<remove_ref_t<From>&, To&> || _dereferenceable_as_convertible<remove_ref_t<From>, To>);

template<typename To, derefs_as<To> From>
constexpr auto&& deref(From&& from) // NOLINT(*-missing-std-forward)
{
    if constexpr (_dereferenceable_as_convertible<From, To>)
    {
        using deref_type = decltype(*declval<From&&>());
        return static_cast<copy_cref_t<deref_type, To>>(*static_cast<From&&>(from));
    }
    else
    {
        return static_cast<copy_cref_t<From&&, To>>(from);
    }
}

template<is_enum E>
constexpr underlying_type_t<E> to_underlying(E e)
{
    return __builtin_bit_cast(underlying_type_t<E>, e);
}

} // namespace asl
