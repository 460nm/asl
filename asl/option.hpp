#pragma once

#include "asl/assert.hpp"
#include "asl/meta.hpp"
#include "asl/maybe_uninit.hpp"
#include "asl/functional.hpp"
#include "asl/annotations.hpp"

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
concept convertible_from_option =
    convertible_from<T, option<U>&> &&
    convertible_from<T, const option<U>&> &&
    convertible_from<T, option<U>&&> &&
    convertible_from<T, const option<U>&&>;

template<typename T, typename U>
concept constructible_from_option = 
    constructible_from<T, option<U>&> &&
    constructible_from<T, const option<U>&> &&
    constructible_from<T, option<U>&&> &&
    constructible_from<T, const option<U>&&>;

template<typename T, typename U>
concept assignable_from_option = 
    assignable_from<T&, option<U>&> &&
    assignable_from<T&, const option<U>&> &&
    assignable_from<T&, option<U>&&> &&
    assignable_from<T&, const option<U>&&>;

template<typename T, typename U>
concept convertible_constructible_from_option = 
    convertible_from_option<T, U> && constructible_from_option<T, U>;


template<typename T, typename U>
concept convertible_constructible_assignable_from_option = 
    convertible_constructible_from_option<T, U> && assignable_from_option<T, U>;

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

    static constexpr bool kHasInlinePayload = default_constructible<T> || kHasNiche;

    using Storage = select_t<kHasInlinePayload, T, maybe_uninit<T>>;
    using HasValueMarker = select_t<kHasNiche, empty, bool>;

    Storage m_payload;
    ASL_NO_UNIQUE_ADDRESS HasValueMarker m_has_value{};

    template<typename... Args>
    constexpr void construct(Args&&... args)
    {
        ASL_ASSERT(!has_value());

        if constexpr (kHasInlinePayload)
        {
            m_payload = T(ASL_FWD(args)...);
        }
        else
        {
            m_payload.init_unsafe(ASL_FWD(args)...);
        }

        if constexpr (!kHasNiche)
        {
            m_has_value = true;
        }
    }

    template<typename U>
    constexpr void assign(U&& arg)
    {
        ASL_ASSERT(has_value());

        if constexpr (kHasInlinePayload)
        {
            m_payload = ASL_FWD(arg);
        }
        else
        {
            m_payload.as_init_unsafe() = ASL_FWD(arg);
        }
    }

