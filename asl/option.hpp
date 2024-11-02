#pragma once

#include "asl/assert.hpp"
#include "asl/meta.hpp"
#include "asl/maybe_uninit.hpp"
#include "asl/functional.hpp"

namespace asl
{

struct nullopt_t {};
static constexpr nullopt_t nullopt{};

// @Todo(option) Niche
// @Todo(option) Reference
// @Todo(option) Function
// @Todo(option) Arrays

template<is_object T> class option;

namespace option_internal
{

template<typename T, typename U>
concept convertible_from_option =
    convertible<option<U>&, T> &&
    convertible<const option<U>&, T> &&
    convertible<option<U>&&, T> &&
    convertible<const option<U>&&, T>;

template<typename T, typename U>
concept constructible_from_option = 
    constructible<T, option<U>&> &&
    constructible<T, const option<U>&> &&
    constructible<T, option<U>&&> &&
    constructible<T, const option<U>&&>;

template<typename T, typename U>
concept assignable_from_option = 
    assignable<T&, option<U>&> &&
    assignable<T&, const option<U>&> &&
    assignable<T&, option<U>&&> &&
    assignable<T&, const option<U>&&>;

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
    requires is_same<un_cvref_t<T>, option<typename T::type>>;
};

template<is_object T>
class option
{
    static constexpr bool kIsTrivial =
        trivially_default_constructible<T> &&
        trivially_copy_constructible<T> &&
        trivially_move_constructible<T> &&
        trivially_copy_assignable<T> &&
        trivially_move_assignable<T> &&
        trivially_destructible<T>;

    using Storage = select_t<kIsTrivial, T, maybe_uninit<T>>;

    Storage m_payload{};
    bool    m_has_value = false;

    template<typename... Args>
    constexpr void construct(Args&&... args) &
    {
        ASL_ASSERT(!m_has_value);
        m_has_value = true;
        if constexpr (kIsTrivial)
        {
            new(&m_payload) T(ASL_FWD(args)...);
        }
        else
        {
            m_payload.init_unsafe(ASL_FWD(args)...);
        }
    }

