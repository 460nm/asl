// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#include "asl/base/meta.hpp"
#include "asl/base/assert.hpp"
#include "asl/base/annotations.hpp"
#include "asl/memory/layout.hpp"

namespace asl
{

static constexpr isize_t dynamic_size = -1;

// NOLINTBEGIN(*-convert-member-functions-to-static)
template<typename T>
class contiguous_iterator
{
    T* m_ptr;

public:
    constexpr explicit contiguous_iterator(T* ptr) : m_ptr{ptr} {}

    constexpr bool operator==(this contiguous_iterator self, contiguous_iterator other) = default;

    constexpr contiguous_iterator& operator++()
    {
        m_ptr += 1;
        return *this;
    }

    constexpr contiguous_iterator operator++(int)
    {
        return contiguous_iterator{ exchange(m_ptr, m_ptr + 1) };
    }

    constexpr T& operator*(this contiguous_iterator self) { return *self.m_ptr; }

    constexpr T* operator->(this contiguous_iterator self) { return self.m_ptr; }
};

template<typename T>
contiguous_iterator(T) -> contiguous_iterator<T>;

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
    constexpr span(T (&array)[N]) // NOLINT(*explicit*)
        requires (kIsDynamic)
        : m_data{static_cast<T*>(array)}
        , m_size{N}
    {}

    template<isize_t N>
    constexpr span(T (&array)[N]) // NOLINT(*explicit*)
        requires (!kIsDynamic) && (N == kSize)
        : m_data{static_cast<T*>(array)}
    {}

    template<is_object U, isize_t kOtherSize>
    constexpr explicit(!kIsDynamic)
    span(const span<U, kOtherSize>& other)
        requires (
            (
                kIsDynamic ||
                is_dynamic(kOtherSize) ||
                kOtherSize == kSize
            ) && convertible_to<U(&)[], T(&)[]>
        )
        : span{static_cast<U*>(other.data()), other.size()}
    {
    }

    constexpr span(const span&) = default;
    constexpr span(span&&) = default;

    constexpr span& operator=(const span&) = default;
    constexpr span& operator=(span&&) = default;

    ~span() = default;

    [[nodiscard]] constexpr isize_t size(this span self)
    {
        if constexpr (kIsDynamic) { return self.m_size; }
        else { return kSize; }
    }

    [[nodiscard]] constexpr isize_t size_bytes(this span self) { return self.size() * size_of<T>; }

    [[nodiscard]] constexpr bool is_empty(this span self) { return self.size() == 0; }

    [[nodiscard]] constexpr T* data(this span self) { return self.m_data; }

    [[nodiscard]] constexpr contiguous_iterator<T> begin(this span self)
    {
        return contiguous_iterator{self.m_data};
    }

    [[nodiscard]] constexpr contiguous_iterator<T> end(this span self)
    {
        return contiguous_iterator{self.m_data + self.size()};
    }

    constexpr T& operator[](this span self, isize_t i)
    {
        ASL_ASSERT(i >= 0 && i < self.size());
        return self.m_data[i]; // NOLINT(*-pointer-arithmetic)
    }

    template<isize_t kOffset, isize_t kSubSize = dynamic_size>
    [[nodiscard]] constexpr auto subspan(this span self)
        requires (
            kOffset >= 0 &&
            (kIsDynamic || kOffset <= kSize) &&
            (kIsDynamic || is_dynamic(kSubSize) || kSubSize <= kSize - kOffset)
        )
    {
        ASL_ASSERT(kOffset <= self.size());

        if constexpr (is_dynamic(kSubSize))
        {
            if constexpr (kIsDynamic)
            {
                return span<T>(self.data() + kOffset, self.size() - kOffset);
            }
            else
            {
                return span<T, kSize - kOffset>(self.data() + kOffset, self.size() - kOffset);
            }
        }
        else
        {
            ASL_ASSERT(kSubSize <= self.size() - kOffset);
            return span<T, kSubSize>(self.data() + kOffset, kSubSize);
        }
    }

    [[nodiscard]] constexpr span<T> subspan(this span self, isize_t offset)
    {
        ASL_ASSERT(offset <= self.size());
        return span<T>{ self.data() + offset, self.size() - offset };
    }

    [[nodiscard]] constexpr span<T> subspan(this span self, isize_t offset, isize_t sub_size)
    {
        ASL_ASSERT(offset <= self.size() && !is_dynamic(sub_size));
        ASL_ASSERT(sub_size <= self.size() - offset);
        return span<T>{ self.data() + offset, sub_size };
    }

    template<isize_t kSubSize>
    [[nodiscard]] constexpr auto first(this span self)
        requires (
            kSubSize >= 0 &&
            (kIsDynamic || kSubSize <= kSize)
        )
    {
        ASL_ASSERT(kSubSize <= self.size());
        return span<T, kSubSize>{ self.data(), kSubSize };
    }

    [[nodiscard]] constexpr span<T> first(this span self, isize_t sub_size)
    {
        ASL_ASSERT(sub_size >= 0 && sub_size <= self.size());
        return span<T>{ self.data(), sub_size };
    }

    template<isize_t kSubSize>
    [[nodiscard]] constexpr auto last(this span self)
        requires (
            kSubSize >= 0 &&
            (kIsDynamic || kSubSize <= kSize)
        )
    {
        ASL_ASSERT(kSubSize <= self.size());
        return span<T, kSubSize>{ self.data() + self.size() - kSubSize, kSubSize };
    }

    [[nodiscard]] constexpr span<T> last(this span self, isize_t sub_size)
    {
        ASL_ASSERT(sub_size >= 0 && sub_size <= self.size());
        return span<T>{ self.data() + self.size() - sub_size, sub_size };
    }
};
// NOLINTEND(*-convert-member-functions-to-static)

template<is_object T, isize_t kSize>
inline span<const byte> as_bytes(span<T, kSize> s)
{
    return span<const byte>(
        reinterpret_cast<const byte*>(s.data()), // NOLINT(*-reinterpret-cast)
        s.size_bytes());
}

template<is_object T, isize_t kSize>
inline span<byte> as_mutable_bytes(span<T, kSize> s)
    requires (!is_const<T>)
{
    return span<byte>(
        reinterpret_cast<byte*>(s.data()), // NOLINT(*-reinterpret-cast)
        s.size_bytes());
}

template<is_object T, isize_t kSize>
span(T (&)[kSize]) -> span<T, kSize>;

} // namespace asl
