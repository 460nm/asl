#pragma once

#include "asl/meta.hpp"
#include "asl/allocator.hpp"
#include "asl/annotations.hpp"
#include "asl/memory.hpp"

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
    
    static_assert(align_of<T> <= align_of<T*>);
    static_assert(align_of<T*> == align_of<isize_t>);
    static_assert(align_of<T*> == align_of<size_t>);

    constexpr size_t load_size_encoded() const
    {
        size_t s{};
        asl::memcpy(&s, &m_size_encoded_, sizeof(size_t));
        return s;
    }

    static constexpr bool is_on_heap(size_t size_encoded)
    {
        return (size_encoded & kOnHeapMask) != 0;
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
   
public:

    static constexpr isize_t kInlineCapacity = []() {
        // 1 byte is used for size inline in m_size_encoded.
        // This is enough because we have at most 24 bytes available,
        // so 23 chars of capacity.
        const isize_t available_size = size_of<T*> + size_of<isize_t> + size_of<size_t> - 1;
        return available_size / size_of<T>;
    }();

    constexpr buffer() requires default_constructible<Allocator> = default;

    explicit constexpr buffer(Allocator allocator)
        : m_allocator{ASL_MOVE(allocator)}
    {}

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
            return is_on_heap(load_size_encoded())
                ? m_capacity
                : kInlineCapacity;
        }
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
            return is_on_heap(load_size_encoded())
                ? m_data
                : reinterpret_cast<const T*>(this);
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
            return is_on_heap(load_size_encoded())
                ? m_data
                : reinterpret_cast<T*>(this);
        }
    }
};

} // namespace asl

