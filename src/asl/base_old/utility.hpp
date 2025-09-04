// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#include "asl/base/meta.hpp"
#include "asl/base/assert.hpp"

namespace std
{

template<typename T>
[[nodiscard]] constexpr asl::un_ref_t<T>&& move(T&& t) noexcept // NOLINT
{
    return static_cast<asl::un_ref_t<T>&&>(t);
}

template<typename T>
[[nodiscard]] constexpr T&& forward(asl::un_ref_t<T>& t) noexcept // NOLINT
{
    return static_cast<T&&>(t);
}

template< class T >
[[nodiscard]] constexpr T&& forward(asl::un_ref_t<T>&& t) noexcept // NOLINT
{
    return static_cast<T&&>(t);
}

template<typename T, typename U>
[[nodiscard]] constexpr auto forward_like(U&& x) noexcept -> asl::copy_cref_t<T, U> // NOLINT
{
    using return_type = asl::copy_cref_t<T, U&&>;
    return static_cast<return_type>(x);
}

template<typename T>
[[nodiscard]] constexpr T* launder(T* ptr) noexcept // NOLINT
    requires (!asl::is_func<T> && !asl::is_void<T>)
{
    return __builtin_launder(ptr);
}

} // namespace std

namespace asl
{

template<moveable T>
constexpr void swap(T& a, T& b)
{
    T tmp{std::move(a)};
    a = std::move(b);
    b = std::move(tmp);
}

template<typename T, typename U>
T exchange(T& obj, U&& new_value)
{
    T old_value = std::move(obj);
    obj = std::forward<U>(new_value);
    return old_value;
}

template<trivially_copy_constructible U, trivially_copy_constructible T>
constexpr U bit_cast(T value) requires (sizeof(T) == sizeof(U))
{
    return __builtin_bit_cast(U, value);
}

template<is_enum T>
constexpr auto to_underlying(T value)
{
    return static_cast<underlying_t<T>>(value);
}

// NOLINTBEGIN(*-macro-parentheses)
#define ASL_DELETE_COPY(T)                         \
    T(const T&) = delete;                          \
    T& operator=(const T&) = delete

#define ASL_DELETE_MOVE(T)                         \
    T(T&&) = delete;                               \
    T& operator=(T&&) = delete

#define ASL_DELETE_COPY_MOVE(T)                    \
    ASL_DELETE_COPY(T);                            \
    ASL_DELETE_MOVE(T)

#define ASL_DEFAULT_COPY(T)                         \
    T(const T&) = default;                          \
    T& operator=(const T&) = default

#define ASL_DEFAULT_MOVE(T)                         \
    T(T&&) = default;                               \
    T& operator=(T&&) = default

#define ASL_DEFAULT_COPY_MOVE(T)                    \
    ASL_DEFAULT_COPY(T);                            \
    ASL_DEFAULT_MOVE(T)
// NOLINTEND(*-macro-parentheses)

#define ASL_CONCAT2(A, B) A##B
#define ASL_CONCAT(A, B) ASL_CONCAT2(A, B)

} // namespace asl
