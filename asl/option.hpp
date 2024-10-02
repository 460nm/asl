#pragma once

#include "asl/assert.hpp"
#include "asl/meta.hpp"
#include "asl/maybe_uninit.hpp"

namespace asl
{

struct nullopt_t {};
static constexpr nullopt_t nullopt{};

// @Todo(option) Niche
// @Todo(option) Reference
// @Todo(option) Function
// @Todo(option) Arrays

template<is_object T>
class option
{
    maybe_uninit<T> m_payload;
    bool            m_has_value = false;

    template<typename... Args>
    constexpr void construct(Args&&... args) &
    {
        ASL_ASSERT(!m_has_value);
        m_payload.init_unsafe(ASL_FWD(args)...);
        m_has_value = true;
    }

    template<typename Arg>
    constexpr void assign(Arg&& arg) &
    {
        ASL_ASSERT(m_has_value);
        m_payload.as_init_unsafe() = ASL_FWD(arg);
    }

public:
    constexpr option() = default;
    constexpr option(nullopt_t) {} // NOLINT(*-explicit-conversions)

    constexpr option(const option& other) requires copy_constructible<T>
    {
        if (other.m_has_value)
        {
            construct(other.m_payload.as_init_unsafe());
        }
    }

    constexpr option(option&& other) requires move_constructible<T>
    {
        if (other.m_has_value)
        {
            construct(ASL_MOVE(other.m_payload.as_init_unsafe()));
        }
    }

    constexpr option& operator=(const option& other) & requires copy_assignable<T> && copy_constructible<T>
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
        else if (m_has_value)
        {
            reset();
        }
        
        return *this;
    }

    constexpr option& operator=(option&& other) & requires move_assignable<T> && move_constructible<T>
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
        else if (m_has_value)
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
        if (m_has_value)
        {
            m_payload.uninit_unsafe();
            m_has_value = false;
        }
    }
    
};

} // namespace asl
