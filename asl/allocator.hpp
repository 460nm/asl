#pragma once

#include "asl/layout.hpp"
#include "asl/meta.hpp"

namespace asl
{

template<typename T>
concept allocator = moveable<T> && equality_comparable<T> &&
    requires(T& alloc, layout layout, void* ptr)
    {
        { alloc.alloc(layout) } -> same_as<void*>;
        { alloc.realloc(ptr, layout, layout) } -> same_as<void*>;
        alloc.dealloc(ptr, layout);
    };

class GlobalHeap
{
public:
    static void* alloc(const layout&);
    static void* realloc(void* ptr, const layout& old, const layout& new_layout);
    static void dealloc(void* ptr, const layout&);
    
    constexpr bool operator==(const GlobalHeap&) const { return true; }
};
static_assert(allocator<GlobalHeap>);

using DefaultAllocator = GlobalHeap;

} // namespace asl
