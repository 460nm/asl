// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#include "asl/base/meta.hpp"
#include "asl/memory/allocator.hpp"
#include "asl/memory/memory.hpp"
#include "asl/base/annotations.hpp"
#include "asl/base/assert.hpp"
#include "asl/types/span.hpp"
#include "asl/hashing/hash.hpp"

namespace asl
{

template<is_object T, allocator Allocator = DefaultAllocator>
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

    static constexpr isize_t kInlineRegionSize = size_of<T*> + size_of<isize_t> + size_of<size_t>;

public:
    static constexpr isize_t kInlineCapacity = []() {
        // 1 byte is used for size inline in m_size_encoded.
        // This is enough because we have at most 24 bytes available,
        // so 23 chars of capacity.
        const isize_t available_size = kInlineRegionSize - 1;
        return available_size / size_of<T>;
    }();

private:
    static_assert(align_of<T> <= align_of<T*>);
    static_assert(align_of<T*> == align_of<isize_t>);
    static_assert(align_of<T*> == align_of<size_t>);

    constexpr size_t load_size_encoded() const
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
                : static_cast<isize_t>(size_encoded >> 56);
        }
    }

    constexpr bool is_on_heap() const
    {
        return is_on_heap(load_size_encoded());
    }

    constexpr T* push_uninit()
    {
        isize_t sz = size();
        resize_uninit_inner(sz + 1);
        return data() + sz;
    }

    constexpr void resize_uninit_inner(isize_t new_size)
    {
        isize_t old_size = size();
        if (!trivially_destructible<T> && new_size < old_size)
        {
            destroy_n(data() + new_size, old_size - new_size);
        }
        reserve_capacity(new_size);
        set_size(new_size);
    }

    constexpr void set_size_inline(isize_t new_size)
    {
        ASL_ASSERT(new_size >= 0 && new_size <= kInlineCapacity);
        size_t size_encoded = (load_size_encoded() & size_t{0x00ff'ffff'ffff'ffff}) | (bit_cast<size_t>(new_size) << 56);
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
            destroy();
            m_data = other.m_data;
            m_capacity = other.m_capacity;
            store_size_encoded(other.load_size_encoded());
        }
        else if (trivially_move_constructible<T>)
        {
            destroy();
            asl::memcpy(this, &other, kInlineRegionSize);
        }
        else if (!assign || m_allocator == other.m_allocator)
        {
            isize_t other_n = other.size();
            isize_t this_n = size();
            resize_uninit_inner(other_n);
            if (other_n <= this_n)
            {
                relocate_assign_n(data(), other.data(), other_n);
            }
            else
            {
                relocate_assign_n(data(), other.data(), this_n);
                relocate_uninit_n(data() + this_n, other.data() + this_n, other_n - this_n);
            }
        }
        else
        {
            destroy();
            isize_t n = other.size();
            ASL_ASSERT(n <= kInlineCapacity);
            relocate_uninit_n(data(), other.data(), n);
            set_size_inline(n);
        }

        other.set_size_inline(0);

        if (assign)
        {
            m_allocator = ASL_MOVE(other.m_allocator);
        }
    }

    void copy_range(span<const T> to_copy)
    {
        isize_t this_size = size();
        isize_t new_size = to_copy.size();

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
            construct_at<T>(it, ASL_FWD(args)...);
        }
    }

