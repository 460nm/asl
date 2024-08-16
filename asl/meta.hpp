#pragma once

namespace asl {

template<typename T> struct id { using type = T; };

template<typename T, T kValue> struct integral_constant { static constexpr T value = kValue; };
template<bool B> using bool_constant = integral_constant<bool, B>;

using true_type  = bool_constant<true>;
using false_type = bool_constant<false>;

template<bool kSelect, typename U, typename V> struct _select_helper             { using type = V; };
template<typename U, typename V>               struct _select_helper<true, U, V> { using type = U; };

template<bool kSelect, typename U, typename V> using select_t = _select_helper<kSelect, U, V>::type;

template<typename T> auto _as_lref_helper(int) -> id<T&>;
template<typename T> auto _as_lref_helper(...) -> id<T>;

template<typename T> auto _as_rref_helper(int) -> id<T&&>;
template<typename T> auto _as_rref_helper(...) -> id<T>;

template<typename T> using as_lref_t = decltype(_as_lref_helper<T>(0))::type;
template<typename T> using as_rref_t = decltype(_as_rref_helper<T>(0))::type;

template<typename T, typename... Args> concept constructible = __is_constructible(T, Args...);

template<typename T> concept default_constructible = constructible<T>;
template<typename T> concept copy_constructible    = constructible<T, as_lref_t<const T>>;
template<typename T> concept move_constructible    = constructible<T, as_rref_t<T>>;

template<typename T, typename... Args> concept trivially_constructible = __is_trivially_constructible(T, Args...);

template<typename T> concept trivially_default_constructible = trivially_constructible<T>;
template<typename T> concept trivially_copy_constructible    = trivially_constructible<T, as_lref_t<const T>>;
template<typename T> concept trivially_move_constructible    = trivially_constructible<T, as_rref_t<T>>;

template<typename T, typename... Args> concept assignable = __is_assignable(T, Args...);

template<typename T> concept copy_assignable = assignable<as_lref_t<T>, as_lref_t<const T>>;
template<typename T> concept move_assignable = assignable<as_lref_t<T>, as_rref_t<T>>;

template<typename T, typename... Args> concept trivially_assignable = __is_trivially_assignable(T, Args...);

template<typename T> concept trivially_copy_assignable = trivially_assignable<as_lref_t<T>, as_lref_t<const T>>;
template<typename T> concept trivially_move_assignable = trivially_assignable<as_lref_t<T>, as_rref_t<T>>;

template<typename T> concept trivially_destructible = __is_trivially_destructible(T);

using nullptr_t = decltype(nullptr);

template<typename T> struct un_ref      { using type = T; };
template<typename T> struct un_ref<T&>  { using type = T; };
template<typename T> struct un_ref<T&&> { using type = T; };
template<typename T> using  un_ref_t = un_ref<T>::type;

template<typename T> struct un_const          { using type = T; };
template<typename T> struct un_const<const T> { using type = T; };
template<typename T> using  un_const_t = un_const<T>::type;

template<typename T> struct un_volatile             { using type = T; };
template<typename T> struct un_volatile<volatile T> { using type = T; };
template<typename T> using  un_volatile_t = un_volatile<T>::type;

template<typename T> using un_qual_t = un_const_t<un_volatile_t<T>>;

template<typename T> using un_qualref_t = un_qual_t<un_ref_t<T>>;

template<typename U, typename V> struct _is_same_helper       : false_type {};
template<typename T>             struct _is_same_helper<T, T> : true_type  {};

template<typename U, typename V>
concept is_same = _is_same_helper<U, V>::value && _is_same_helper<V, U>::value;

template<typename T> struct _is_const_helper          : false_type {};
template<typename T> struct _is_const_helper<const T> : true_type  {};

template<typename T> concept is_const = _is_const_helper<T>::value;

template<typename T> concept is_void    = is_same<un_qual_t<T>, void>;
template<typename T> concept is_nullptr = is_same<un_qual_t<T>, nullptr_t>;

template<typename T> concept is_integral       = __is_integral(T);
template<typename T> concept is_floating_point = __is_floating_point(T);
template<typename T> concept is_arithmetic     = is_integral<T> || is_floating_point<T>;

template<typename T>            struct _is_array_helper       : false_type {};
template<typename T>            struct _is_array_helper<T[]>  : true_type  {};
template<typename T, int N> struct _is_array_helper<T[N]> : true_type  {};

template<typename T> concept is_array = _is_array_helper<T>::value;

template<typename T> concept is_class = __is_class(T);
template<typename T> concept is_union = __is_union(T);

template<typename T> concept is_enum  = __is_enum(T);

template<typename T> struct _is_ptr_helper     : false_type {};
template<typename T> struct _is_ptr_helper<T*> : true_type  {};

template<typename T> concept is_ptr = _is_ptr_helper<un_qual_t<T>>::value;

template<typename T> struct _is_ref_helper      { static constexpr bool lref = false; static constexpr bool rref = false; };
template<typename T> struct _is_ref_helper<T&>  { static constexpr bool lref = true;  static constexpr bool rref = false; };
template<typename T> struct _is_ref_helper<T&&> { static constexpr bool lref = false; static constexpr bool rref = true;  };

template<typename T> concept is_lref = _is_ref_helper<T>::lref;
template<typename T> concept is_rref = _is_ref_helper<T>::rref;
template<typename T> concept is_ref  = is_lref<T> || is_rref<T>;

template<typename T> concept is_func = !is_const<const T> && !is_ref<T>;

template<typename T>             struct _is_member_ptr_helper         : false_type {};
template<typename T, typename U> struct _is_member_ptr_helper<T U::*> : true_type  {};

template<typename T>             struct _is_member_func_ptr_helper         : false_type {};
template<typename T, typename U> struct _is_member_func_ptr_helper<T U::*> : bool_constant<is_func<T>>  {};

template<typename T> concept is_member_ptr        = _is_member_ptr_helper<un_qual_t<T>>::value;
template<typename T> concept is_member_func_ptr   = _is_member_func_ptr_helper<un_qual_t<T>>::value;
template<typename T> concept is_member_object_ptr = is_member_ptr<T> && !is_member_func_ptr<T>;

template<typename T> concept is_fundamental = is_arithmetic<T> || is_void<T> || is_nullptr<T>;
template<typename T> concept is_compound    = !is_fundamental<T>;
template<typename T> concept is_scalar      = (is_fundamental<T> && !is_void<T>) || is_enum<T> || is_ptr<T> || is_member_ptr<T>;
template<typename T> concept is_object      = is_scalar<T> || is_class<T> || is_union<T>;

template<typename T> concept is_empty = is_void<T> || __is_empty(T);

struct empty {};

template<typename T> using devoid_t = select_t<is_void<T>, empty, T>;

template<typename F> struct _tame_helper { using type = F; };

#define ASL_TAME_IMPL(SUFFIX) \
    template<typename Ret, typename... Args> \
    struct _tame_helper<Ret(Args...) SUFFIX> { using type = Ret(Args...); }

ASL_TAME_IMPL();
ASL_TAME_IMPL(const);
ASL_TAME_IMPL(volatile);
ASL_TAME_IMPL(volatile const);
ASL_TAME_IMPL(&&);
ASL_TAME_IMPL(const &&);
ASL_TAME_IMPL(volatile &&);
ASL_TAME_IMPL(volatile const &&);
ASL_TAME_IMPL(&);
ASL_TAME_IMPL(const &);
ASL_TAME_IMPL(volatile &);
ASL_TAME_IMPL(volatile const &);
ASL_TAME_IMPL(noexcept);
ASL_TAME_IMPL(const noexcept);
ASL_TAME_IMPL(volatile noexcept);
ASL_TAME_IMPL(volatile const noexcept);
ASL_TAME_IMPL(&& noexcept);
ASL_TAME_IMPL(const && noexcept);
ASL_TAME_IMPL(volatile && noexcept);
ASL_TAME_IMPL(volatile const && noexcept);
ASL_TAME_IMPL(& noexcept);
ASL_TAME_IMPL(const & noexcept);
ASL_TAME_IMPL(volatile & noexcept);
ASL_TAME_IMPL(volatile const & noexcept);

#undef ASL_TAME_IMPL

template<typename T> using tame_t = _tame_helper<T>::type;

template<typename T> using as_raw_ptr_t = un_ref_t<tame_t<T>>*;


} // namespace asl
