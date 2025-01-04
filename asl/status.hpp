#pragma once

#include "asl/integers.hpp"
#include "asl/string_view.hpp"

namespace asl
{

// @Todo Make status with formatting

enum class status_code : uint8_t
{
    ok               = 0,
    unknown          = 1,
    internal         = 2,
    runtime          = 3,
    invalid_argument = 4,
};

class status
{
    void* m_payload{};

    static constexpr void* status_to_payload(status_code code)
    {
        return code == status_code::ok
            ? nullptr
            : bit_cast<void*>((static_cast<uintptr_t>(code) << 1) | 1);
    }

    static constexpr status_code payload_to_status(void* payload)
    {
        return static_cast<status_code>(bit_cast<uintptr_t>(payload) >> 1);
    }

    constexpr bool is_inline() const
    {
        return m_payload == nullptr || (bit_cast<uintptr_t>(m_payload) & 1) != 0;
    }

    constexpr status_code code_inline() const
    {
        ASL_ASSERT(is_inline());
        if (m_payload == nullptr)
        {
            return status_code::ok;
        }
        return payload_to_status(m_payload);
    }

    status_code code_internal() const;

    void unref();
    
public:
    constexpr status() = default;

    constexpr ~status()
    {
        if (!is_inline())
        {
            unref();
        }
    }
    
    explicit constexpr status(status_code code)
        : m_payload{status_to_payload(code)}
    {}

    status(status_code code, string_view msg);

    constexpr status(status&& other)
        : m_payload{exchange(other.m_payload, nullptr)}
    {}

    constexpr status& operator=(status&& other)
    {
        if (&other != this)
        {
            swap(m_payload, other.m_payload);
        }
        return *this;
    }

    // @Todo Copy constructor & assignment

    constexpr bool ok() const
    {
        return m_payload == nullptr || code() == status_code::ok;
    }

    // NOLINTNEXTLINE(*-explicit-conversions)
    constexpr operator bool() const { return ok(); }

    constexpr status_code code() const
    {
        return is_inline() ? code_inline() : code_internal();
    }
};

} // namespace asl