    template<typename Arg>
    constexpr void assign(Arg&& arg) &
    {
        ASL_ASSERT(m_has_value);
        if constexpr (kIsTrivial)
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

    constexpr option() = default;
    constexpr option(nullopt_t) {} // NOLINT(*-explicit-conversions)

    template<typename U = T>
    constexpr explicit (!convertible<U&&, T>)
    option(U&& value)
        requires (
            constructible<T, U> &&
            !is_same<un_cvref_t<U>, option>
        )
    {
        construct(ASL_FWD(value));
    }
    
    constexpr option(const option& other)
        requires copy_constructible<T> && kIsTrivial = default;

    constexpr option(const option& other)
        requires copy_constructible<T> && (!kIsTrivial)
    {
        if (other.m_has_value)
        {
            construct(other.m_payload.as_init_unsafe());
        }
    }

    constexpr option(const option& other)
        requires (!copy_constructible<T>) = delete;
        
    constexpr option(option&& other)
        requires move_constructible<T> && kIsTrivial = default;

    constexpr option(option&& other)
        requires move_constructible<T> && (!kIsTrivial)
    {
        if (other.m_has_value)
        {
            construct(ASL_MOVE(other.m_payload.as_init_unsafe()));
        }
    }

    template<typename U>
    constexpr explicit (!convertible<const U&, T>)
    option(const option<U>& other)
        requires (
            constructible<T, const U&> && 
            !option_internal::convertible_constructible_from_option<T, U>
        )
    {
        if (other.has_value())
        {
            construct(other.value());
        }
    }

    template<typename U>
    constexpr explicit (!convertible<U&&, T>)
    option(option<U>&& other)
        requires (
            constructible<T, U&&> &&
            !option_internal::convertible_constructible_from_option<T, U>
        )
    {
        if (other.has_value())
        {
            construct(ASL_MOVE(other).value());
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
            assignable<T&, U> &&
            constructible<T, U> && 
            !is_same<un_cvref_t<U>, option>
        )
    {
        if (m_has_value)
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
        requires (!copy_assignable<T> || copy_constructible<T>) = delete;
        
    constexpr option& operator=(const option& other) &
        requires copy_assignable<T> && copy_constructible<T> && kIsTrivial = default;

    constexpr option& operator=(const option& other) &
        requires copy_assignable<T> && copy_constructible<T> && (!kIsTrivial)
    {
        if (&other == this) { return *this; }

        if (other.m_has_value)
        {
            if (m_has_value)
            {
                assign(other.m_payload.as_init_unsafe());
            }
            else
            {
                construct(other.m_payload.as_init_unsafe());
            }
        }
        else
        {
            reset();
        }
        
        return *this;
    }

    constexpr option& operator=(option&& other) &
        requires move_assignable<T> && move_constructible<T> && kIsTrivial = default;
        
    constexpr option& operator=(option&& other) &
        requires move_assignable<T> && move_constructible<T> && (!kIsTrivial)
    {
        if (&other == this) { return *this; }

        if (other.m_has_value)
        {
            if (m_has_value)
            {
                assign(ASL_MOVE(other.m_payload.as_init_unsafe()));
            }
            else
            {
                construct(ASL_MOVE(other.m_payload.as_init_unsafe()));
            }
        }
        else
        {
            reset();
        }
        
        return *this;
    }

    template<typename U = T>
    constexpr option& operator=(const option<U>& other) &
        requires (
            constructible<T, const U&> &&
            assignable<T&, const U&> &&
            !option_internal::convertible_constructible_assignable_from_option<T, U>
        )
    {
        if (other.has_value())
        {
            if (m_has_value)
            {
                assign(other.value());
            }
            else
            {
                construct(other.value());
            }
        }
        else
        {
            reset();
        }

        return *this;
    }
    
    template<typename U = T>
    constexpr option& operator=(option<U>&& other) &
        requires (
            constructible<T, U> &&
            assignable<T&, U> &&
            !option_internal::convertible_constructible_assignable_from_option<T, U>
        )
    {
        if (other.has_value())
        {
            if (m_has_value)
            {
                assign(ASL_MOVE(other).value());
            }
            else
            {
                construct(ASL_MOVE(other).value());
            }
        }
        else
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
        if constexpr (kIsTrivial)
        {
            m_has_value = false;
        }
        else if (m_has_value)
        {
            m_payload.uninit_unsafe();
            m_has_value = false;
        }
    }

    constexpr bool has_value() const { return m_has_value; }

    constexpr T&& value() &&
    {
        ASL_ASSERT(m_has_value); // @Todo Release assert
        if constexpr (kIsTrivial)
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
        ASL_ASSERT(m_has_value); // @Todo Release assert
        if constexpr (kIsTrivial)
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
        ASL_ASSERT(m_has_value); // @Todo Release assert
        if constexpr (kIsTrivial)
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
        requires copy_constructible<T> && convertible<U&&, T>
    {
        return has_value() ? value() : static_cast<T>(ASL_FWD(other_value));
    }

    template<typename U>
    constexpr T value_or(U&& other_value) &&
        requires move_constructible<T> && convertible<U&&, T>
    {
        return has_value() ? ASL_MOVE(value()) : static_cast<T>(ASL_FWD(other_value));
    }

    template<typename... Args>
    constexpr T& emplace(Args&&... args) &
        requires constructible<T, Args&&...>
    {
        if (m_has_value) { reset(); }
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
        requires is_same<un_cvref_t<result_of_t<F()>>, option>
    {
        return has_value() ? *this : invoke(ASL_FWD(f));
    }

    template<typename F>
    constexpr option or_else(F&& f) &&
        requires is_same<un_cvref_t<result_of_t<F()>>, option>
    {
        return has_value() ? ASL_MOVE(*this) : invoke(ASL_FWD(f));
    }
};

template<typename T>
option(T) -> option<T>;

} // namespace asl
