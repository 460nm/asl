// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#include "asl/containers/buffer.hpp"
#include "asl/strings/string_view.hpp"

namespace asl
{

template<allocator Allocator = DefaultAllocator>
class string
{
    buffer<char, Allocator> m_buffer;

    explicit constexpr string(buffer<char, Allocator>&& buffer) :
        m_buffer{std::move(buffer)}
    {}

    template<allocator A>
    friend class StringBuilder;

public:
    constexpr string() requires default_constructible<Allocator> = default;
    explicit constexpr string(Allocator allocator) : m_buffer{std::move(allocator)} {}

    // NOLINTNEXTLINE(*explicit*)
    constexpr string(string_view sv)
        requires default_constructible<Allocator>
        : m_buffer{sv.as_span()}
    {}

    constexpr string(string_view sv, Allocator allocator)
        : m_buffer{sv.as_span(), std::move(allocator)}
    {}

    constexpr ~string() = default;

    constexpr string(const string&) requires copy_constructible<Allocator> = default;
    constexpr string(string&&) = default;

    constexpr string& operator=(const string&) requires copy_assignable<Allocator> = default;
    constexpr string& operator=(string&&) = default;

    [[nodiscard]] constexpr isize_t size() const { return m_buffer.size(); }

    [[nodiscard]] constexpr bool is_empty() const { return m_buffer.is_empty(); }

    [[nodiscard]] constexpr const char* data() const { return m_buffer.data(); }

    // NOLINTNEXTLINE(*explicit*)
    constexpr operator string_view() const
    {
        return as_string_view();
    }

    [[nodiscard]] constexpr string_view as_string_view() const
    {
        auto span = m_buffer.as_span();
        return string_view{span.data(), span.size()};
    }

    constexpr bool operator==(const string& other) const
    {
        return as_string_view() == other.as_string_view();
    }

    constexpr bool operator==(string_view other) const
    {
        return as_string_view() == other;
    }

    template<typename H>
    friend H AslHashValue(H h, const string& str)
    {
        return H::combine(h, str.as_string_view());
    }
};

string() -> string<>;

} // namespace asl
