// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#include "asl/base/support.hpp"
#include "asl/base/memory.hpp"
#include "asl/base/bits.hpp"
#include "asl/base/memory_ops.hpp"
#include "asl/base/meta.hpp"
#include "asl/base/assert.hpp"
#include "asl/types/span.hpp"
#include "asl/hashing/hash.hpp"
#include "asl/allocator/allocator.hpp"

namespace asl
{

template<typename T, allocator Allocator = DefaultAllocator>
requires is_object<T> && movable<T>
class buffer
{
    T*         m_data{};
    isize_t    m_capacity{};

    static constexpr size_t kOnHeapMask = 0x8000'0000'0000'0000ULL;

    // bit 63       : 1 = on heap, 0 = inline
    // bits [62:56] : size when inline
    // bits [62:0]  : size when on heap
    size_t     m_size_encoded_{};

    ASL_NO_UNIQUE_ADDRESS Allocator m_allocator;

    static constexpr isize_t kInlineRegionSize = sizeof(T*) + sizeof(isize_t) + sizeof(size_t);

public:
    static constexpr isize_t kInlineCapacity = []() {
        // 1 byte is used for size inline in m_size_encoded.
        // This is enough because we have at most 24 bytes available,
        // so 23 chars of capacity.
        const isize_t available_size = kInlineRegionSize - 1;
        return available_size / sizeof(T);
    }();

private:
    static_assert(alignof(T) <= alignof(T*));
    static_assert(alignof(T*) == alignof(isize_t));
    static_assert(alignof(T*) == alignof(size_t));

    [[nodiscard]] constexpr size_t load_size_encoded() const
    {
        size_t s{};
        asl::memcpy(&s, &m_size_encoded_, sizeof(size_t));
        return s;
    }

    constexpr void store_size_encoded(size_t encoded)
    {
        asl::memcpy(&m_size_encoded_, &encoded, sizeof(size_t));
    }

    static constexpr bool is_on_heap(size_t size_encoded)
    {
        return (size_encoded & kOnHeapMask) != 0;
    }

    static constexpr size_t encode_size_heap(isize_t size)
    {
        return static_cast<size_t>(size) | kOnHeapMask;
    }

    static constexpr isize_t decode_size(size_t size_encoded)
    {
        if constexpr (kInlineCapacity == 0)
        {
            return is_on_heap(size_encoded)
                ? static_cast<isize_t>(size_encoded & (~kOnHeapMask))
                : 0;
        }
        else
        {
            return is_on_heap(size_encoded)
                ? static_cast<isize_t>(size_encoded & (~kOnHeapMask))
                : static_cast<isize_t>(size_encoded >> 56U);
        }
    }

    [[nodiscard]] constexpr bool is_on_heap() const
    {
        return is_on_heap(load_size_encoded());
    }

    constexpr void* push_uninit()
    {
        const isize_t sz = size();
        resize_uninit_inner(sz + 1);
        return data() + sz;
    }

    constexpr void resize_uninit_inner(isize_t new_size)
    {
        if constexpr (!is_trivially_destructible<T>)
        {
            const isize_t old_size = size();
            if (new_size < old_size)
            {
                destroy_n(data() + new_size, old_size - new_size);
            }
        }
        reserve_capacity(new_size);
        set_size(new_size);
    }

