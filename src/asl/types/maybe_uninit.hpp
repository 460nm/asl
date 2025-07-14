// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#include "asl/base/meta.hpp"
#include "asl/base/utility.hpp"
#include "asl/memory/memory.hpp"

namespace asl
{

template<is_object T>
union maybe_uninit
{
private:
    T m_value;

public:
    constexpr maybe_uninit() requires trivially_default_constructible<T> = default;
    constexpr maybe_uninit() requires (!trivially_default_constructible<T>) {} // NOLINT

    explicit constexpr maybe_uninit(in_place_t, auto&&... args)
        requires constructible_from<T, decltype(args)...>
        : m_value{std::forward<decltype(args)>(args)...}
    {}

    constexpr maybe_uninit(const maybe_uninit&) requires trivially_copy_constructible<T> = default;
    constexpr maybe_uninit(const maybe_uninit&) requires (!trivially_copy_constructible<T>) {} // NOLINT

    constexpr maybe_uninit(maybe_uninit&&) requires trivially_move_constructible<T> = default;
    constexpr maybe_uninit(maybe_uninit&&) requires (!trivially_move_constructible<T>) {} // NOLINT

    constexpr maybe_uninit& operator=(const maybe_uninit&) requires trivially_copy_assignable<T> = default;
    constexpr maybe_uninit& operator=(const maybe_uninit&) requires (!trivially_copy_assignable<T>) { return *this; } // NOLINT

    constexpr maybe_uninit& operator=(maybe_uninit&&) requires trivially_move_assignable<T> = default;
    constexpr maybe_uninit& operator=(maybe_uninit&&) requires (!trivially_move_assignable<T>) { return *this; } // NOLINT

    constexpr ~maybe_uninit() requires trivially_destructible<T> = default;
    constexpr ~maybe_uninit() requires (!trivially_destructible<T>) {} // NOLINT

    // @Safety Value must not have been initialized yet
    constexpr void construct_unsafe(auto&&... args)
        requires constructible_from<T, decltype(args)...>
    {
        construct_at<T>(&m_value, std::forward<decltype(args)>(args)...);
    }

    // @Safety Value must have been initialized
    constexpr void assign_unsafe(auto&& value)
        requires assignable_from<T&, decltype(value)>
    {
        m_value = std::forward<decltype(value)>(value);
    }

    // @Safety Value must have been initialized
    constexpr void destroy_unsafe()
    {
        if constexpr (!trivially_destructible<T>)
        {
            destroy(&m_value);
        }
    }

    // @Safety Value must have been initialized
    constexpr auto&& as_init_unsafe(this auto&& self)
    {
        return std::forward<decltype(self)>(self).m_value;
    }
};

} // namespace asl
