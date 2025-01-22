#pragma once

#include "asl/buffer.hpp"
#include "asl/string.hpp"
#include "asl/string_view.hpp"

namespace asl
{

template<allocator Allocator = DefaultAllocator>
class string_builder
{
    buffer<char, Allocator> m_buffer;

public:
    constexpr string_builder() requires default_constructible<Allocator> = default;
    explicit constexpr string_builder(Allocator allocator) : m_buffer{ASL_MOVE(allocator)} {}

    constexpr ~string_builder() = default;

    constexpr string_builder(const string_builder&) requires copy_constructible<Allocator> = default;
    constexpr string_builder(string_builder&&) = default;

    constexpr string_builder& operator=(const string_builder&) requires copy_assignable<Allocator> = default;
    constexpr string_builder& operator=(string_builder&&) = default;

    constexpr string_view as_string_view() const
    {
        auto span = m_buffer.as_span();
        return string_view{span.data(), span.size()};
    }

    void reset()
    {
        m_buffer.clear();
    }

    // @Todo(C++23) Deducing this

    string_builder& push(string_view sv) &
    {
        isize_t old_size = m_buffer.size();
        m_buffer.resize_zero(old_size + sv.size());
        asl::memcpy(m_buffer.data() + old_size, sv.data(), sv.size());
        return *this;
    }

    string_builder&& push(string_view sv) &&
    {
        isize_t old_size = m_buffer.size();
        m_buffer.resize_zero(old_size + sv.size());
        asl::memcpy(m_buffer.data() + old_size, sv.data(), sv.size());
        return ASL_MOVE(*this);
    }

    string_builder& push(char c) &
    {
        m_buffer.push(c);
        return *this;
    }

    string_builder&& push(char c) &&
    {
        m_buffer.push(c);
        return ASL_MOVE(*this);
    }

    string<Allocator> finish() &&
    {
        return string<Allocator>{ASL_MOVE(m_buffer)};
    }

    template<allocator StringAllocator = Allocator>
    string<StringAllocator> as_string()
        requires default_constructible<StringAllocator>
    {
        return string<StringAllocator>{as_string_view()};
    }

    template<allocator StringAllocator = Allocator>
    string<StringAllocator> as_string(Allocator allocator)
    {
        return string<StringAllocator>{as_string_view(), ASL_MOVE(allocator)};
    }
};

string_builder() -> string_builder<>;

} // namespace asl
