// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#include "asl/base/assert.hpp"
#include "asl/base/annotations.hpp"
#include "asl/base/utility.hpp"
#include "asl/memory/memory.hpp"
#include "asl/memory/allocator.hpp"
#include "asl/hashing/hash.hpp"

namespace asl
{

template<is_object T, allocator Allocator = DefaultAllocator>
class box
{
    T* m_ptr;
    ASL_NO_UNIQUE_ADDRESS Allocator m_alloc;

public:
    explicit constexpr box(niche_t)
        requires default_constructible<Allocator>
        : m_ptr{nullptr}
        , m_alloc{}
    {}

    constexpr box(T* ptr, Allocator alloc)
        : m_ptr{ptr}
        , m_alloc{ASL_MOVE(alloc)}
    {
        ASL_ASSERT(m_ptr != nullptr);
    }

    constexpr box(box&& other)
        : m_ptr{exchange(other.m_ptr, nullptr)}
        , m_alloc{ASL_MOVE(other.m_alloc)}
    {}

    template<is_object U>
    requires convertible_from<T*, U*>
    constexpr box(box<U, Allocator>&& other) // NOLINT(*-explicit-conversions)
        : m_ptr{exchange(other.m_ptr, nullptr)}
        , m_alloc{ASL_MOVE(other.m_alloc)}
    {}

    constexpr box& operator=(box&& other)
    {
        if (this == &other) { return *this; }

        if (m_ptr != nullptr) { reset(); }

        m_ptr = exchange(other.m_ptr, nullptr);
        m_alloc = ASL_MOVE(other.m_alloc);

        return *this;
    }

    box(const box&) = delete;
    box& operator=(const box&) = delete;

    constexpr ~box()
    {
        reset();
    }

    constexpr void reset()
    {
        if (m_ptr != nullptr)
        {
            destroy(m_ptr);
            m_alloc.dealloc(m_ptr, layout::of<T>());
            m_ptr = nullptr;
        }
    }

    constexpr T* get() const { return m_ptr; }

    constexpr T& operator*() const
    {
        ASL_ASSERT(m_ptr != nullptr);
        return *m_ptr;
    }

    constexpr T* operator->() const
    {
        ASL_ASSERT(m_ptr != nullptr);
        return m_ptr;
    }

    constexpr bool operator==(niche_t) const
    {
        return m_ptr == nullptr;
    }

    template<typename H>
    requires hashable<T>
    friend H AslHashValue(H h, const box& b)
    {
        return H::combine(ASL_MOVE(h), *b);
    }

    template<is_object U, allocator A>
    friend constexpr U* leak(box<U, A>&&);

    template<is_object U, allocator A>
    friend class box;
};

template<is_object T, allocator Allocator = DefaultAllocator, typename... Args>
constexpr box<T, Allocator> make_box_in(Allocator allocator, Args&&... args)
    requires constructible_from<T, Args&&...>
{
    void* raw_ptr = allocator.alloc(layout::of<T>());
    auto* ptr = construct_at<T>(raw_ptr, ASL_FWD(args)...);
    return box(ptr, ASL_MOVE(allocator));
}

template<is_object T, allocator Allocator = DefaultAllocator, typename... Args>
constexpr box<T, Allocator> make_box(Args&&... args)
    requires default_constructible<Allocator> && constructible_from<T, Args&&...>
{
    Allocator allocator{};
    void* raw_ptr = allocator.alloc(layout::of<T>());
    auto* ptr = construct_at<T>(raw_ptr, ASL_FWD(args)...);
    return box<T>(ptr, ASL_MOVE(allocator));
}

template<is_object T, allocator A>
constexpr T* leak(box<T, A>&& b)
{
    return exchange(b.m_ptr, nullptr);
}

} // namespace asl

