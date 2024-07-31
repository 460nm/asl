#pragma once

#include "asl/meta/types.hpp"

namespace asl::internal {
    
template<typename T, typename = void>
inline constexpr bool is_referenceable = false;

template<typename T>
inline constexpr bool is_referenceable<T, void_t<T&>> = true;

template<typename T, bool = is_referenceable<T>>
struct as_ref_helper { using lvalue = T; using rvalue = T; };

template<typename T>
struct as_ref_helper<T, true> { using lvalue = T&; using rvalue = T&&; };

template<typename T> struct un_ref_helper { using type = T; };
template<typename T> struct un_ref_helper<T&> { using type = T; };
template<typename T> struct un_ref_helper<T&&> { using type = T; };

template<typename T>
struct is_ref_helper
{
    static inline constexpr bool lref = false;
    static inline constexpr bool rref = false;
};

template<typename T>
struct is_ref_helper<T&>
{
    static inline constexpr bool lref = true;
    static inline constexpr bool rref = false;
};

template<typename T>
struct is_ref_helper<T&&>
{
    static inline constexpr bool lref = false;
    static inline constexpr bool rref = true;
};

} // namespace asl::internal

