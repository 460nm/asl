// Copyright 2025 Steven Le Rouzic
//
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#include "asl/base/utility.hpp"
#include "asl/base/assert.hpp"
#include "asl/base/numeric.hpp"
#include "asl/containers/buffer.hpp"
#include "asl/memory/allocator.hpp"
#include "asl/types/array.hpp"
#include "asl/types/maybe_uninit.hpp"

namespace asl
{

template<
    is_object T,
    isize_t kChunkSize,
    allocator Allocator = DefaultAllocator>
class chunked_buffer
{
    static_assert(kChunkSize > 0 && is_pow2(kChunkSize));

    using Chunk = array<maybe_uninit<T>, kChunkSize>;

    buffer<Chunk*, Allocator>    m_chunks;
    isize_t                      m_size{};

public:
    constexpr chunked_buffer()
        requires default_constructible<Allocator>
        = default;

    explicit constexpr chunked_buffer(Allocator allocator)
        : m_chunks{std::move(allocator)}
    {}

    // @Todo copy constructor
    constexpr chunked_buffer(const chunked_buffer& other)
        requires copyable<T> && copy_constructible<Allocator>
    {}

    constexpr chunked_buffer(chunked_buffer&& other)
        : m_chunks{std::move(other.m_chunks)}
        , m_size{asl::exchange(other.m_size, 0)}
    {
        ASL_ASSERT(m_chunks.size() == 0);
    }
    
    // @Todo copy assignment
    constexpr chunked_buffer& operator=(const chunked_buffer& other)
        requires copyable<T>
    {
        if (&other == this) { return *this; }
        return *this;
    }
    
    // @Todo move assignment
    constexpr chunked_buffer& operator=(chunked_buffer&& other)
    {
        if (&other == this) { return *this; }
        return *this;
    }

    ~chunked_buffer()
    {
        destroy();
    }

    void clear()
    {
        if constexpr (trivially_destructible<T>)
        {
            m_size = 0;
        }
        else
        {
            for (Chunk* chunk: m_chunks)
            {
                if (m_size == 0) { break; }

                isize_t in_chunk = asl::min(m_size, kChunkSize);
                destroy_n(&(*chunk)[0].as_init_unsafe(), in_chunk);
            
                m_size -= in_chunk;
            }
            ASL_ASSERT(m_size == 0);
        }
    }

    void destroy()
    {
        clear();
        ASL_ASSERT(size() == 0);
        
        for (Chunk* chunk:  m_chunks)
        {
            alloc_delete(m_chunks.allocator(), chunk);
        }

        m_chunks.destroy();
    }

    [[nodiscard]] constexpr isize_t size() const { return m_size; }

    [[nodiscard]] constexpr bool is_empty() const { return size() == 0; }

    [[nodiscard]] constexpr isize_t capacity() const
    {
        return m_chunks.size() * kChunkSize;
    }

    // @Todo iterators
    // @Todo element access
    // @Todo push
    // @Todo reserve_capacity
    // @Todo resize (default/with value)
    // @Todo resize_zero
    // @Todo resize_uninit
};

} // namespace asl

