// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#include "asl/base/integers.hpp"
#include "asl/strings/string_view.hpp"
#include "asl/formatting/format.hpp"

namespace asl
{

class Formatter;

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
    string_view message_internal() const;

    void ref();
    void unref();

public:
    constexpr ~status()
    {
        if (!is_inline()) { unref(); }
    }

    explicit constexpr status(status_code code)
        : m_payload{status_to_payload(code)}
    {}

    status(status_code code, string_view msg);
    status(status_code code, string_view fmt, span<format_internals::type_erased_arg> args);

    constexpr status(const status& other)
        : m_payload{other.m_payload}
    {
        if (!is_inline()) { ref(); }
    }

    constexpr status(status&& other)
        : m_payload{exchange(other.m_payload, status_to_payload(other.code()))}
    {}

    constexpr status& operator=(const status& other)
    {
        if (&other != this)
        {
            if (!is_inline()) { unref(); }
            m_payload = other.m_payload;
            if (!is_inline()) { ref(); }
        }
        return *this;
    }

    constexpr status& operator=(status&& other)
    {
        if (&other != this)
        {
            if (!is_inline()) { unref(); }
            m_payload = exchange(other.m_payload, status_to_payload(other.code()));
        }
        return *this;
    }

    constexpr bool ok() const
    {
        return m_payload == nullptr;
    }

    constexpr status_code code() const
    {
        return is_inline() ? code_inline() : code_internal();
    }

    constexpr string_view message() const
    {
        if (!is_inline())
        {
            return message_internal();
        }
        return {};
    }

    constexpr status&& throw_status() && { return std::move(*this); }

    friend void AslFormat(Formatter& f, const status&);

    template<typename H>
    friend H AslHashValue(H h, const status& s)
    {
        if (s.is_inline())
        {
            return H::combine(std::move(h), s.code());
        }
        return H::combine(std::move(h), s.code(), s.message());
    }
};

static constexpr status ok() { return status{status_code::ok}; }

#define ASL_DEFINE_ERROR_(type) \
    static constexpr status type##_error() { return status{status_code::type}; }                \
    static inline status type##_error(string_view sv) { return status{status_code::type, sv}; } \
    template<formattable... Args>                                                               \
    [[maybe_unused]] static status type##_error(string_view fmt, const Args&... args)           \
    {                                                                                           \
        format_internals::type_erased_arg type_erased_args[] = {                                \
            format_internals::type_erased_arg(args)...                                          \
        };                                                                                      \
        return status{status_code::type, fmt, type_erased_args};                                \
    }

ASL_DEFINE_ERROR_(unknown)
ASL_DEFINE_ERROR_(internal)
ASL_DEFINE_ERROR_(runtime)
ASL_DEFINE_ERROR_(invalid_argument)

#define ASL_TRY(VALUE) if (VALUE.ok()) {} else { return std::move(VALUE).throw_status(); }

} // namespace asl
