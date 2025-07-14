// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#include "asl/containers/buffer.hpp"
#include "asl/strings/string_view.hpp"

namespace asl
{

template<allocator Allocator = DefaultAllocator>
class string : protected buffer<char, Allocator>
{
    using Base = buffer<char, Allocator>;

    explicit constexpr string(buffer<char, Allocator>&& b) :
        Base{std::move(b)}
    {}

    template<allocator A>
    friend class StringBuilder;

public:
    constexpr string() requires default_constructible<Allocator> = default;

    explicit constexpr string(Allocator allocator)
        : Base{std::move(allocator)}
    {}

    // NOLINTNEXTLINE(*explicit*)
    constexpr string(string_view sv)
        requires default_constructible<Allocator>
        : Base{sv.as_span()}
    {}

    constexpr string(string_view sv, Allocator allocator)
        : Base{sv.as_span(), std::move(allocator)}
    {}

    using Base::size;

    using Base::is_empty;

    [[nodiscard]] constexpr const char* data() const { return Base::data(); }

    // NOLINTNEXTLINE(*explicit*)
    constexpr operator string_view() const
    {
        return as_string_view();
    }

    [[nodiscard]] constexpr string_view as_string_view() const
    {
        auto span = Base::as_span();
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
