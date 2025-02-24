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
            m_payload.construct_unsafe(ASL_FWD(args)...);
            m_has_value = true;
        }
        else
        {
            if constexpr (move_assignable<T>)
            {
                m_payload.assign_unsafe(ASL_MOVE(T{ASL_FWD(args)...}));
            }
            else
            {
                m_payload.destroy_unsafe();
                m_payload.construct_unsafe(ASL_FWD(args)...);
            }
        }
    }

    template<typename U>
    constexpr void assign(U&& arg)
    {
        ASL_ASSERT(has_value());
        m_payload.assign_unsafe(ASL_FWD(arg));
    }

public:
    using type = T;

    constexpr option() : option{nullopt} {}

     // NOLINTNEXTLINE(*-explicit-conversions)
    constexpr option(nullopt_t) requires (!kHasNiche) {}

     // NOLINTNEXTLINE(*-explicit-conversions)
    constexpr option(nullopt_t) requires kHasNiche : m_payload{in_place, niche_t{}} {}

    template<typename U = T>
    constexpr explicit (!convertible_from<T, U&&>)
    option(U&& value)
        requires (
            kHasNiche &&
            constructible_from<T, U&&> &&
            !same_as<un_cvref_t<U>, option>
        )
        : m_payload{in_place, ASL_FWD(value)}
    {}

    template<typename U = T>
    constexpr explicit (!convertible_from<T, U&&>)
    option(U&& value)
        requires (
            !kHasNiche &&
            constructible_from<T, U&&> &&
            !is_option<U>
        )
        : m_payload{in_place, ASL_FWD(value)}
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
            construct(ASL_MOVE(other.m_payload.as_init_unsafe()));
        }
    }

    template<typename U>
    constexpr explicit (!convertible_from<T, const U&>)
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
    constexpr explicit (!convertible_from<T, U&&>)
    option(option<U>&& other)
        requires (
            constructible_from<T, U&&> &&
            option_internal::not_constructible_from_option<T, U>
        )
        : option{nullopt}
    {
        if (other.has_value())
        {
            construct(ASL_MOVE(other.m_payload.as_init_unsafe()));
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
            assign(ASL_FWD(value));
        }
        else
        {
            construct(ASL_FWD(value));
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
                assign(ASL_MOVE(other.m_payload.as_init_unsafe()));
            }
            else
            {
                construct(ASL_MOVE(other.m_payload.as_init_unsafe()));
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
                assign(ASL_MOVE(other.m_payload.as_init_unsafe()));
            }
            else
            {
                construct(ASL_MOVE(other.m_payload.as_init_unsafe()));
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
                m_payload.assign_unsafe(ASL_MOVE(T{niche_t{}}));
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

    constexpr bool has_value() const
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

    template<typename Self>
    constexpr auto&& value(this Self&& self)
    {
        ASL_ASSERT_RELEASE(self.has_value());
        return ASL_FWD(self).m_payload.as_init_unsafe();
    }

    template<typename U>
    constexpr T value_or(U&& other_value) const&
        requires copy_constructible<T> && convertible_from<T, U&&>
    {
        return has_value() ? value() : static_cast<T>(ASL_FWD(other_value));
    }

    template<typename U>
    constexpr T value_or(U&& other_value) &&
        requires move_constructible<T> && convertible_from<T, U&&>
    {
        return has_value() ? ASL_MOVE(value()) : static_cast<T>(ASL_FWD(other_value));
    }

    template<typename... Args>
    constexpr T& emplace(Args&&... args) &
        requires constructible_from<T, Args&&...>
    {
        if (has_value()) { reset(); }
        construct(ASL_FWD(args)...);
        return value();
    }

    template<typename F>
    constexpr auto and_then(F&& f) &
        requires is_option<result_of_t<F(T&)>>
    {
        if (has_value())
        {
            return invoke(ASL_FWD(f), value());
        }
        return un_cvref_t<result_of_t<F(T&)>>{};
    }

    template<typename F>
    constexpr auto and_then(F&& f) const&
        requires is_option<result_of_t<F(const T&)>>
    {
        if (has_value())
        {
            return invoke(ASL_FWD(f), value());
        }
        return un_cvref_t<result_of_t<F(const T&)>>{};
    }

    template<typename F>
    constexpr auto and_then(F&& f) &&
        requires is_option<result_of_t<F(T)>>
    {
        if (has_value())
        {
            return invoke(ASL_FWD(f), ASL_MOVE(value()));
        }
        return un_cvref_t<result_of_t<F(T)>>{};
    }

    template<typename F>
    constexpr auto transform(F&& f) &
    {
        using U = un_cvref_t<result_of_t<F(T&)>>;
        if (has_value())
        {
            return option<U>{ invoke(ASL_FWD(f), value()) };
        }
        return option<U>{};
    }

    template<typename F>
    constexpr auto transform(F&& f) const&
    {
        using U = un_cvref_t<result_of_t<F(const T&)>>;
        if (has_value())
        {
            return option<U>{ invoke(ASL_FWD(f), value()) };
        }
        return option<U>{};
    }

    template<typename F>
    constexpr auto transform(F&& f) &&
    {
        using U = un_cvref_t<result_of_t<F(T)>>;
        if (has_value())
        {
            return option<U>{ invoke(ASL_FWD(f), ASL_MOVE(value())) };
        }
        return option<U>{};
    }

    template<typename F>
    constexpr option or_else(F&& f) const&
        requires same_as<un_cvref_t<result_of_t<F()>>, option>
    {
        return has_value() ? *this : invoke(ASL_FWD(f));
    }

    template<typename F>
    constexpr option or_else(F&& f) &&
        requires same_as<un_cvref_t<result_of_t<F()>>, option>
    {
        return has_value() ? ASL_MOVE(*this) : invoke(ASL_FWD(f));
    }

    template<typename H>
    requires (!uniquely_represented<option>) && hashable<T>
    friend H AslHashValue(H h, const option& opt)
    {
        if (!opt.has_value())
        {
            return H::combine(ASL_MOVE(h), 0);
        }
        return H::combine(ASL_MOVE(h), 1, opt.value());
    }
};

template<typename T>
requires has_niche<T> && uniquely_represented<T>
struct is_uniquely_represented<option<T>> : true_type {};

template<typename T>
option(T) -> option<T>;

} // namespace asl
