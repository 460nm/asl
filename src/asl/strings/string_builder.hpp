// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

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
    explicit constexpr StringBuilder(Allocator allocator) : m_buffer{std::move(allocator)} {}

    constexpr ~StringBuilder() = default;

    constexpr StringBuilder(const StringBuilder&) requires copy_constructible<Allocator> = default;
    constexpr StringBuilder(StringBuilder&&) = default;

    constexpr StringBuilder& operator=(const StringBuilder&) requires copy_assignable<Allocator> = default;
    constexpr StringBuilder& operator=(StringBuilder&&) = default;

    [[nodiscard]] constexpr string_view as_string_view() const
    {
        auto span = m_buffer.as_span();
        return string_view{span.data(), span.size()};
    }

    void reset()
    {
        m_buffer.clear();
    }

    auto push(this auto&& self, string_view sv) -> decltype(self)
        requires (!is_const<un_ref_t<decltype(self)>>)
    {
        const isize_t old_size = self.m_buffer.size();
        self.m_buffer.resize_uninit(old_size + sv.size());
        // NOLINTNEXTLINE(*-pointer-arithmetic)
        asl::memcpy(self.m_buffer.data() + old_size, sv.data(), sv.size());
        return std::forward<decltype(self)>(self);
    }

    auto push(this auto&& self, char c) -> decltype(self)
        requires (!is_const<un_ref_t<decltype(self)>>)
    {
        self.m_buffer.push(c);
        return std::forward<decltype(self)>(self);
    }

    string<Allocator> finish() &&
    {
        return string<Allocator>{std::move(m_buffer)};
    }

    template<allocator StringAllocator = Allocator>
    string<StringAllocator> as_string() const
        requires default_constructible<StringAllocator>
    {
        return string<StringAllocator>{as_string_view()};
    }

    template<allocator StringAllocator = Allocator>
    string<StringAllocator> as_string(Allocator allocator) const
    {
        return string<StringAllocator>{as_string_view(), std::move(allocator)};
    }
};

StringBuilder() -> StringBuilder<>;

template<typename Allocator = DefaultAllocator>
class StringWriter : public asl::Writer
{
    StringBuilder<Allocator> m_builder;

public:
    constexpr StringWriter() requires default_constructible<Allocator> = default;
    explicit constexpr StringWriter(Allocator allocator) : m_builder{std::move(allocator)} {}

    constexpr ~StringWriter() override = default;

    constexpr StringWriter(const StringWriter&) requires copy_constructible<Allocator> = default;
    constexpr StringWriter(StringWriter&&) = default;

    constexpr StringWriter& operator=(const StringWriter&) requires copy_assignable<Allocator> = default;
    constexpr StringWriter& operator=(StringWriter&&) = default;

    void write(span<const byte> str) override
    {
        // NOLINTNEXTLINE(*-reinterpret-cast)
        m_builder.push(string_view{reinterpret_cast<const char*>(str.data()), str.size()});
    }

    [[nodiscard]] constexpr string_view as_string_view() const
    {
        return m_builder.as_string_view();
    }

    string<Allocator> finish() &&
    {
        return std::move(m_builder).finish();
    }

    template<allocator StringAllocator = Allocator>
    string<StringAllocator> as_string() const
        requires default_constructible<StringAllocator>
    {
        return m_builder.as_string();
    }

    template<allocator StringAllocator = Allocator>
    string<StringAllocator> as_string(Allocator allocator) const
    {
        return m_builder.as_string(std::move(allocator));
    }
};

StringWriter() -> StringWriter<>;

template<allocator Allocator = DefaultAllocator>
string<Allocator> format_to_string(string_view fmt, const formattable auto&... args)
    requires default_constructible<Allocator>
{
    StringWriter writer{};
    format(&writer, fmt, args...);
    return std::move(writer).finish();
}

template<allocator Allocator = DefaultAllocator>
string<Allocator> format_to_string(Allocator allocator, string_view fmt, const formattable auto&... args)
{
    StringWriter writer{std::move(allocator)};
    format(&writer, fmt, args...);
    return std::move(writer).finish();
}

} // namespace asl
