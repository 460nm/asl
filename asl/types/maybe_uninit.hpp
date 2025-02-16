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

    template<typename... Args>
    explicit constexpr maybe_uninit(in_place_t, Args&&... args)
        requires constructible_from<T, Args&&...>
        : m_value{ASL_FWD(args)...}
    {}

    constexpr maybe_uninit(const maybe_uninit&) requires trivially_copy_constructible<T> = default;
    constexpr maybe_uninit(const maybe_uninit&) requires (!trivially_copy_constructible<T>) {} // NOLINT

    constexpr maybe_uninit(maybe_uninit&&) requires trivially_move_constructible<T> = default;
    constexpr maybe_uninit(maybe_uninit&&) requires (!trivially_move_constructible<T>) {} // NOLINT

    constexpr maybe_uninit& operator=(const maybe_uninit&) requires trivially_copy_assignable<T> = default;
    constexpr maybe_uninit& operator=(const maybe_uninit&) requires (!trivially_copy_assignable<T>) {}

    constexpr maybe_uninit& operator=(maybe_uninit&&) requires trivially_move_assignable<T> = default;
    constexpr maybe_uninit& operator=(maybe_uninit&&) requires (!trivially_move_assignable<T>) {}

    constexpr ~maybe_uninit() requires trivially_destructible<T> = default;
    constexpr ~maybe_uninit() requires (!trivially_destructible<T>) {} // NOLINT

    // @Safety Value must not have been initialized yet
    template<typename... Args>
    constexpr void construct_unsafe(Args&&... args)
        requires constructible_from<T, Args&&...>
    {
        construct_at<T>(&m_value, ASL_FWD(args)...);
    }

    // @Safety Value must have been initialized
    template<typename U>
    constexpr void assign_unsafe(U&& value)
        requires assignable_from<T&, U&&>
    {
        m_value = ASL_FWD(value);
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
    constexpr const T& as_init_unsafe() const& { return m_value; }
    constexpr T&       as_init_unsafe() &      { return m_value; }
    constexpr T&&      as_init_unsafe() &&     { return ASL_MOVE(m_value); }
};

} // namespace asl
