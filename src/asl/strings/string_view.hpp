// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#include "asl/base/assert.hpp"
#include "asl/base/memory_ops.hpp"
#include "asl/types/span.hpp"

namespace asl
{

// NOLINTBEGIN(*-convert-member-functions-to-static)
class string_view
{
    const char* m_data{};
    isize_t     m_size{};

public:
    constexpr string_view() = default;

    constexpr string_view(nullptr_t) : string_view() {} // NOLINT(*explicit*)

    constexpr string_view(const char* data, isize_t size)
        : m_data{data}
        , m_size{size}
    {}

    constexpr string_view(const char* begin, const char* end)
        : m_data{begin}
        , m_size{end - begin}
    {
        ASL_ASSERT(begin <= end);
    }

    template<isize_t kSize>
    constexpr string_view(const char (&str)[kSize]) // NOLINT(*explicit*)
        requires (kSize >= 1)
        : m_data{static_cast<const char*>(str)}
        , m_size{kSize - 1}
    {
        ASL_ASSERT(m_data[kSize - 1] == '\0'); // NOLINT(*-pointer-arithmetic)
    }

    static constexpr string_view from_zstr(const char* str)
    {
        return {str, asl::strlen(str)};
    }

    constexpr string_view(const string_view&) = default;
    constexpr string_view(string_view&&) = default;

    constexpr string_view& operator=(const string_view&) = default;
    constexpr string_view& operator=(string_view&&) = default;

    ~string_view() = default;

    [[nodiscard]] constexpr isize_t size(this string_view self)
    {
        return self.m_size;
    }

    [[nodiscard]] constexpr bool is_empty(this string_view self)
    {
        return self.m_size == 0;
    }

    [[nodiscard]] constexpr const char* data(this string_view self)
    {
        return self.m_data;
    }

    [[nodiscard]] constexpr contiguous_iterator<const char> begin(this string_view self)
    {
        return contiguous_iterator{self.m_data};
    }

    [[nodiscard]] constexpr contiguous_iterator<const char> end(this string_view self)
    {
        // NOLINTNEXTLINE(*-pointer-arithmetic)
        return contiguous_iterator{self.m_data + self.m_size};
    }

    [[nodiscard]] constexpr span<const char> as_span(this string_view self)
    {
        return {self.m_data, self.m_size};
    }

    [[nodiscard]] constexpr char operator[](this string_view self, isize_t i)
    {
        ASL_ASSERT(i >= 0 && i < self.m_size);
        return self.m_data[i]; // NOLINT(*-pointer-arithmetic)
    }

    [[nodiscard]] constexpr string_view substr(this string_view self, isize_t offset, isize_t size)
    {
        ASL_ASSERT(offset >= 0 && size >= 0 && offset + size <= self.m_size);
        return string_view{self.m_data + offset, size}; // NOLINT(*-pointer-arithmetic)
    }

    [[nodiscard]] constexpr string_view substr(this string_view self, isize_t offset)
    {
        ASL_ASSERT(offset >= 0 && offset <= self.m_size);
        return string_view{self.m_data + offset, self.m_size - offset}; // NOLINT(*-pointer-arithmetic)
    }

    [[nodiscard]] constexpr string_view first(this string_view self, isize_t size)
    {
        return self.substr(0, size);
    }

    [[nodiscard]] constexpr string_view last(this string_view self, isize_t size)
    {
        return self.substr(self.m_size - size);
    }

    constexpr bool operator==(this string_view self, string_view other)
    {
        if (self.m_size != other.m_size) { return false; }
        return asl::memcmp(self.m_data, other.m_data, self.m_size) == 0;
    }

    template<typename H>
    friend H AslHashValue(H h, string_view sv)
    {
        return H::combine(H::combine_contiguous(h, as_bytes(sv.as_span())), sv.size());
    }
};
// NOLINTEND(*-convert-member-functions-to-static)

} // namespace asl

constexpr asl::string_view operator ""_sv(const char* s, size_t len)
{
    return {s, static_cast<isize_t>(len)};
}
