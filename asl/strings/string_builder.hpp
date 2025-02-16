#pragma once

#include "asl/containers/buffer.hpp"
#include "asl/strings/string.hpp"
#include "asl/strings/string_view.hpp"
#include "asl/formatting/format.hpp"
#include "asl/io/writer.hpp"

namespace asl
{

template<allocator Allocator = DefaultAllocator>
class StringBuilder
{
    buffer<char, Allocator> m_buffer;

public:
    constexpr StringBuilder() requires default_constructible<Allocator> = default;
    explicit constexpr StringBuilder(Allocator allocator) : m_buffer{ASL_MOVE(allocator)} {}

    constexpr ~StringBuilder() = default;

    constexpr StringBuilder(const StringBuilder&) requires copy_constructible<Allocator> = default;
    constexpr StringBuilder(StringBuilder&&) = default;

    constexpr StringBuilder& operator=(const StringBuilder&) requires copy_assignable<Allocator> = default;
    constexpr StringBuilder& operator=(StringBuilder&&) = default;

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

    StringBuilder& push(string_view sv) &
    {
        isize_t old_size = m_buffer.size();
        m_buffer.resize_zero(old_size + sv.size());
        asl::memcpy(m_buffer.data() + old_size, sv.data(), sv.size());
        return *this;
    }

    StringBuilder&& push(string_view sv) &&
    {
        isize_t old_size = m_buffer.size();
        m_buffer.resize_zero(old_size + sv.size());
        asl::memcpy(m_buffer.data() + old_size, sv.data(), sv.size());
        return ASL_MOVE(*this);
    }

    StringBuilder& push(char c) &
    {
        m_buffer.push(c);
        return *this;
    }

    StringBuilder&& push(char c) &&
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

StringBuilder() -> StringBuilder<>;

template<typename Allocator = DefaultAllocator>
class StringWriter : public asl::Writer
{
    StringBuilder<Allocator> m_builder;

public:
    constexpr StringWriter() requires default_constructible<Allocator> = default;
    explicit constexpr StringWriter(Allocator allocator) : m_builder{ASL_MOVE(allocator)} {}

    constexpr ~StringWriter() override = default;

    constexpr StringWriter(const StringWriter&) requires copy_constructible<Allocator> = default;
    constexpr StringWriter(StringWriter&&) = default;

    constexpr StringWriter& operator=(const StringWriter&) requires copy_assignable<Allocator> = default;
    constexpr StringWriter& operator=(StringWriter&&) = default;

    void write(span<const byte> str) override
    {
        m_builder.push(string_view{reinterpret_cast<const char*>(str.data()), str.size()});
    }

    constexpr string_view as_string_view() const
    {
        return m_builder.as_string_view();
    }

    string<Allocator> finish() &&
    {
        return ASL_MOVE(m_builder).finish();
    }

    template<allocator StringAllocator = Allocator>
    string<StringAllocator> as_string()
        requires default_constructible<StringAllocator>
    {
        return m_builder.as_string();
    }

    template<allocator StringAllocator = Allocator>
    string<StringAllocator> as_string(Allocator allocator)
    {
        return m_builder.as_string(ASL_MOVE(allocator));
    }
};

StringWriter() -> StringWriter<>;

template<allocator Allocator = DefaultAllocator, formattable... Args>
string<Allocator> format_to_string(string_view fmt, const Args&... args)
    requires default_constructible<Allocator>
{
    StringWriter writer{};
    format(&writer, fmt, args...);
    return ASL_MOVE(writer).finish();
}

template<allocator Allocator = DefaultAllocator, formattable... Args>
string<Allocator> format_to_string(Allocator allocator, string_view fmt, const Args&... args)
{
    StringWriter writer{ASL_MOVE(allocator)};
    format(&writer, fmt, args...);
    return ASL_MOVE(writer).finish();
}

} // namespace asl
