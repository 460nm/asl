// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#include "asl/base/assert.hpp"
#include "asl/base/meta.hpp"
#include "asl/base/functional.hpp"
#include "asl/base/annotations.hpp"
#include "asl/types/maybe_uninit.hpp"
#include "asl/hashing/hash.hpp"

namespace asl
{

struct nullopt_t {};
static constexpr nullopt_t nullopt{};

// @Todo(option) Reference
// @Todo(option) Function
// @Todo(option) Arrays

template<is_object T> class option;

namespace option_internal
{

template<typename T, typename U>
concept not_constructible_from_option =
    !constructible_from<T, option<U>&> &&
    !constructible_from<T, const option<U>&> &&
    !constructible_from<T, option<U>&&> &&
    !constructible_from<T, const option<U>&&>;

template<typename T, typename U>
concept not_assignable_from_option =
    !assignable_from<T&, option<U>&> &&
    !assignable_from<T&, const option<U>&> &&
    !assignable_from<T&, option<U>&&> &&
    !assignable_from<T&, const option<U>&&>;

template<typename T, typename U>
concept not_constructible_assignable_from_option =
    not_constructible_from_option<T, U> &&
    not_assignable_from_option<T, U>;

} // namespace option_internal

template<typename T>
concept is_option = requires
{
    typename T::type;
    requires same_as<un_cvref_t<T>, option<typename T::type>>;
};

template<is_object T>
class option
{
    static constexpr bool kHasNiche = has_niche<T>;

    using HasValueMarker = select_t<kHasNiche, empty, bool>;

    maybe_uninit<T> m_payload{};
    ASL_NO_UNIQUE_ADDRESS HasValueMarker m_has_value{};

    template<is_object U>
    friend class option;

    template<typename... Args>
    constexpr void construct(Args&&... args)
    {
        ASL_ASSERT(!has_value());

        if constexpr (!kHasNiche)
        {
            m_payload.construct_unsafe(std::forward<Args>(args)...);
            m_has_value = true;
        }
        else
        {
            if constexpr (move_assignable<T>)
            {
                m_payload.assign_unsafe(T{std::forward<Args>(args)...});
            }
            else
            {
                m_payload.destroy_unsafe();
                m_payload.construct_unsafe(std::forward<Args>(args)...);
            }
        }
    }

    template<typename U>
    constexpr void assign(U&& arg)
    {
        ASL_ASSERT(has_value());
        m_payload.assign_unsafe(std::forward<U>(arg));
    }

public:
    using type = T;

    constexpr option() : option{nullopt} {}

     // NOLINTNEXTLINE(*explicit*)
    constexpr option(nullopt_t) requires (!kHasNiche) {}

     // NOLINTNEXTLINE(*explicit*)
    constexpr option(nullopt_t) requires kHasNiche : m_payload{in_place, niche_t{}} {}

    template<typename U = T>
    constexpr explicit (!convertible_to<U&&, T>)
    option(U&& value)
        requires (
            kHasNiche &&
            constructible_from<T, U&&> &&
            !same_as<un_cvref_t<U>, option>
        )
        : m_payload{in_place, std::forward<U>(value)}
    {}

    template<typename U = T>
    constexpr explicit (!convertible_to<U&&, T>)
    option(U&& value)
        requires (
            !kHasNiche &&
            constructible_from<T, U&&> &&
            !is_option<U>
        )
        : m_payload{in_place, std::forward<U>(value)}
        , m_has_value{true}
    {}

    constexpr option(const option& other) requires trivially_copy_constructible<T> = default;
    constexpr option(const option& other) requires (!copy_constructible<T>) = delete;

    constexpr option(const option& other)
        requires copy_constructible<T> && (!trivially_copy_constructible<T>)
        : option{nullopt}
    {
        if (other.has_value())
        {
            construct(other.m_payload.as_init_unsafe());
        }
    }

    constexpr option(option&& other) requires trivially_move_constructible<T> = default;
    constexpr option(option&& other) requires (!move_constructible<T>) = delete;

    constexpr option(option&& other)
        requires move_constructible<T> && (!trivially_move_constructible<T>)
        : option{nullopt}
    {
        if (other.has_value())
        {
            construct(std::move(other.m_payload.as_init_unsafe()));
        }
    }

    template<typename U>
    constexpr explicit (!convertible_to<const U&, T>)
    option(const option<U>& other)
        requires (
            constructible_from<T, const U&> &&
            option_internal::not_constructible_from_option<T, U>
        )
        : option{nullopt}
    {
        if (other.has_value())
        {
            construct(other.m_payload.as_init_unsafe());
        }
    }

    template<typename U>
    constexpr explicit (!convertible_to<U&&, T>)
    option(option<U>&& other)
        requires (
            constructible_from<T, U&&> &&
            option_internal::not_constructible_from_option<T, U>
        )
        : option{nullopt}
    {
        if (other.has_value())
        {
            construct(std::move(other).m_payload.as_init_unsafe());
        }
    }

    constexpr option& operator=(nullopt_t) &
    {
        reset();
        return *this;
    }

    template<typename U = T>
    constexpr option& operator=(U&& value) &
        requires (
            assignable_from<T&, U&&> &&
            constructible_from<T, U&&> &&
            !is_option<U>
        )
    {
        if (has_value())
        {
            assign(std::forward<U>(value));
        }
        else
        {
            construct(std::forward<U>(value));
        }

        return *this;
    }

    constexpr option& operator=(const option& other) &
        requires (!copy_assignable<T>) = delete;

    constexpr option& operator=(const option& other) &
        requires trivially_copy_assignable<T> = default;

