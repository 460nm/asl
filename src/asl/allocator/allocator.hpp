// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#include "asl/base/meta.hpp"
#include "asl/base/layout.hpp"
#include "asl/base/memory_ops.hpp"

namespace asl
{

template<typename T>
concept allocator = movable<T> && equality_comparable<T> &&
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
    return construct_at<T>(ptr, std::forward<decltype(args)>(args)...);
}

template<typename T>
T* alloc_uninit(allocator auto& a)
    requires is_trivially_default_constructible<T>
{
    void* ptr = a.alloc(layout::of<T>());
    return reinterpret_cast<T*>(ptr); // NOLINT(*-reinterpret-cast)
}

template<typename T>
T* alloc_uninit_unsafe(allocator auto& a)
{
    void* ptr = a.alloc(layout::of<T>());
    return reinterpret_cast<T*>(ptr); // NOLINT(*-reinterpret-cast)
}

template<typename T>
void alloc_delete(allocator auto& a, T* ptr)
{
    destroy_at(ptr);
    a.dealloc(ptr, layout::of<T>());
}

template<typename T>
constexpr T* alloc_new_default(auto&&... args)
{
    DefaultAllocator allocator{};
    return alloc_new<T>(allocator, std::forward<decltype(args)>(args)...);
}

template<typename T>
void alloc_delete_default(T* ptr)
{
    DefaultAllocator allocator{};
    alloc_delete(allocator, ptr);
}

} // namespace asl
