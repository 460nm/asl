#pragma once

#include "asl/meta.hpp"
#include "asl/annotations.hpp"
#include "asl/layout.hpp"
#include "asl/assert.hpp"

namespace asl
{

static constexpr isize_t dynamic_size = -1;

template<is_object T, isize_t kSize = dynamic_size>
class span
{
    static constexpr bool is_dynamic(isize_t size)
    {
        return size < 0;
    }
    
    static constexpr bool kIsDynamic = is_dynamic(kSize);

    using SizeType = select_t<kIsDynamic, isize_t, empty>;

    T* m_data{};
    ASL_NO_UNIQUE_ADDRESS SizeType m_size{};

public:
    constexpr span() requires (kIsDynamic || kSize == 0) = default;

    constexpr span(T* data, isize_t size)
        requires kIsDynamic
        : m_data{data}
        , m_size{size}
    {}

    constexpr explicit span(T* data, isize_t size)
        requires (!kIsDynamic)
        : m_data{data}
    {
        ASL_ASSERT(size == kSize);
    }

    template<isize_t N>
    constexpr span(T (&array)[N]) // NOLINT(*-explicit-conversions)
        requires (kIsDynamic)
        : m_data{array}
        , m_size{N}
    {}

    template<isize_t N>
    constexpr span(T (&array)[N]) // NOLINT(*-explicit-conversions)
        requires (!kIsDynamic) && (N == kSize)
        : m_data{array}
    {}

    template<is_object U, isize_t kOtherSize>
    constexpr explicit(!kIsDynamic)
    span(const span<U, kOtherSize>& other) // NOLINT(*-explicit-conversions)
        requires (
            (
                kIsDynamic ||
                is_dynamic(kOtherSize) ||
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

    constexpr isize_t size() const
    {
        if constexpr (kIsDynamic) { return m_size; }
        else { return kSize; }
    }

    constexpr isize_t size_bytes() const { return size() * size_of<T>; }

    constexpr bool is_empty() const { return size() == 0; }

    constexpr T* data() const { return m_data; }

    constexpr T* begin() const { return m_data; }
    constexpr T* end() const { return m_data + size(); }

    constexpr T& operator[](isize_t i) const
    {
        ASL_ASSERT(i >= 0 && i < size());
        return m_data[i]; // NOLINT(*-pointer-arithmetic)
    }

    template<isize_t kOffset, isize_t kSubSize = dynamic_size>
    constexpr auto subspan() const
        requires (
            kOffset >= 0 &&
            (kIsDynamic || kOffset <= kSize) &&
            (kIsDynamic || is_dynamic(kSubSize) || kSubSize <= kSize - kOffset)
        )
    {
        ASL_ASSERT(kOffset <= size());

        if constexpr (is_dynamic(kSubSize))
        {
            if constexpr (kIsDynamic)
            {
                return span<T>(data() + kOffset, size() - kOffset);
            }
            else
            {
                return span<T, kSize - kOffset>(data() + kOffset, size() - kOffset);
            }
        }
        else
        {
            ASL_ASSERT(kSubSize <= size() - kOffset);
            return span<T, kSubSize>(data() + kOffset, kSubSize);
        }
    }

    constexpr span<T> subspan(isize_t offset, isize_t sub_size = dynamic_size) const
    {
        ASL_ASSERT(offset <= size());
        
        if (is_dynamic(sub_size))
        {
            return span<T>{ data() + offset, size() - offset };
        }
        
        ASL_ASSERT(sub_size <= size() - offset);
        return span<T>{ data() + offset, sub_size };
    }

    template<isize_t kSubSize>
    constexpr auto first() const
        requires (
            kSubSize >= 0 &&
            (kIsDynamic || kSubSize <= kSize)
        )
    {
        ASL_ASSERT(kSubSize <= size());
        return span<T, kSubSize>{ data(), kSubSize };
    }

    constexpr span<T> first(isize_t sub_size) const
    {
        ASL_ASSERT(sub_size >= 0 && sub_size <= size());
        return span<T>{ data(), sub_size };
    }

    template<isize_t kSubSize>
    constexpr auto last() const
        requires (
            kSubSize >= 0 &&
            (kIsDynamic || kSubSize <= kSize)
        )
    {
        ASL_ASSERT(kSubSize <= size());
        return span<T, kSubSize>{ data() + size() - kSubSize, kSubSize };
    }

    constexpr span<T> last(isize_t sub_size) const
    {
        ASL_ASSERT(sub_size >= 0 && sub_size <= size());
        return span<T>{ data() + size() - sub_size, sub_size };
    }
};

template<is_object T, isize_t kSize>
inline span<const byte> as_bytes(span<T, kSize> s)
{
    return span<const byte>(
        reinterpret_cast<const byte*>(s.data()),
        s.size_bytes());
}

template<is_object T, isize_t kSize>
inline span<byte> as_mutable_bytes(span<T, kSize> s)
    requires (!is_const<T>)
{
    return span<byte>(
        reinterpret_cast<byte*>(s.data()),
        s.size_bytes());
}

template<is_object T, isize_t kSize>
span(T (&)[kSize]) -> span<T, kSize>;

} // namespace asl
