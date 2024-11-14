#pragma once

#include "asl/integers.hpp"
#include "asl/meta.hpp"
#include "asl/span.hpp"
#include "asl/memory.hpp"

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

    constexpr string_view(const string_view&) = default;
    constexpr string_view(string_view&&) = default;

    constexpr string_view& operator=(const string_view&) = default;
    constexpr string_view& operator=(string_view&&) = default;

    ~string_view() = default;

    constexpr isize_t size() const { return m_size; }
   
    constexpr bool is_empty() const { return m_size == 0; }

    constexpr const char* data() const { return m_data; }

    constexpr const char* begin() const { return m_data; }
    
    constexpr const char* end() const { return m_data + m_size; } // NOLINT(*-pointer-arithmetic)

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

    constexpr bool operator==(string_view other) const
    {
        if (m_size != other.m_size) { return false; }
        return memcmp(m_data, other.m_data, m_size) == 0;
    }
};

} // namespace asl

constexpr asl::string_view operator ""_sv(const char* s, size_t len)
{
    return asl::string_view(s, static_cast<isize_t>(len));
}
