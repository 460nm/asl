// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#include "asl/base/assert.hpp"
#include "asl/base/support.hpp"
#include "asl/base/meta.hpp"
#include "asl/base/memory.hpp"
#include "asl/allocator/allocator.hpp"
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
        requires is_default_constructible<Allocator>
        : m_ptr{nullptr}
        , m_alloc{}
    {}

    constexpr box(T* ptr, Allocator alloc)
        : m_ptr{ptr}
        , m_alloc{std::move(alloc)}
    {
        ASL_ASSERT(m_ptr != nullptr);
    }

    constexpr box(box&& other)
        : m_ptr{std::exchange(other.m_ptr, nullptr)}
        , m_alloc{std::move(other.m_alloc)}
    {}

    template<is_object U>
    requires convertible_to<U*, T*>
    constexpr box(box<U, Allocator>&& other) // NOLINT(*explicit*,*-not-moved)
        : m_ptr{std::exchange(other.m_ptr, nullptr)}
        , m_alloc{std::move(other.m_alloc)}
    {}

    constexpr box& operator=(box&& other)
    {
        if (this == &other) { return *this; }

        if (m_ptr != nullptr) { reset(); }

        m_ptr = std::exchange(other.m_ptr, nullptr);
        m_alloc = std::move(other.m_alloc);

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

    constexpr auto* get(this auto&& self)
    {
        return self.m_ptr;
    }

    constexpr auto&& operator*(this auto&& self)
    {
        ASL_ASSERT(self.m_ptr != nullptr);
        return std::forward_like<decltype(self)&&>(*self.m_ptr);
    }

    constexpr auto* operator->(this auto&& self)
    {
        ASL_ASSERT(self.m_ptr != nullptr);
        return self.m_ptr;
    }

    constexpr bool operator==(niche_t) const
    {
        return m_ptr == nullptr;
    }

    template<typename H>
    requires hashable<T>
    friend H AslHashValue(H h, const box& b)
    {
        return H::combine(std::move(h), *b);
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
    auto* ptr = construct_at<T>(raw_ptr, std::forward<Args>(args)...);
    return box(ptr, std::move(allocator));
}

template<is_object T, allocator Allocator = DefaultAllocator, typename... Args>
constexpr box<T, Allocator> make_box(Args&&... args)
    requires is_default_constructible<Allocator> && constructible_from<T, Args&&...>
{
    Allocator allocator{};
    void* raw_ptr = allocator.alloc(layout::of<T>());
    auto* ptr = construct_at<T>(raw_ptr, std::forward<Args>(args)...);
    return box<T>(ptr, std::move(allocator));
}

template<is_object T, allocator A>
constexpr T* leak(box<T, A>&& b)
{
    return std::exchange(std::move(b).m_ptr, nullptr);
}

} // namespace asl

