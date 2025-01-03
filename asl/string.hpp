#pragma once

#include "asl/buffer.hpp"
#include "asl/string_view.hpp"

namespace asl
{

template<allocator Allocator = DefaultAllocator>
class string
{
    buffer<char, Allocator> m_buffer;

public:
    constexpr string() requires default_constructible<Allocator> = default;
    explicit constexpr string(Allocator allocator) : m_buffer{ASL_MOVE(allocator)} {}

    // NOLINTNEXTLINE(*-explicit-conversions)
    constexpr string(string_view sv)
        requires default_constructible<Allocator>
        : m_buffer{sv.as_span()}
    {}

    constexpr string(string_view sv, Allocator allocator)
        : m_buffer{sv.as_span(), ASL_MOVE(allocator)}
    {}

    constexpr ~string() = default;

    constexpr string(const string&) requires copy_constructible<Allocator> = default;
    constexpr string(string&&) = default;

    constexpr string& operator=(const string&) requires copy_assignable<Allocator> = default;
    constexpr string& operator=(string&&) = default;

    constexpr isize_t size() const { return m_buffer.size(); }
    constexpr const char* data() const { return m_buffer.data(); }

    // NOLINTNEXTLINE(*-explicit-conversions)
    constexpr operator string_view() const
    {
        return as_string_view();
    }

    constexpr string_view as_string_view() const
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
};

} // namespace asl