    constexpr void set_size_inline(isize_t new_size)
    {
        ASL_ASSERT(new_size >= 0 && new_size <= kInlineCapacity);
        const size_t size_encoded =
            (load_size_encoded() & size_t{0x00ff'ffff'ffff'ffff})
            | (std::bit_cast<size_t>(new_size) << 56U);
        store_size_encoded(size_encoded);
    }

    constexpr void set_size(isize_t new_size)
    {
        ASL_ASSERT(new_size >= 0 && new_size <= capacity());
        if (is_on_heap())
        {
            store_size_encoded(encode_size_heap(new_size));
        }
        else
        {
            set_size_inline(new_size);
        }
    }

    // NOLINTNEXTLINE(*-rvalue-reference-param-not-moved)
    void move_from_other(buffer&& other, bool assign)
    {
        if (other.is_on_heap())
        {
            // If the other in on heap, destroy here and adopt their
            // data. We'll soon adopt the allocator as well.
            destroy();
            m_data = other.m_data;
            m_capacity = other.m_capacity;
            store_size_encoded(other.load_size_encoded());
        }
        else if (!assign || m_allocator == other.m_allocator)
        {
            // If allocators are compatible, we can move other's inline
            // data here, even if it's on heap here, because that
            // memory can be freed by other's allocator, which we will
            // soon adopt.
            //
            // @Note There is an argument to be made for not doing this and
            // instead destroying our data here and moving into inline
            // storage, which frees one allocation. But also this avoids
            // freeing. So I don't know.
            // Maybe If this storage is much much larger than the inline
            // data, it's worth freeing.
            const isize_t other_n = other.size();
            const isize_t this_n = size();
            resize_uninit_inner(other_n);
            if (other_n <= this_n)
            {
                move_assign_n(data(), other.data(), other_n);
            }
            else
            {
                move_assign_n(data(), other.data(), this_n);
                move_uninit_n(data() + this_n, other.data() + this_n, other_n - this_n);
            }
            destroy_n(other.data(), other_n);
        }
        else
        {
            // Otherwise, if we have to free, because the allocators are
            // not compatible, well we free and move into our inline
            // storage region.
            // There is an optimization here when the data is trivially
            // move constructible (which implies trivially destructible),
            // we copy the whole inline region, which includes the size.
            // Very magic.

            destroy();
            if constexpr (is_trivially_move_constructible<T>)
            {
                ASL_ASSERT(!is_on_heap());
                asl::memcpy(this, &other, kInlineRegionSize);
            }
            else
            {
                const isize_t n = other.size();
                ASL_ASSERT(n <= kInlineCapacity);
                resize_uninit_inner(n);
                ASL_ASSERT(!is_on_heap());
                move_uninit_n(data(), other.data(), n);
                destroy_n(other.data(), n);
            }
        }

        other.set_size_inline(0);

        if (assign)
        {
            m_allocator = std::move(other.m_allocator);
        }
    }

    void copy_range(span<const T> to_copy)
    {
        const isize_t this_size = size();
        const isize_t new_size = to_copy.size();

        resize_uninit_inner(to_copy.size());
        ASL_ASSERT(capacity() >= new_size);
        ASL_ASSERT(size() == to_copy.size());

        if (new_size <= this_size)
        {
            copy_assign_n(data(), to_copy.data(), new_size);
        }
        else
        {
            copy_assign_n(data(), to_copy.data(), this_size);
            copy_uninit_n(data() + this_size, to_copy.data() + this_size, new_size - this_size);
        }
    }

    template<typename... Args>
    void resize_inner(isize_t new_size, Args&&... args)
        requires constructible_from<T, Args&&...>
    {
        ASL_ASSERT(new_size >= 0);

        isize_t old_size = size();
        resize_uninit_inner(new_size);

        T* data_ptr = data();
        T* end = data_ptr + new_size;

        // NOLINTNEXTLINE(*-pointer-arithmetic)
        for (T* it = data_ptr + old_size; it < end; ++it)
        {
            construct_at<T>(it, std::forward<Args>(args)...);
        }
    }

public:
    constexpr buffer() requires is_default_constructible<Allocator> = default;

    explicit constexpr buffer(span<const T> s)
        requires is_default_constructible<Allocator>
        : buffer{}
    {
        copy_range(s);
    }

    explicit constexpr buffer(Allocator allocator)
        : m_allocator{std::move(allocator)}
    {}

    explicit constexpr buffer(span<const T> s, Allocator allocator)
        : m_allocator{std::move(allocator)}
    {
        copy_range(s);
    }

    constexpr buffer(const buffer& other)
        requires copy_constructible<Allocator> && copyable<T>
        : m_allocator{other.m_allocator}
    {
        copy_range(other);
    }

    constexpr buffer(buffer&& other)
        : buffer(std::move(other.m_allocator))
    {
        move_from_other(std::move(other), false);
    }

    constexpr buffer& operator=(const buffer& other)
        requires copyable<T>
    {
        if (&other == this) { return *this; }
        copy_range(other);
        return *this;
    }

    constexpr buffer& operator=(buffer&& other)
    {
        if (&other == this) { return *this; }
        move_from_other(std::move(other), true);
        return *this;
    }

    ~buffer()
    {
        destroy();
    }

    constexpr Allocator allocator_copy() const
        requires copy_constructible<Allocator>
    {
        return m_allocator;
    }

    constexpr Allocator& allocator() { return m_allocator; }

    [[nodiscard]] constexpr isize_t size() const
    {
        return decode_size(load_size_encoded());
    }

    [[nodiscard]] constexpr bool is_empty() const { return size() == 0; }

    [[nodiscard]] constexpr isize_t capacity() const
    {
        if constexpr (kInlineCapacity == 0)
        {
            return m_capacity;
        }
        else
        {
            return is_on_heap() ? m_capacity : kInlineCapacity;
        }
    }

    void clear()
    {
        const isize_t current_size = size();
        if (current_size == 0) { return; }

        destroy_n(data(), current_size);
        set_size(0);
    }

    void destroy()
    {
        clear();
        if (is_on_heap())
        {
            if (m_data != nullptr)
            {
                auto current_layout = layout::array<T>(m_capacity);
                m_allocator.dealloc(m_data, current_layout);
            }
            set_size_inline(0);
        }
    }

    void reserve_capacity(isize_t new_capacity)
    {
        ASL_ASSERT(new_capacity >= 0);

        if (new_capacity <= capacity()) { return; }
        ASL_ASSERT(new_capacity > kInlineCapacity);

        new_capacity = static_cast<isize_t>(bit_ceil(static_cast<uint64_t>(new_capacity)));

        T* old_data = data();
        const isize_t old_capacity = capacity();
        const isize_t current_size = size();
        const bool currently_on_heap = is_on_heap();

        auto old_layout = layout::array<T>(old_capacity);
        auto new_layout = layout::array<T>(new_capacity);

        if (currently_on_heap && is_trivially_move_constructible<T>)
        {
            m_data = static_cast<T*>(m_allocator.realloc(m_data, old_layout, new_layout));
            m_capacity = new_capacity;
            return;
        }

        T* new_data = static_cast<T*>(m_allocator.alloc(new_layout));

        move_uninit_n(new_data, old_data, current_size);
        destroy_n(old_data, current_size);

        if (currently_on_heap)
        {
            m_allocator.dealloc(old_data, old_layout);
        }

        m_data = new_data;
        m_capacity = new_capacity;
        store_size_encoded(encode_size_heap(current_size));
    }

    constexpr void resize_uninit(isize_t new_size)
        requires is_trivially_default_constructible<T>
    {
        resize_uninit_inner(new_size);
    }

    constexpr void resize_zero(isize_t new_size)
        requires is_trivially_default_constructible<T>
    {
        const isize_t old_size = size();
        resize_uninit_inner(new_size);

        if (new_size > old_size)
        {
            memzero(data() + old_size, (new_size - old_size) * static_cast<isize_t>(sizeof(T)));
        }
    }

    void resize(isize_t new_size)
        requires is_default_constructible<T>
    {
        if constexpr (is_trivially_default_constructible<T>)
        {
            resize_zero(new_size);
        }
        else
        {
            resize_inner(new_size);
        }
    }

    void resize(isize_t new_size, const T& value)
    {
        resize_inner(new_size, value);
    }

    constexpr T& push(auto&&... args)
        requires constructible_from<T, decltype(args)&&...>
    {
        void* uninit = push_uninit();
        T* init = construct_at<T>(uninit, std::forward<decltype(args)>(args)...);
        return *init;
    }

    auto data(this auto&& self)
    {
        using return_type = add_ptr_t<copy_const_t<remove_ref_t<decltype(self)>, T>>;
        // NOLINTNEXTLINE(*-reinterpret-cast)
        auto&& buffer = reinterpret_cast<copy_cref_t<decltype(self), class buffer>>(self);
        if constexpr (kInlineCapacity == 0)
        {
            return return_type{ buffer.m_data };
        }
        else
        {
            return buffer.is_on_heap()
                ? return_type{ buffer.m_data }
                // NOLINTNEXTLINE(*-reinterpret-cast)
                : std::launder(reinterpret_cast<return_type>(&buffer));
        }
    }

    constexpr auto begin(this auto&& self)
    {
        using type = copy_const_t<remove_ref_t<decltype(self)>, T>;
        return contiguous_iterator<type>{self.data()};
    }

    constexpr auto end(this auto&& self)
    {
        using type = copy_const_t<remove_ref_t<decltype(self)>, T>;
        return contiguous_iterator<type>{self.data() + self.size()};
    }

    constexpr operator span<const T>() const // NOLINT(*explicit*)
    {
        return as_span();
    }

    constexpr operator span<T>() // NOLINT(*explicit*)
    {
        return as_span();
    }

    constexpr auto as_span(this auto&& self)
    {
        using type = copy_const_t<remove_ref_t<decltype(self)>, T>;
        return span<type>{self.data(), self.size()};
    }

    constexpr auto&& operator[](this auto&& self, isize_t i)
    {
        ASL_ASSERT(i >= 0 && i < self.size());
        return std::forward_like<decltype(self)>(std::forward<decltype(self)>(self).data()[i]);
    }

    template<typename H>
    requires hashable<T>
    friend H AslHashValue(H h, const buffer& b)
    {
        return H::combine_contiguous(std::move(h), b.as_span());
    }
};

} // namespace asl

