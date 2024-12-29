#pragma once

#include "asl/meta.hpp"
#include "asl/allocator.hpp"
#include "asl/annotations.hpp"
#include "asl/memory.hpp"
#include "asl/assert.hpp"

namespace asl
{

template<is_object T, allocator Allocator = DefaultAllocator>
class buffer
{
    
    T*         m_data{};
    isize_t    m_capacity{};

public:
    static constexpr isize_t kInlineCapacity = []() {
        // 1 byte is used for size inline in m_size_encoded.
        // This is enough because we have at most 24 bytes available,
        // so 23 chars of capacity.
        const isize_t available_size = size_of<T*> + size_of<isize_t> + size_of<size_t> - 1;
        return available_size / size_of<T>;
    }();

private:
    static constexpr size_t kOnHeapMask = 0x8000'0000'0000'0000ULL;

    // bit 63       : 1 = on heap, 0 = inline
    // bits [62:56] : size when inline
    // bits [62:0]  : size when on heap
    size_t     m_size_encoded_{};

    ASL_NO_UNIQUE_ADDRESS Allocator m_allocator;
    
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
        reserve_capacity(sz + 1);
        set_size(sz + 1);
        return data() + sz;
    }

    constexpr void set_size(isize_t new_size)
    {
        ASL_ASSERT(new_size >= 0);
        ASL_ASSERT_RELEASE(new_size <= capacity());
        size_t size_encoded = load_size_encoded();
        if (kInlineCapacity == 0 || is_on_heap(size_encoded))
        {
            store_size_encoded(encode_size_heap(new_size));
        }
        else
        {
            ASL_ASSERT(new_size <= kInlineCapacity);
            size_encoded = (size_encoded & size_t{0x00ff'ffff'ffff'ffff}) | (bit_cast<size_t>(new_size) << 56);
            store_size_encoded(size_encoded);
        }
    }

public:
    constexpr buffer() requires default_constructible<Allocator> = default;

    explicit constexpr buffer(Allocator allocator)
        : m_allocator{ASL_MOVE(allocator)}
    {}

    // @Todo Destructor
    // @Todo clear
    // @Todo Copy/move constructor & assignment
    // @Todo Do leak checks on Linux

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

    constexpr T& push(auto&&... args)
        requires constructible_from<T, decltype(args)&&...>
    {
        T* uninit = push_uninit();
        T* init = construct_at<T>(uninit, ASL_FWD(args)...);
        return *init;
    }

    // @Todo(C++23) Use deducing this
    const T* data() const
    {
        if constexpr (kInlineCapacity == 0)
        {
            return m_data;
        }
        else
        {
            return is_on_heap() ? m_data : reinterpret_cast<const T*>(this);
        }
    }

    T* data()
    {
        if constexpr (kInlineCapacity == 0)
        {
            return m_data;
        }
        else
        {
            return is_on_heap() ? m_data : reinterpret_cast<T*>(this);
        }
    }

    // @Todo(C++23) Use deducing this
    constexpr T& operator[](isize_t i)
    {
        ASL_ASSERT(i >= 0 && i <= size());
        return data()[i];
    }
    
    constexpr const T& operator[](isize_t i) const
    {
        ASL_ASSERT(i >= 0 && i <= size());
        return data()[i];
    }
};

} // namespace asl