public:
    constexpr buffer() requires default_constructible<Allocator> = default;

    explicit constexpr buffer(span<const T> s)
        requires default_constructible<Allocator>
        : buffer{}
    {
        copy_range(s);
    }

    explicit constexpr buffer(Allocator allocator)
        : m_allocator{ASL_MOVE(allocator)}
    {}

    explicit constexpr buffer(span<const T> s, Allocator allocator)
        : m_allocator{ASL_MOVE(allocator)}
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
        requires moveable<T>
        : buffer(ASL_MOVE(other.m_allocator))
    {
        move_from_other(ASL_MOVE(other), false);
    }

    constexpr buffer& operator=(const buffer& other)
        requires copyable<T>
    {
        if (&other == this) { return *this; }
        copy_range(other);
        return *this;
    }

    constexpr buffer& operator=(buffer&& other)
        requires moveable<T>
    {
        if (&other == this) { return *this; }
        move_from_other(ASL_MOVE(other), true);
        return *this;
    }

    ~buffer()
    {
        destroy();
    }

    constexpr isize_t size() const
    {
        return decode_size(load_size_encoded());
    }

    constexpr isize_t capacity() const
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
        isize_t current_size = size();
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
        ASL_ASSERT_RELEASE(new_capacity <= 0x4000'0000'0000'0000);

        if (new_capacity <= capacity()) { return; }
        ASL_ASSERT(new_capacity > kInlineCapacity);

        new_capacity = static_cast<isize_t>(round_up_pow2(static_cast<uint64_t>(new_capacity)));

        T* old_data = data();
        const isize_t old_capacity = capacity();
        const isize_t current_size = size();
        const bool currently_on_heap = is_on_heap();

        auto old_layout = layout::array<T>(old_capacity);
        auto new_layout = layout::array<T>(new_capacity);

        if (currently_on_heap && trivially_move_constructible<T>)
        {
            m_data = reinterpret_cast<T*>(m_allocator.realloc(m_data, old_layout, new_layout));
            m_capacity = new_capacity;
            return;
        }

        T* new_data = reinterpret_cast<T*>(m_allocator.alloc(new_layout));

        relocate_uninit_n(new_data, old_data, current_size);

        if (currently_on_heap)
        {
            m_allocator.dealloc(old_data, old_layout);
        }

        m_data = new_data;
        m_capacity = new_capacity;
        store_size_encoded(encode_size_heap(current_size));
    }

    constexpr void resize_uninit(isize_t new_size)
        requires trivially_default_constructible<T> && trivially_destructible<T>
    {
        reserve_capacity(new_size);
        set_size(new_size);
    }

    constexpr void resize_zero(isize_t new_size)
        requires trivially_default_constructible<T> && trivially_destructible<T>
    {
        isize_t old_size = size();
        resize_uninit(new_size);

        if (new_size > old_size)
        {
            memzero(data() + old_size, (new_size - old_size) * size_of<T>);
        }
    }

    void resize(isize_t new_size)
        requires default_constructible<T>
    {
        resize_inner(new_size);
    }

    void resize(isize_t new_size, const T& value)
    {
        resize_inner(new_size, value);
    }

    constexpr T& push(auto&&... args)
        requires constructible_from<T, decltype(args)&&...>
    {
        T* uninit = push_uninit();
        T* init = construct_at<T>(uninit, ASL_FWD(args)...);
        return *init;
    }

    auto data(this auto&& self)
    {
        using return_type = un_ref_t<copy_cref_t<decltype(self), T>>*;
        if constexpr (kInlineCapacity == 0)
        {
            return return_type{ self.m_data };
        }
        else
        {
            return self.is_on_heap() ? return_type{ self.m_data } : reinterpret_cast<return_type>(&self);
        }
    }

    constexpr auto begin(this auto&& self)
    {
        using type = un_ref_t<copy_cref_t<decltype(self), T>>;
        return contiguous_iterator<type>{self.data()};
    }

    constexpr auto end(this auto&& self)
    {
        using type = un_ref_t<copy_cref_t<decltype(self), T>>;
        return contiguous_iterator<type>{self.data() + self.size()};
    }

    constexpr operator span<const T>() const // NOLINT(*-explicit-conversions)
    {
        return as_span();
    }

    constexpr operator span<T>() // NOLINT(*-explicit-conversions)
    {
        return as_span();
    }

    constexpr auto as_span(this auto&& self)
    {
        using type = un_ref_t<copy_cref_t<decltype(self), T>>;
        return span<type>{self.data(), self.size()};
    }

    constexpr auto&& operator[](this auto&& self, isize_t i)
    {
        ASL_ASSERT(i >= 0 && i <= self.size());
        return ASL_FWD_LIKE(decltype(self), ASL_FWD(self).data()[i]);
    }

    template<typename H>
    requires hashable<T>
    friend H AslHashValue(H h, const buffer& b)
    {
        return H::combine_contiguous(ASL_MOVE(h), b.as_span());
    }
};

} // namespace asl

