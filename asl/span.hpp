#pragma once

#include "asl/meta.hpp"
#include "asl/annotations.hpp"
#include "asl/layout.hpp"
#include "asl/assert.hpp"

namespace asl
{

static constexpr int64_t dynamic_size = -1;

template<is_object T, int64_t kSize = dynamic_size>
class span
{
    static constexpr bool kIsDynamic = kSize < 0;

    using SizeType = select_t<kIsDynamic, int64_t, empty>;

    T* m_data{};
    ASL_NO_UNIQUE_ADDRESS SizeType m_size{};

public:
    constexpr span() = default;

    constexpr span(T* data, int64_t size)
        requires kIsDynamic
        : m_data{data}
        , m_size{size}
    {}

    constexpr explicit span(T* data, int64_t size)
        requires (!kIsDynamic)
        : m_data{data}
    {
        ASL_ASSERT(size == kSize);
    }

    template<int64_t N>
    constexpr span(T (&array)[N]) // NOLINT(*-explicit-conversions)
        requires (kIsDynamic)
        : m_data{array}
        , m_size{N}
    {}

    template<int64_t N>
    constexpr span(T (&array)[N]) // NOLINT(*-explicit-conversions)
        requires (!kIsDynamic) && (N == kSize)
        : m_data{array}
    {}

    template<is_object U, int64_t kOtherSize>
    constexpr explicit(!kIsDynamic)
    span(const span<U, kOtherSize>& other) // NOLINT(*-explicit-conversions)
        requires (
            (
                kIsDynamic ||
                span<U, kOtherSize>::kIsDynamic ||
                kOtherSize == kSize
            ) && convertible_from<T(&)[], U(&)[]>
        )
        : span{static_cast<U*>(other.data()), other.size()}
    {
    }

    constexpr span(const span&) = default;
    constexpr span(span&&) = default;
    
    constexpr span& operator=(const span&) = default;
    constexpr span& operator=(span&&) = default;

    ~span() = default;

    constexpr int64_t size() const
    {
        if constexpr (kIsDynamic) { return m_size; }
        else { return kSize; }
    }

    constexpr int64_t size_bytes() const { return size() * size_of<T>; }

    constexpr bool is_empty() const { return size() == 0; }

    constexpr T* data() const { return m_data; }

    constexpr T* begin() const { return m_data; }
    constexpr T* end() const { return m_data + size(); }

    constexpr T& operator[](int64_t i) const
    {
        ASL_ASSERT(i >= 0 && i < size());
        return m_data[i]; // NOLINT(*-pointer-arithmetic)
    }

    // @Todo subspan, first, last
    // @Todo as_(mutable_)bytes

    template<is_object U, int64_t kOtherSize>
    friend class span;
};

} // namespace asl
