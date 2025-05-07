// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#include "asl/base/integers.hpp"

namespace asl {

struct source_location
{
    const char* file;
    int         line;

    explicit source_location(
        const char* file_ = __builtin_FILE(),
        int line_ = __builtin_LINE())
        : file{file_}
        , line{line_}
    {}
};

struct empty {};

template<typename T> struct id { using type = T; };

struct in_place_t {};
static constexpr in_place_t in_place{};

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

template<typename T> auto _as_ptr_helper(int) -> id<T*>;
template<typename T> auto _as_ptr_helper(...) -> id<T>;

template<typename T> using as_ptr_t = decltype(_as_ptr_helper<T>(0))::type;

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

template<typename T> concept copyable = copy_constructible<T> && copy_assignable<T>;
template<typename T> concept moveable = move_constructible<T> && move_assignable<T>;

using nullptr_t = decltype(nullptr);

template<typename T> struct _un_const_helper          { using type = T; };
template<typename T> struct _un_const_helper<const T> { using type = T; };

template<typename T> using un_const_t = _un_const_helper<T>::type;

template<typename T> using as_const_t = const T;

template<typename T> struct _is_const_helper          : false_type {};
template<typename T> struct _is_const_helper<const T> : true_type {};

template<typename T> concept is_const = _is_const_helper<T>::value;

template<typename T> struct _un_volatile_helper             { using type = T; };
template<typename T> struct _un_volatile_helper<volatile T> { using type = T; };

template<typename T> using un_volatile_t = _un_volatile_helper<T>::type;

template<typename T> using un_cv_t = un_volatile_t<un_const_t<T>>;

template<typename T> using un_cvref_t = un_cv_t<un_ref_t<T>>;

template<typename T> concept is_void = same_as<void, un_cv_t<T>>;

template<typename T> struct _is_ref_helper      { static constexpr bool l = false; static constexpr bool r = false; };
template<typename T> struct _is_ref_helper<T&>  { static constexpr bool l = true;  static constexpr bool r = false; };
template<typename T> struct _is_ref_helper<T&&> { static constexpr bool l = false; static constexpr bool r = true;  };

template<typename T> concept is_ref = _is_ref_helper<T>::l || _is_ref_helper<T>::r;
template<typename T> concept is_rref = _is_ref_helper<T>::r;
template<typename T> concept is_lref = _is_ref_helper<T>::l;

template<typename From, typename To, bool kLref = is_lref<From>, bool kRref = is_rref<From>>
struct _copy_ref_helper { using type = To; };

template<typename From, typename To>
struct _copy_ref_helper<From, To, true, false> { using type = as_lref_t<To>; };

template<typename From, typename To>
struct _copy_ref_helper<From, To, false, true> { using type = as_rref_t<To>; };

template<typename From, typename To, bool kIsConst = is_const<un_ref_t<From>>>
struct _copy_const_helper { using type = To; };

template<typename From, typename To>
struct _copy_const_helper<From, To, true> { using type = const To; };

template<typename From, typename To> using copy_cref_t =
    _copy_ref_helper<From, typename _copy_const_helper<From, un_cvref_t<To>>::type>::type;

template<typename T> struct _is_ptr_helper     : false_type {};
template<typename T> struct _is_ptr_helper<T*> : true_type {};

template<typename T> concept is_ptr = _is_ptr_helper<un_cv_t<T>>::value;

template<typename From, typename To>
concept convertible_to = __is_convertible(From, To);

template<typename Derived, typename Base>
concept derived_from = __is_class(Derived) && __is_class(Base) && convertible_to<const volatile Derived*, const volatile Base*>;

template<typename Derived, typename Base>
concept same_or_derived_from = same_as<un_cvref_t<Derived>, Base> || derived_from<un_cvref_t<Derived>, Base>;

template<typename T> struct _tame_helper { using type = T; };

#define TAME_HELPER_IMPL(TRAILING)                                  \
    template<typename R, typename... Args>                          \
    struct _tame_helper<R(Args...) TRAILING> { using type = R(Args...); } // NOLINT(*-parentheses)

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

template<typename T>             struct _is_member_ptr_helper         : false_type {};
template<typename C, typename T> struct _is_member_ptr_helper<T C::*> : true_type
{
    static constexpr bool kIsFunc = is_func<T>;
};

template<typename T> concept is_member_ptr = _is_member_ptr_helper<un_cv_t<T>>::value;

template<typename T> concept is_member_func_ptr = is_member_ptr<T> && _is_member_ptr_helper<un_cv_t<T>>::kIsFunc;
template<typename T> concept is_member_data_ptr = is_member_ptr<T> && !_is_member_ptr_helper<un_cv_t<T>>::kIsFunc;

template<typename T> concept is_object = !is_void<T> && !is_ref<T> && !is_func<T>;

template<typename T>        struct _array_helper       : false_type { using type = T; };
template<typename T>        struct _array_helper<T[]>  : true_type  { using type = T; };
template<typename T, int N> struct _array_helper<T[N]> : true_type  { using type = T; };

template<typename T> concept is_array = _array_helper<T>::value;

template<typename T> using remove_extent_t = _array_helper<T>::type;

template<typename T>
using decay_t =
    select_t<
        is_array<un_ref_t<T>>,
        as_ptr_t<remove_extent_t<un_ref_t<T>>>,
        select_t<
            is_func<un_ref_t<T>>,
            as_ptr_t<un_ref_t<T>>,
            un_cv_t<un_ref_t<T>>>>;

template<typename T> struct _is_floating_point_helper         : false_type {};
template<>           struct _is_floating_point_helper<float>  : true_type  {};
template<>           struct _is_floating_point_helper<double> : true_type  {};

template<typename T> concept is_floating_point = _is_floating_point_helper<un_cv_t<T>>::value;

template<typename T> struct _integer_traits
{
    static constexpr bool kSigned = false;
    static constexpr bool kUnsigned = false;
};

template<> struct _integer_traits<uint8_t>
{
    static constexpr bool kSigned = false;
    static constexpr bool kUnsigned = true;
    using as_signed = int8_t;
};

template<> struct _integer_traits<uint16_t>
{
    static constexpr bool kSigned = false;
    static constexpr bool kUnsigned = true;
    using as_signed = int16_t;
};

template<> struct _integer_traits<uint32_t>
{
    static constexpr bool kSigned = false;
    static constexpr bool kUnsigned = true;
    using as_signed = int32_t;
};

template<> struct _integer_traits<uint64_t>
{
    static constexpr bool kSigned = false;
    static constexpr bool kUnsigned = true;
    using as_signed = int64_t;
};

template<> struct _integer_traits<int8_t>
{
    static constexpr bool kSigned = true;
    static constexpr bool kUnsigned = false;
    using as_unsigned = uint8_t;
};

template<> struct _integer_traits<int16_t>
{
    static constexpr bool kSigned = true;
    static constexpr bool kUnsigned = false;
    using as_unsigned = uint16_t;
};

template<> struct _integer_traits<int32_t>
{
    static constexpr bool kSigned = true;
    static constexpr bool kUnsigned = false;
    using as_unsigned = uint32_t;
};

template<> struct _integer_traits<int64_t>
{
    static constexpr bool kSigned = true;
    static constexpr bool kUnsigned = false;
    using as_unsigned = uint64_t;
};

template<typename T> concept is_signed_integer   = _integer_traits<T>::kSigned;
template<typename T> concept is_unsigned_integer = _integer_traits<T>::kUnsigned;

template<typename T> concept is_integer = is_signed_integer<T> || is_unsigned_integer<T>;

template<is_signed_integer T>   using as_unsigned_integer = _integer_traits<T>::as_unsigned;
template<is_unsigned_integer T> using as_signed_integer   = _integer_traits<T>::as_signed;

template<typename T> concept is_enum = __is_enum(T);

template<is_enum T> using underlying_t = __underlying_type(T);

template<typename T>     struct is_uniquely_represented : false_type {};
template<is_integer T>   struct is_uniquely_represented<T> : true_type {};
template<is_enum T>      struct is_uniquely_represented<T> : true_type {};
template<>               struct is_uniquely_represented<uint128_t> : true_type {};
template<>               struct is_uniquely_represented<byte> : true_type {};

template<typename T> concept uniquely_represented = is_uniquely_represented<un_cv_t<T>>::value;

template<typename T, typename U>
concept equality_comparable_with = requires (const un_cvref_t<T>& a, const un_cvref_t<U>& b)
{
    { a == b } -> same_as<bool>;
    { b == a } -> same_as<bool>;
    { a != b } -> same_as<bool>;
    { b != a } -> same_as<bool>;
};

template<typename T> concept equality_comparable = equality_comparable_with<T, T>;

struct niche_t {};

template<typename T>
concept has_niche = constructible_from<T, niche_t> && equality_comparable_with<T, niche_t>;

template<typename T>
concept is_niche = same_as<un_cvref_t<T>, niche_t>;

template<typename From, typename To>
concept _dereferenceable_as_convertible = requires(From& t)
{
    { *t } -> convertible_to<To&>;
};

template<typename From, typename To>
concept derefs_as = is_object<To> &&
    (convertible_to<un_ref_t<From>&, To&> || _dereferenceable_as_convertible<un_ref_t<From>, To>);

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

} // namespace asl
