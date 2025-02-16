#pragma once

#include "asl/base/meta.hpp"
#include "asl/memory/layout.hpp"
#include "asl/memory/memory.hpp"

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

template<typename T>
T* alloc_new(allocator auto& a, auto&&... args)
{
    void* ptr = a.alloc(layout::of<T>());
    return construct_at<T>(ptr, ASL_FWD(args)...);
}

template<typename T>
void alloc_delete(allocator auto& a, T* ptr)
{
    destroy(ptr);
    a.dealloc(ptr, layout::of<T>());
}

template<typename T>
constexpr T* alloc_new_default(auto&&... args)
{
    return alloc_new<T>(DefaultAllocator{}, ASL_FWD(args)...);
}

template<typename T>
void alloc_delete_default(T* ptr)
{
    alloc_delete(DefaultAllocator{}, ptr);
}

} // namespace asl
