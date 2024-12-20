#pragma once

#include "asl/meta.hpp"
#include "asl/maybe_uninit.hpp"

namespace asl
{

struct nullopt_t {};
static constexpr nullopt_t nullopt{};

template<is_object T>
class option
{
    maybe_uninit<T> m_payload;
    bool            m_has_value = false;

public:
    constexpr option() = default;
    constexpr option(nullopt_t) {} // NOLINT(*-explicit-conversions)

    constexpr option(const option& other) requires copy_constructible<T>
    {
        if (other.m_has_value)
        {
            m_payload.init_unsafe(other.m_payload.as_init_unsafe());
            m_has_value = true;
        }
    }

    constexpr option(option&& other) requires move_constructible<T>
    {
        if (other.m_has_value)
        {
            m_payload.init_unsafe(ASL_MOVE(other.m_payload.as_init_unsafe()));
            m_has_value = true;
        }
    }

    constexpr option& operator=(const option& other) & requires copy_assignable<T> && copy_constructible<T>
    {
        if (&other == this) { return *this; }

        if (other.m_has_value)
        {
            if (m_has_value)
            {
                m_payload.as_init_unsafe() = other.m_payload.as_init_unsafe();
            }
            else
            {
                m_payload.init_unsafe(other.m_payload.as_init_unsafe());
                m_has_value = true;
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
                m_payload.as_init_unsafe() = ASL_MOVE(other.m_payload.as_init_unsafe());
            }
            else
            {
                m_payload.init_unsafe(ASL_MOVE(other.m_payload.as_init_unsafe()));
                m_has_value = true;
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
