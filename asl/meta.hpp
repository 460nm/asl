#pragma once

namespace asl {

template<typename T, T kValue> struct integral_constant { static constexpr T value = kValue; };
template<bool B> using bool_constant = integral_constant<bool, B>;

using true_type  = bool_constant<true>;
using false_type = bool_constant<false>;

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

template<typename T>           struct _is_array_helper       : false_type {};
template<typename T>           struct _is_array_helper<T[]>  : true_type  {};
template<typename T, size_t N> struct _is_array_helper<T[N]> : true_type  {};

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
template<typename T> concept is_object      = is_scalar<T> || is_array<T> || is_class<T> || is_union<T>;

} // namespace asl