    constexpr option& operator=(const option& other) &
        requires copy_assignable<T> && (!trivially_copy_constructible<T>)
    {
        if (&other == this) { return *this; }

        if (other.has_value())
        {
            if (has_value())
            {
                assign(other.m_payload.as_init_unsafe());
            }
            else
            {
                construct(other.m_payload.as_init_unsafe());
            }
        }
        else if (has_value())
        {
            reset();
        }

        return *this;
    }

    constexpr option& operator=(option&& other) &
        requires (!move_assignable<T>) = delete;

    constexpr option& operator=(option&& other) &
        requires trivially_move_assignable<T> = default;

    constexpr option& operator=(option&& other) &
        requires move_assignable<T> && (!trivially_move_constructible<T>)
    {
        if (&other == this) { return *this; }

        if (other.has_value())
        {
            if (has_value())
            {
                assign(std::move(other.m_payload.as_init_unsafe()));
            }
            else
            {
                construct(std::move(other.m_payload.as_init_unsafe()));
            }
        }
        else if (has_value())
        {
            reset();
        }

        return *this;
    }

    template<typename U = T>
    constexpr option& operator=(const option<U>& other) &
        requires (
            constructible_from<T, const U&> &&
            assignable_from<T&, const U&> &&
            option_internal::not_constructible_assignable_from_option<T, U>
        )
    {
        if (other.has_value())
        {
            if (has_value())
            {
                assign(other.m_payload.as_init_unsafe());
            }
            else
            {
                construct(other.m_payload.as_init_unsafe());
            }
        }
        else if (has_value())
        {
            reset();
        }

        return *this;
    }

    template<typename U = T>
    constexpr option& operator=(option<U>&& other) &
        requires (
            constructible_from<T, U&&> &&
            assignable_from<T&, U&&> &&
            option_internal::not_constructible_assignable_from_option<T, U>
        )
    {
        if (other.has_value())
        {
            if (has_value())
            {
                assign(std::move(other).m_payload.as_init_unsafe());
            }
            else
            {
                construct(std::move(other).m_payload.as_init_unsafe());
            }
        }
        else if (has_value())
        {
            reset();
        }

        return *this;
    }

    constexpr ~option() requires trivially_destructible<T> = default;
    constexpr ~option() requires (!trivially_destructible<T>)
    {
        reset();
    }

    constexpr void reset()
    {
        if (!has_value()) { return; }

        if constexpr (kHasNiche)
        {
            if constexpr (move_assignable<T>)
            {
                m_payload.assign_unsafe(std::move(T{niche_t{}}));
            }
            else
            {
                m_payload.destroy_unsafe();
                m_payload.construct_unsafe(niche_t{});
            }
        }
        else
        {
            m_has_value = false;
            m_payload.destroy_unsafe();
        }
    }

    [[nodiscard]] constexpr bool has_value() const
    {
        if constexpr (kHasNiche)
        {
            return m_payload.as_init_unsafe() != niche_t{};
        }
        else
        {
            return m_has_value;
        }
    }

    constexpr auto&& value(this auto&& self)
    {
        ASL_ASSERT_RELEASE(self.has_value());
        return std::forward<decltype(self)>(self).m_payload.as_init_unsafe();
    }

    template<typename U>
    constexpr T value_or(U&& other_value) const&
        requires copy_constructible<T> && convertible_to<U&&, T>
    {
        return has_value() ? value() : static_cast<T>(std::forward<U>(other_value));
    }

    template<typename U>
    constexpr T value_or(U&& other_value) &&
        requires move_constructible<T> && convertible_to<U&&, T>
    {
        return has_value() ? std::move(value()) : static_cast<T>(std::forward<U>(other_value));
    }

    constexpr T& emplace(auto&&... args) &
        requires constructible_from<T, decltype(args)...>
    {
        if (has_value()) { reset(); }
        construct(std::forward<decltype(args)>(args)...);
        return value();
    }

    template<typename F>
    constexpr auto and_then(this auto&& self, F&& f)
    {
        using Result = invoke_result_t<F, copy_cref_t<decltype(self), T>>;
        static_assert(is_option<Result>);

        if (self.has_value())
        {
            return invoke(std::forward<F>(f), std::forward<decltype(self)>(self).value());
        }
        return Result{ asl::nullopt };
    }

    template<typename F>
    constexpr auto transform(this auto&& self, F&& f)
    {
        using Result = invoke_result_t<F, copy_cref_t<decltype(self), T>>;
        if (self.has_value())
        {
            return option<un_cvref_t<Result>>{
                invoke(std::forward<F>(f), std::forward<decltype(self)>(self).value())
            };
        }
        return option<un_cvref_t<Result>>{ asl::nullopt };
    }

    template<typename F>
    constexpr option or_else(F&& f) const&
        requires same_as<un_cvref_t<invoke_result_t<F>>, option>
    {
        return has_value() ? *this : invoke(std::forward<F>(f));
    }

    template<typename F>
    constexpr option or_else(F&& f) &&
        requires same_as<un_cvref_t<invoke_result_t<F>>, option>
    {
        return has_value() ? std::move(*this) : invoke(std::forward<F>(f));
    }

    template<typename H>
    requires (!uniquely_represented<option>) && hashable<T>
    friend H AslHashValue(H h, const option& opt)
    {
        if (!opt.has_value())
        {
            return H::combine(std::move(h), 0);
        }
        return H::combine(std::move(h), 1, opt.value());
    }
};

template<typename T>
requires has_niche<T> && uniquely_represented<T>
struct is_uniquely_represented<option<T>> : true_type {};

template<typename T>
option(T) -> option<T>;

} // namespace asl
