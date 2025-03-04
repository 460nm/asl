// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#include "asl/base/integers.hpp"
#include "asl/base/meta.hpp"
#include "asl/types/span.hpp"
#include "asl/memory/memory.hpp"

namespace asl
{

class string_view
{
    const char* m_data{};
    isize_t     m_size{};

public:
    constexpr string_view() = default;

    constexpr string_view(nullptr_t) : string_view() {} // NOLINT(*-explicit-conversions)

    constexpr string_view(const char* data, isize_t size)
        : m_data{data}
        , m_size{size}
    {}

    template<isize_t kSize>
    constexpr string_view(const char (&str)[kSize]) // NOLINT(*-explicit-conversions)
        requires (kSize >= 1)
        : m_data{str}
        , m_size{kSize - 1}
    {
        ASL_ASSERT(m_data[kSize - 1] == '\0'); // NOLINT(*-pointer-arithmetic)
    }

    static constexpr string_view from_zstr(const char* str)
    {
        return string_view(str, asl::strlen(str));
    }

    constexpr string_view(const string_view&) = default;
    constexpr string_view(string_view&&) = default;

    constexpr string_view& operator=(const string_view&) = default;
    constexpr string_view& operator=(string_view&&) = default;

    ~string_view() = default;

    constexpr isize_t size() const { return m_size; }

    constexpr bool is_empty() const { return m_size == 0; }

    constexpr const char* data() const { return m_data; }

    constexpr contiguous_iterator<const char> begin() const { return contiguous_iterator{m_data}; }

    // NOLINTNEXTLINE(*-pointer-arithmetic)
    constexpr contiguous_iterator<const char> end() const { return contiguous_iterator{m_data + m_size}; }

    constexpr span<const char> as_span() const { return span<const char>(m_data, m_size); }

    constexpr char operator[](isize_t i) const
    {
        ASL_ASSERT(i >= 0 && i < m_size);
        return m_data[i]; // NOLINT(*-pointer-arithmetic)
    }

    constexpr string_view substr(isize_t offset, isize_t size) const
    {
        ASL_ASSERT(offset >= 0 && size >= 0 && offset + size <= m_size);
        return string_view{m_data + offset, size}; // NOLINT(*-pointer-arithmetic)
    }

    constexpr string_view substr(isize_t offset) const
    {
        ASL_ASSERT(offset >= 0 && offset <= m_size);
        return string_view{m_data + offset, m_size - offset}; // NOLINT(*-pointer-arithmetic)
    }

    constexpr string_view first(isize_t size) const
    {
        return substr(0, size);
    }

    constexpr string_view last(isize_t size) const
    {
        return substr(m_size - size);
    }

    constexpr bool operator==(string_view other) const
    {
        if (m_size != other.m_size) { return false; }
        return memcmp(m_data, other.m_data, m_size) == 0;
    }

    template<typename H>
    friend H AslHashValue(H h, string_view sv)
    {
        return H::combine(H::combine_contiguous(h, as_bytes(sv.as_span())), sv.size());
    }
};

} // namespace asl

constexpr asl::string_view operator ""_sv(const char* s, size_t len)
{
    return asl::string_view(s, static_cast<isize_t>(len));
}
