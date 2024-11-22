#pragma once

#include "asl/allocator.hpp"
#include "asl/assert.hpp"
#include "asl/annotations.hpp"
#include "asl/memory.hpp"
#include "asl/utility.hpp"
#include "asl/box.hpp"

namespace asl
{

template<is_object T, allocator Allocator = DefaultAllocator>
class box
{
    T* m_ptr;
    ASL_NO_UNIQUE_ADDRESS Allocator m_alloc;
    
public:
    explicit constexpr box(niche)
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
            if constexpr (!trivially_destructible<T>)
            {
                m_ptr->~T();
            }
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

    constexpr bool operator==(niche) const
    {
        return m_ptr == nullptr;
    }
};

template<is_object T, allocator Allocator = DefaultAllocator, typename... Args>
constexpr box<T, Allocator> make_box_in(Allocator allocator, Args&&... args)
    requires constructible_from<T, Args&&...>
{
    void* raw_ptr = allocator.alloc(layout::of<T>());
    T* ptr = new (raw_ptr) T(ASL_FWD(args)...);
    return box(ptr, ASL_MOVE(allocator));
}

template<is_object T, allocator Allocator = DefaultAllocator, typename... Args>
constexpr box<T, Allocator> make_box(Args&&... args)
    requires default_constructible<Allocator> && constructible_from<T, Args&&...>
{
    Allocator allocator{};
    void* raw_ptr = allocator.alloc(layout::of<T>());
    T* ptr = new (raw_ptr) T{ ASL_FWD(args)... };
    return box<T>(ptr, ASL_MOVE(allocator));
}

} // namespace asl

