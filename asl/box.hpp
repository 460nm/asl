#pragma once

#include "asl/allocator.hpp"
#include "asl/annotations.hpp"

namespace asl
{

template<is_object T, allocator Allocator = DefaultAllocator>
class box
{
    T* m_ptr;
    ASL_NO_UNIQUE_ADDRESS Allocator m_alloc;
    
public:
};

} // namespace asl

