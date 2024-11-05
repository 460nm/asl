#pragma once

#include "asl/integers.hpp"
#include "asl/meta.hpp"

namespace asl
{

class string_view
{
    const char* m_data{};
    int64_t     m_size{};

public:
    constexpr string_view() = default;

    constexpr string_view(nullptr_t) : string_view() {} // NOLINT(*-explicit-conversions)

    constexpr string_view(const char* data, int64_t size)
        : m_data{data}
        , m_size{size}
    {}

    constexpr string_view(const char* data) // NOLINT(*-explicit-conversions)
        : m_data{data}
        , m_size{static_cast<int64_t>(__builtin_strlen(data))}
    {}

    constexpr string_view(const string_view&) = default;
    constexpr string_view(string_view&&) = default;

    constexpr string_view& operator=(const string_view&) = default;
    constexpr string_view& operator=(string_view&&) = default;

    ~string_view() = default;
};

} // namespace asl
