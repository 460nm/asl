#pragma once

#include "asl/meta.hpp"
#include "asl/allocator.hpp"
#include "asl/annotations.hpp"

namespace asl
{

template<is_object T, allocator Allocator = DefaultAllocator>
class buffer
{
    
    T*         m_data{};
    isize_t    m_capacity{};

    // bit 63       : 0 = on heap, 1 = inline
    // bits [62:56] : size when inline
    // bits [52:0]  : size when on heap
    size_t     m_size_encoded{};

    ASL_NO_UNIQUE_ADDRESS Allocator m_allocator;
    
    static_assert(align_of<T> <= align_of<T*>);
    static_assert(align_of<T*> == align_of<isize_t>);
    static_assert(align_of<T*> == align_of<size_t>);
    
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
};

} // namespace asl

