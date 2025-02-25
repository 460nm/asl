#pragma once

#include "asl/types/status.hpp"
#include "asl/types/maybe_uninit.hpp"
#include "asl/hashing/hash.hpp"

namespace asl
{

template<is_object T>
class status_or
{
    status          m_status;
    maybe_uninit<T> m_value{};

public:
    // @Todo Convert copy
    // @Todo Convert move

    constexpr status_or(const status_or& other)
        requires copy_constructible<T>
        : m_status{other.m_status}
    {
        if (other.ok())
        {
            m_value.construct_unsafe(other.m_value.as_init_unsafe());
        }
    }

    constexpr status_or(status_or&& other)
        requires move_constructible<T>
        : m_status{ASL_MOVE(other.m_status)}
    {
        if (other.ok())
        {
            m_value.construct_unsafe(ASL_MOVE(other.m_value.as_init_unsafe()));
        }
    }

    constexpr status_or& operator=(const status_or& other)
        requires copyable<T>
    {
        if (&other != this)
        {
            if (ok())
            {
                if (other.ok())
                {
                    m_value.assign_unsafe(other.m_value.as_init_unsafe());
                }
                else
                {
                    m_value.destroy_unsafe();
                }
            }
            else if (other.ok())
            {
                m_value.construct_unsafe(other.m_value.as_init_unsafe());
            }
            m_status = other.m_status;
        }
        return *this;
    }

    constexpr status_or& operator=(status_or&& other)
        requires moveable<T>
    {
        if (&other != this)
        {
            if (ok())
            {
                if (other.ok())
                {
                    m_value.assign_unsafe(ASL_MOVE(other.m_value.as_init_unsafe()));
                }
                else
                {
                    m_value.destroy_unsafe();
                }
            }
            else if (other.ok())
            {
                m_value.construct_unsafe(ASL_MOVE(other.m_value.as_init_unsafe()));
            }
            m_status = ASL_MOVE(other.m_status);
        }
        return *this;
    }

    constexpr ~status_or()
    {
        if (m_status.ok())
        {
            m_value.destroy_unsafe();
        }
    }

    // NOLINTNEXTLINE(*-explicit-conversions)
    constexpr status_or(const status& status) : m_status{status}
    {
        ASL_ASSERT_RELEASE(!m_status.ok());
    }

    // NOLINTNEXTLINE(*-explicit-conversions)
    constexpr status_or(status&& status) : m_status{ASL_MOVE(status)}
    {
        ASL_ASSERT_RELEASE(!m_status.ok());
    }

    status_or& operator=(status status) = delete;

    template<typename U = T>
    constexpr explicit (!convertible_from<T, U&&>)
    status_or(U&& value)
        requires (
            constructible_from<T, U&&> &&
            !same_as<un_cvref_t<U>, status_or> &&
            !same_as<un_cvref_t<U>, status>
        )
        : m_status{status_code::ok}
        , m_value{in_place, ASL_FWD(value)}
    {}

    constexpr bool ok() const { return m_status.ok(); }

    constexpr status_code code() const { return m_status.code(); }

    constexpr string_view message() const { return m_status.message(); }

    constexpr status&& throw_status() && { return ASL_MOVE(m_status); }

    template<typename Self>
    constexpr auto&& value(this Self&& self)
    {
        ASL_ASSERT_RELEASE(self.ok());
        return ASL_FWD(self).m_value.as_init_unsafe();
    }

    template<typename U>
    constexpr T value_or(U&& other_value) const&
        requires copy_constructible<T> && convertible_from<T, U&&>
    {
        return ok() ? value() : static_cast<T>(ASL_FWD(other_value));
    }

    template<typename U>
    constexpr T value_or(U&& other_value) &&
        requires move_constructible<T> && convertible_from<T, U&&>
    {
        return ok() ? ASL_MOVE(value()) : static_cast<T>(ASL_FWD(other_value));
    }

    friend void AslFormat(Formatter& f, const status_or& status)
    {
        AslFormat(f, status.m_status);
    }

    template<typename H>
    requires hashable<T>
    friend H AslHashValue(H h, const status_or& s)
    {
        if (s.ok())
        {
            return H::combine(ASL_MOVE(h), s.m_status, s.value());
        }
        return H::combine(ASL_MOVE(h), s.m_status);
    }
};

template<typename T>
status_or(T) -> status_or<T>;

} // namespace asl