public:
    using type = T;

    constexpr option() : option(nullopt) {}
    
     // NOLINTNEXTLINE(*-explicit-conversions)
    constexpr option(nullopt_t) requires (!kHasNiche) && trivially_default_constructible<T> {}
    
     // NOLINTNEXTLINE(*-explicit-conversions)
    constexpr option(nullopt_t) requires (!kHasNiche) && (!trivially_default_constructible<T>)
        : m_payload{}
    {}
    
     // NOLINTNEXTLINE(*-explicit-conversions)
    constexpr option(nullopt_t) requires kHasNiche
        : m_payload{niche{}}
    {}

    template<typename U = T>
    constexpr explicit (!convertible_from<T, U&&>)
    option(U&& value)
        requires (
            kHasNiche &&
            constructible_from<T, U> &&
            !same_as<un_cvref_t<U>, option> &&
            !is_niche<U>
        )
        : m_payload(ASL_FWD(value))
    {}

    template<typename U = T>
    constexpr explicit (!convertible_from<T, U&&>)
    option(U&& value)
        requires (
            kHasInlinePayload &&
            !kHasNiche &&
            constructible_from<T, U> &&
            !same_as<un_cvref_t<U>, option> &&
            !is_niche<U>
        )
        : m_payload(ASL_FWD(value))
        , m_has_value{true}
    {}

    template<typename U = T>
    constexpr explicit (!convertible_from<T, U&&>)
    option(U&& value)
        requires (
            !kHasInlinePayload &&
            constructible_from<T, U> &&
            !same_as<un_cvref_t<U>, option> &&
            !is_niche<U>
        )
        : option(nullopt)
    {
        construct(ASL_FWD(value));
    }
    
    constexpr option(const option& other)
        requires copy_constructible<T> && kHasInlinePayload = default;

    constexpr option(const option& other)
        requires copy_constructible<T> && (!kHasInlinePayload)
        : option(nullopt)
    {
        if (other.has_value())
        {
            construct(other.value());
        }
    }

    constexpr option(const option& other)
        requires (!copy_constructible<T>) = delete;
        
    constexpr option(option&& other)
        requires move_constructible<T> && kHasInlinePayload = default;

    constexpr option(option&& other)
        requires move_constructible<T> && (!kHasInlinePayload)
        : option(nullopt)
    {
        if (other.has_value())
        {
            construct(ASL_MOVE(other.value()));
        }
    }

    constexpr option(option&& other)
        requires (!move_constructible<T>) = delete;

    template<typename U>
    constexpr explicit (!convertible_from<T, const U&>)
    option(const option<U>& other)
        requires (
            constructible_from<T, const U&> && 
            !option_internal::convertible_constructible_from_option<T, U>
        )
        : option(nullopt)
    {
        if (other.has_value())
        {
            construct(other.value());
        }
    }

    template<typename U>
    constexpr explicit (!convertible_from<T, U&&>)
    option(option<U>&& other)
        requires (
            constructible_from<T, U&&> &&
            !option_internal::convertible_constructible_from_option<T, U>
        )
        : option(nullopt)
    {
        if (other.has_value())
        {
            construct(ASL_MOVE(other.value()));
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
            assignable_from<T&, U> &&
            constructible_from<T, U> && 
            !same_as<un_cvref_t<U>, option>
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
        requires (!copy_assignable<T> || !copy_constructible<T>) = delete;
        
    constexpr option& operator=(const option& other) &
        requires copy_assignable<T> && copy_constructible<T> && kHasInlinePayload = default;

    constexpr option& operator=(const option& other) &
        requires copy_assignable<T> && copy_constructible<T> && (!kHasInlinePayload)
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
        requires move_assignable<T> && move_constructible<T> && kHasInlinePayload = default;
        
    constexpr option& operator=(option&& other) &
        requires move_assignable<T> && move_constructible<T> && (!kHasInlinePayload)
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
            !option_internal::convertible_constructible_assignable_from_option<T, U>
        )
    {
        if (other.has_value())
        {
            if (has_value())
            {
                assign(other.value());
            }
            else
            {
                construct(other.value());
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
            constructible_from<T, U> &&
            assignable_from<T&, U> &&
            !option_internal::convertible_constructible_assignable_from_option<T, U>
        )
    {
        if (other.has_value())
        {
            if (has_value())
            {
                assign(ASL_MOVE(other).value());
            }
            else
            {
                construct(ASL_MOVE(other).value());
            }
        }
        else if (has_value())
        {
            reset();
        }

        return *this;
    }
    
    constexpr ~option() = default;
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
                m_payload = T(niche{});
            }
            else
            {
                destruct(&m_payload);
                construct_at<T>(&m_payload, niche{});
            }
        }
        else
        {
            m_has_value = false;
            if constexpr (!kHasInlinePayload)
            {
                m_payload.uninit_unsafe();
            }
        }
    }

    constexpr bool has_value() const
    {
        if constexpr (kHasNiche)
        {
            return m_payload != niche{};
        }
        else
        {
            return m_has_value;
        }
    }

    // @Todo(C++23) Deducing this
    constexpr T&& value() &&
    {
        ASL_ASSERT_RELEASE(has_value());
        if constexpr (kHasInlinePayload)
        {
            return ASL_MOVE(m_payload);
        }
        else
        {
            return ASL_MOVE(m_payload).as_init_unsafe();
        }
    }

    constexpr T& value() &
    {
        ASL_ASSERT_RELEASE(has_value());
        if constexpr (kHasInlinePayload)
        {
            return m_payload;
        }
        else
        {
            return m_payload.as_init_unsafe();
        }
    }

    constexpr const T& value() const&
    {
        ASL_ASSERT_RELEASE(has_value());
        if constexpr (kHasInlinePayload)
        {
            return m_payload;
        }
        else
        {
            return m_payload.as_init_unsafe();
        }
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
};

template<typename T>
option(T) -> option<T>;

} // namespace asl
