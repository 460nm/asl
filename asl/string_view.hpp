#pragma once

#include "asl/integers.hpp"
#include "asl/meta.hpp"

namespace asl
{

// @Todo Replace all the __builtin_strlen

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
};

} // namespace asl

constexpr asl::string_view operator ""_sv(const char* s, size_t len)
{
    return asl::string_view(s, static_cast<isize_t>(len));
}
